#pragma once
#include <QSerialPort>

struct status_t{
	unsigned tick_count;
	unsigned duration_in_ticks;
	int16_t left_v, right_v;
	int16_t left_pwm, right_pwm;
	unsigned long current_time;
	int queue_size;
};

struct MotionCmd {
	MotionCmd() : MotionCmd(0, 0, 0) {};
	MotionCmd(const int lv, const int rv, const int tick)
		: left_v(lv),
		right_v(rv),
		ticks(tick) {};
	int left_v;
	int right_v;
	int ticks;
};

class RomiRobot{
public:
	RomiRobot(const QString port_name);
	void setGains(const int kp, const int ki, const int kd, const int kt);
	void setSetpoints(const MotionCmd &cmd);
	status_t readStatus();
	bool isReady();
	void sendHeartbeat();
	uint16_t getBatVoltage();

private:
	void read();
	bool ready = true;
	QSerialPort port;
	char buffer[2048];
};