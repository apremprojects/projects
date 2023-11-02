#pragma once

class PidControl {
public:
  PidControl();

  int16_t process(const float v, const float dt);

  void setTarget(const float target);
  void setGains(const float kp, const float ki, const float kd);

  void clear();

  float currentTarget() const { return target; }
  float currentError() const { return error; }
  float integralError() const { return error_sum; }
  float getKp() const {return Kp;}
  float getKi() const {return Ki;}
  float getKd() const {return Kd;}

private:
  int16_t getSpeed(const float error, const float der_error, const float dt, float& sum_error);

  float Kp;
  float Ki;
  float Kd;

  float error;
  float target;
  float error_sum;
  float prev_error;
};