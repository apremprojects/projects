#pragma once

class PidControl {
public:
  PidControl();

  float process(const float v, const float dt, const float min_range, const float max_range);

  void setTarget(const float _t);
  void setGains(const float kp, const float ki, const float kd);

  float target() const { return t; }
  float integralError() const { return sum_error; }

private:
  void clear();

  float Kp;
  float Ki;
  float Kd;

  float t;
  float sum_error;
  float prev_error;
};