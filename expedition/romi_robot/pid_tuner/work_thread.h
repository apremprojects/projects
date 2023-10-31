#pragma once

#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QSerialPort>

#include <Eigen/Core>

struct WheelData {
	int left_v;
	int right_v;
	int left_sp;
	int right_sp;
	int left_pwm;
	int right_pwm;
	float timestamp;
};

struct WorkParams {
  QString port_name;
  int kp;
  int ki;
  int kd;
  int left_sp;
  int right_sp;
  int sampling_time_ms;
  unsigned duration_sec;
  bool sp_changed = false;
};

class WorkThread : public QThread {
  Q_OBJECT
public:
  WorkThread(QObject* parent, QMutex& mutex, QQueue<WheelData>& queue, const WorkParams& params);
  ~WorkThread();
  void stop();
private:
  void run() override;
  void read();
  void setGains(const int kp, const int ki, const int kd, const int period_ms);
  void setSetpoints(const int left_wheel, const int right_wheel);
  bool done = false;
  int samples = 0;
  QMutex& mutex;
  QQueue<WheelData>& queue;
  const WorkParams& params;
  char buffer[2048];
  QSerialPort* serial_port;
};
