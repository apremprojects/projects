#pragma once

#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QSerialPort>

#include <Eigen/Core>

struct ImuData {
  float timestamp;
  Eigen::Vector3f acc;
  Eigen::Vector3f gyro;
  Eigen::Vector3f mag;
};

struct WorkParams {
  QString port_name;
  int sampling_time_ms;
  unsigned duration_sec;
};

class WorkThread : public QThread {
  Q_OBJECT
public:
  WorkThread(QObject* parent, QMutex& mutex, QQueue<ImuData>& queue, const WorkParams& params);
  ~WorkThread();
  void stop();
private:
  void run() override;
  void read(QSerialPort& port, char* buffer);
  bool done = false;
  int samples = 0;
  QMutex& mutex;
  QQueue<ImuData>& queue;
  const WorkParams& params;
};
