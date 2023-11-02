#include <Romi32U4.h>
#include "romi_pid_motor_control.h"

RomiPidMotorControl::RomiPidMotorControl()
  : motor_left(),
    motor_right() {}

void RomiPidMotorControl::process(const float v_left, const float v_right, const float dt, int16_t& pwm_left, int16_t& pwm_right) {
  pwm_left = motor_left.process(v_left, dt);
  pwm_right = motor_right.process(v_right, dt);
}

void RomiPidMotorControl::setTarget(const float v_left, const float v_right) {
  motor_left.setTarget(v_left);
  motor_right.setTarget(v_right);
}

void RomiPidMotorControl::setGains(const float kp, const float ki, const float kd) {
  motor_left.setGains(kp, ki, kd);
  motor_right.setGains(kp, ki, kd);
}

float RomiPidMotorControl::leftTarget() const {
  return motor_left.currentTarget();
}

float RomiPidMotorControl::rightTarget() const {
  return motor_right.currentTarget();
}
