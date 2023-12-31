#include <Romi32U4.h>

#include "romi_pid_motor_control.h"
#include "circular_queue.h"

struct MotionCmd {
	MotionCmd() : MotionCmd(0, 0, 0) {};
	MotionCmd(const int8_t lv, const int8_t rv, const int16_t tick)
		: left_v(lv),
		  right_v(rv),
		  ticks(tick) {};
	int8_t left_v;
	int8_t right_v;
	int16_t ticks;
};

char cmd;
char line[100];
int delay_ms;
int16_t prev_counts_left;
int16_t prev_counts_right;

unsigned long prev_time_ms;

uint32_t tick_count;
uint32_t duration_in_ticks;
uint32_t tick_count_last_hb;
uint32_t halt_count_ticks;

CircularQueue<MotionCmd> queue;
RomiPidMotorControl motors_pid;

char read() {
  char ch = '\n';
  if (Serial.available()) {
    ch = Serial.read();
  }
  return ch;
}

void read_line(char* line) {
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
  long int kp, ki, kd;
  read_line(line);
  sscanf(line, " %ld, %ld, %ld, %d", &kp, &ki, &kd, &delay_ms);
  //prev_time_ms = millis();
  tick_count_last_hb = 0;
  halt_count_ticks = 1000/delay_ms;
  sprintf(line, "k: %ld, %ld, %ld, %d, %u, %lu\r\n", kp, ki, kd, delay_ms, halt_count_ticks, millis());
  Serial.print(line);
  motors_pid.setGains(static_cast<float>(kp)/1000.0f, static_cast<float>(ki)/1000.0f, static_cast<float>(kd)/1000.0f);
  motors_pid.setTarget(0.0f, 0.0f);
  Romi32U4Motors::setSpeeds(0, 0);
}

void setTargets() {
  int left_v, right_v;
  read_line(line);
  sscanf(line, " %d, %d, %u", &left_v, &right_v, &duration_in_ticks);
  //prev_time_ms = millis();
  sprintf(line, "t: %d, %d, %u, %lu\r\n", left_v, right_v, duration_in_ticks, millis());
  Serial.print(line);
  MotionCmd m_cmd(left_v, right_v, duration_in_ticks);
  if(!queue.isFull()){
    queue.push(m_cmd);
  }
  //motors_pid.setTarget(static_cast<float>(left_v), static_cast<float>(right_v));
  //tick_count = 0;
  //tick_count_last_hb = 0;
}

void heartBeat() {
  tick_count_last_hb = 0;
}

int16_t velocity(const int16_t curr_count, const int16_t prev_count) {
  const uint16_t curr_sign = ((uint16_t)curr_count) & 0x8000;
  const uint16_t prev_sign = ((uint16_t)prev_count) & 0x8000;
  const int16_t diff = (curr_sign == prev_sign)?(curr_count - prev_count):(((uint16_t)curr_count) - prev_count);
  return diff;
}

void reportBattery(){
  const uint16_t v = readBatteryMillivolts();
  sprintf(line, "b: %hu\r\n", v);
  Serial.print(line);
}

void setup() {
  Romi32U4Buzzer::playNote(NOTE_C(4), 200, 15);
  Serial.begin(115200);
  while (!Serial);
  delay_ms = 10;
  prev_counts_left = 0;
  prev_counts_right = 0;
  prev_time_ms = 0;
  tick_count_last_hb = 0;
  tick_count = 0;
  duration_in_ticks = 0;
  motors_pid.setTarget(0.0f, 0.0f);
  Romi32U4Encoders::init();
}

void loop() {
  cmd = read();
  switch (cmd) {
    case 'k':
      setGains();
      break;
    case 't':
      setTargets();
      break;
    case 's':
      heartBeat();
      break;
    case 'b':
      reportBattery();
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

  if (tick_count > duration_in_ticks) {
    if (queue.isEmpty()){
      motors_pid.setTarget(0.0f, 0.0f);
      Romi32U4Motors::setSpeeds(0, 0);
      return;
    } else{
      MotionCmd m_cmd;
      queue.front(m_cmd);
      queue.pop();
      motors_pid.setTarget(m_cmd.left_v, m_cmd.right_v);
      duration_in_ticks = m_cmd.ticks;
      tick_count = 0;
      tick_count_last_hb = 0;
    }
  }
  
  if(tick_count_last_hb > halt_count_ticks){
    queue.clear();
    motors_pid.setTarget(0.0f, 0.0f);
    Romi32U4Motors::setSpeeds(0, 0);
    return;
  }
  float dt = ((float)diff_ms)/1000.0f;
  int16_t counts_left = Romi32U4Encoders::getCountsLeft();
  int16_t counts_right = Romi32U4Encoders::getCountsRight();
  int16_t v_left = velocity(counts_left, prev_counts_left);
  int16_t v_right = velocity(counts_right, prev_counts_right);
  prev_counts_left = counts_left;
  prev_counts_right = counts_right;
  int16_t pwm_left, pwm_right;
  motors_pid.process(static_cast<float>(v_left), static_cast<float>(v_right), dt, pwm_left, pwm_right);
  Romi32U4Motors::setSpeeds(pwm_left, pwm_right);
  sprintf(line, "s %hd, %hd, %hd, %hd, %lu, %d, %d, %d\r\n", 
    v_left, v_right, pwm_left, pwm_right, curr_time_ms,
    (int)duration_in_ticks, (int)tick_count, queue.size());
  Serial.print(line);
}
