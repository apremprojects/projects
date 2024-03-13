#include <Romi32U4.h>
#include "pid_control.h"
#include "circular_queue.h"
#include <math.h>

// k 600, 110000, 1, 500, 0, 0, 500, 100000, 0, 25

struct Position {
  double x;
  double y;
};

char cmd;
char line[200];
int delay_ms;
int16_t prev_counts_left;
int16_t prev_counts_right;

unsigned long prev_time_ms;
int tick_count;
int duration_in_ticks;
int tick_count_last_hb;
int halt_count_ticks;
long int kp_v, ki_v, kd_v;
long int kp_dist, ki_dist, kd_dist;
long int kp_yaw, ki_yaw, kd_yaw;
bool is_idle;
bool is_oriented;

PidControl left_v_pid;
PidControl right_v_pid;
PidControl dist_pid;
PidControl heading_pid;

double yaw;
Position origin;
Position dest;
CircularQueue<Position> wps_q;

double radToDeg(const double theta) {
  return (180.0*theta)/M_PI;
}

double degToRad(const double deg){
  return (deg/180.0) * M_PI;
}

double ticksToDistance(const double enc_ticks) {
  return (0.07*M_PI/1440.0)*enc_ticks;
}

double ticksToAngle(const double enc_ticks) {
  return ticksToDistance(enc_ticks)/0.14;
}

double distanceToTicks(const double d) {
  return (1440.0*d)/(0.07*M_PI);
}

double angleToTicks(const double theta_rad) {
  return distanceToTicks(0.07*theta_rad);
}

double distance(const Position& p1, const Position& p2) {
  const double dx = p1.x - p2.x;
  const double dy = p1.y - p2.y;
  return sqrt(dx*dx + dy*dy);
}

double headingDiff(const double yaw, const Position& src, const Position& dest) {
  const double dist = distance(dest, src);
  const Position dest_dir = {(dest.x-src.x)/dist, (dest.y-src.y)/dist};
  const Position head_dir = {cos(yaw), sin(yaw)};
  const double angle = acos((dest_dir.x*head_dir.x + dest_dir.y*head_dir.y));
  const double cross_prod = head_dir.x*dest_dir.y - head_dir.y*dest_dir.x;
  return (cross_prod < 0.0)?(-angle):angle;
}

float clamp(const float val, const float min_range, const float max_range) {
  return min(max(min_range, val), max_range);
}

char read() {
  char ch = '\n';
  if (Serial.available()) {
    ch = Serial.read();
  }
  return ch;
}

void readLine(char* line) {
  char ch;
  int length = 0;
  while ((ch = read()) != '\n') {
    if (ch == '\r') {
      continue;
    }
    line[length++] = ch; 
  }
  line[length] = '\0';
}

void setGains() {
  readLine(line);
  sscanf(line, " %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %d", &kp_v, 
            &ki_v, &kd_v, 
            &kp_dist, &ki_dist, &kd_dist, 
            &kp_yaw, &ki_yaw, &kd_yaw, 
            &delay_ms);
  tick_count_last_hb = 0;
  halt_count_ticks = 1000/delay_ms;
  sprintf(line, "k: %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %d, %d, %lu\r\n", 
              kp_v, ki_v, kd_v, 
              kp_dist, ki_dist, kd_dist, 
              kp_yaw, ki_yaw, kd_yaw, 
              delay_ms, halt_count_ticks, millis());
  Serial.print(line);
  left_v_pid.setGains(static_cast<float>(kp_v)/1000.0f, static_cast<float>(ki_v)/1000.0f, static_cast<float>(kd_v)/1000.0f);
  right_v_pid.setGains(static_cast<float>(kp_v)/1000.0f, static_cast<float>(ki_v)/1000.0f, static_cast<float>(kd_v)/1000.0f);
  dist_pid.setGains(static_cast<float>(kp_dist)/1000.0f, static_cast<float>(ki_dist)/1000.0f, static_cast<float>(kd_dist)/1000.0f);
  heading_pid.setGains(static_cast<float>(kp_yaw)/1000.0f, static_cast<float>(ki_yaw)/1000.0f, static_cast<float>(kd_yaw)/1000.0f);
  reset();
}

void getStatus() {
  tick_count_last_hb = 0;
  sprintf(line, "s: %lf, %lf, %lf, %lu, %d, %d\r\n", origin.x, origin.y, yaw, prev_time_ms, (int)wps_q.isFull(), (int)is_idle);
  Serial.print(line);
}

void wayPoint() {
  int x, y;
  Position pos;
  readLine(line);
  sscanf(line, " %d, %d", &x, &y);
  pos.x = static_cast<double>(x)/1000.0;
  pos.y = static_cast<double>(y)/1000.0;
  wps_q.push(pos);
  sprintf(line, "w: %lf, %lf, %lu\r\n", pos.x, pos.y, millis());
  Serial.print(line);
}

void reset() {
  readLine(line);
  long int tx = 0, ty = 0, tt = 0;
  sscanf(line, " %ld, %ld, %ld\r\n", &tx, &ty, &tt);

  origin.x = static_cast<double>(tx) / 1000.0;
  origin.y = static_cast<double>(ty) / 1000.0;
  yaw = static_cast<double>(tt) / 180.0 * M_PI;

  wps_q.clear();

  dest = origin;
  is_idle = true;
  is_oriented = true;
  Romi32U4Motors::setSpeeds(0, 0);
  left_v_pid.setTarget(0.0f);
  right_v_pid.setTarget(0.0f);
  dist_pid.setTarget(0.0f);
  heading_pid.setTarget(0.0f);
}

int16_t velocity(const int16_t curr_count, const int16_t prev_count) {
  const uint16_t curr_sign = ((uint16_t)curr_count) & 0x8000;
  const uint16_t prev_sign = ((uint16_t)prev_count) & 0x8000;
  const int16_t diff = (curr_sign == prev_sign)?(curr_count - prev_count):(((uint16_t)curr_count) - prev_count);
  return diff;
}

void setup() {
  Romi32U4Buzzer::playNote(NOTE_C(4), 200, 15);
  Serial.begin(115200);
  while (!Serial);
  Serial.print("Romi robot...\r\n");
  delay_ms = 10;
  prev_counts_left = 0;
  prev_counts_right = 0;
  prev_time_ms = 0;
  tick_count_last_hb = 0;
  tick_count = 0;
  duration_in_ticks = 0;
  left_v_pid.setTarget(0.0f);
  right_v_pid.setTarget(0.0f);
  Romi32U4Encoders::init();
  reset();
}

void loop() {
  cmd = read();
  switch (cmd) {
    case 'k':
      setGains();
      break;
    case 's':
      getStatus();
      break;
    case 'w':
      wayPoint();
      break;
    case 'r':
      reset();
      break;
  }
  unsigned long curr_time_ms = millis();
  unsigned long diff_ms = curr_time_ms - prev_time_ms;
  if (diff_ms < delay_ms) {
    return;
  }
  prev_time_ms = curr_time_ms;

  ++tick_count;
  ++tick_count_last_hb;
  /*if (tick_count_last_hb > halt_count_ticks) {
    left_v_pid.setTarget(0.0f);
    right_v_pid.setTarget(0.0f);
    Romi32U4Motors::setSpeeds(0, 0);
    return;
  }*/

  float dt = ((float)diff_ms)/1000.0f;
  const double dist = distance(dest, origin);
  if (abs(dist) < 0.003) {//50 ticks in meters?
    if (wps_q.isEmpty()) {
      is_idle = true;
      Romi32U4Motors::setSpeeds(0, 0);
      return;
    } else {
      wps_q.front(dest);
      wps_q.pop();
      is_idle = false;
      is_oriented = false;
    }
  }
  //calculate velocity in tick/sampling time
  int16_t counts_left = Romi32U4Encoders::getCountsLeft();
  int16_t counts_right = Romi32U4Encoders::getCountsRight();
  int16_t v_left = velocity(counts_left, prev_counts_left);
  int16_t v_right = velocity(counts_right, prev_counts_right);
  float v = (static_cast<float>(v_left) + static_cast<float>(v_right)) / 2.0f;

  double vel = ticksToDistance(v);

  //integrate velocity
  origin.x += vel * cos(yaw);
  origin.y += vel * sin(yaw);

  //set yaw
  yaw += ticksToAngle(v_left-v_right);

  // Run cascade PID distance, angle PID and set the velocity set points
  int16_t tv_l = 0, tv_r = 0, tv_dist = 0, tv_angle = 0, dist_error = 0;
  double heading_error = headingDiff(yaw, origin, dest);
  dist_error = distanceToTicks(dist);
  tv_dist = static_cast<int16_t>(dist_pid.process(dist_error, dt, -15.0f, 15.0f));//dist error, sampling time, and bounds
  tv_angle = static_cast<int16_t>(heading_pid.process(heading_error, dt, -10.0f, 10.0f));

  //display update messages, display as x, y, yaw, target_angle
  //sprintf(line, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %d, %d\r\n", origin.x, origin.y, dest.x, dest.y, yaw, heading_error, dist_error, tv_dist, tv_angle);
  //Serial.print(line);


  if(!is_oriented){ //if it isn't oriented
    //correct till < 5 deg, then set is_oriented to true
    tv_l = tv_angle;
    tv_r = - tv_angle;
    if(abs(heading_error) < degToRad(5)){
      is_oriented = true;
    }
  }
  else if(abs(heading_error) < degToRad(10)){ //if heading error < 10 deg, clamp to small values
    if(dist > 0.03) {//going forwards and turning
      tv_angle = static_cast<int16_t>(heading_pid.process(heading_error, dt, -10.0f, 10.0f) * 0.1);
      tv_l = tv_dist + tv_angle;
      tv_r = tv_dist - tv_angle;
      //Serial.print("here1\r\n");
    }
    else { //when dist < 3cm, refuse to turn
      tv_l = tv_dist;
      tv_r = tv_dist;
      //Serial.print("here2\r\n");
    }
  }
  else { //if angle heading error > 5 deg, stop and adjust to < 2 deg
    is_oriented = false;
  }
  //only rot, only set tv_l = tv_angle, etc.

  left_v_pid.setTarget(static_cast<float>(tv_l));
  right_v_pid.setTarget(static_cast<float>(tv_r));

  prev_counts_left = counts_left;
  prev_counts_right = counts_right;
  int16_t pwm_left = static_cast<int16_t>(left_v_pid.process(static_cast<float>(v_left), dt, -300, 300));
  int16_t pwm_right = static_cast<int16_t>(right_v_pid.process(static_cast<float>(v_right), dt, -300, 300));
  Romi32U4Motors::setSpeeds(-pwm_left, -pwm_right);
  //sprintf(line, "PWMs: %d, %d\r\n", -pwm_left, -pwm_right);
  //Serial.print(line);
}
