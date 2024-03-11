#include "pid_control.h"

PidControl::PidControl()
  : t(0.0f) {
  setGains(0.0f, 0.0f, 0.0f);
}

float PidControl::process(const float v, const float dt, const float min_range, const float max_range) {
  const float error = t - v;
  float der_error = (error - prev_error)/dt;
  prev_error = error;
  sum_error += (error*dt);
  float out = (Kp*error) + (Ki*sum_error) + (Kd*der_error);
  if (out < min_range) {
    out = min_range;
    sum_error -= (error*dt);   
  } else if (out > max_range) {
    out = max_range;
    sum_error -= (error*dt);
  }
  return -out;
}

void PidControl::clear() {
  sum_error = 0.0f;
  prev_error = 0.0f;
}

void PidControl::setTarget(const float target) {
  t = target;
}

void PidControl::setGains(const float kp, const float ki, const float kd) {
  Kp = kp;
  Ki = ki;
  Kd = kd;
  clear();
}
