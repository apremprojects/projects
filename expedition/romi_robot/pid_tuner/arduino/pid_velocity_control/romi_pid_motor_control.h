#pragma once

#include "pid_control.h"

class RomiPidMotorControl {
public:
  RomiPidMotorControl();

  void process(const float v_left, const float v_right, const float dt, int16_t& pwm_left, int16_t& pwm_right);

  float leftTarget() const;
  float rightTarget() const;

  void setTarget(const float v_left, const float v_right);
  void setGains(const float kp, const float ki, const float kd);

private:
  const float Kp = 1000.0f;
  const float Ki = 3000.0f;
  const float Kd = 1.0f;

  PidControl motor_left;
  PidControl motor_right;
};
