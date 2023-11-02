#include "pid_control.h"

PidControl::PidControl()
  : target(0.0f) {
  setGains(0.0f, 0.0f, 0.0f);
}

int16_t PidControl::process(const float v, const float dt) {
  if (target == 0.0f) {
    return 0;
  }
  error = target - v;
  error_sum += (error*dt);
  float der_error = (error - prev_error)/dt;
  prev_error = error;
  return getSpeed(error, der_error, dt, error_sum);
}

void PidControl::clear() {
  error = 0.0f,
  error_sum = 0.0f;
  prev_error = 0.0f;
}

void PidControl::setTarget(const float t) {
  target = t;
}

void PidControl::setGains(const float kp, const float ki, const float kd) {
  Kp = kp;
  Ki = ki;
  Kd = kd;
  clear();
}

int16_t PidControl::getSpeed(const float error, const float der_error, const float dt, float& sum_error) {
  int16_t s = static_cast<int16_t>(Kp*error + Ki*sum_error + Kd*der_error);
  if (s < -300) {
    s = -300;
    sum_error -= (error*dt);   
  } else if (s > 300) {
    s = 300;
    sum_error -= (error*dt);
  }
  return s;
}
