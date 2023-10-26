#pragma once

#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QSerialPort>

#include <Eigen/Core>

struct AccelData {
  Eigen::Vector3f acc;
  float time_stamp;
};

struct WorkParams {
  QString port_name;
  int sampling_time_ms;
  unsigned duration_sec;
};

class WorkThread : public QThread {
  Q_OBJECT
public:
  WorkThread(QObject* parent, QMutex& mutex, QQueue<AccelData>& queue, const WorkParams& params);
  ~WorkThread();

  void stop();

private:
  void run() override;
  void read(QSerialPort& port, char* buffer);
  bool done = false;

  QMutex& mutex;
  QQueue<AccelData>& queue;
  const WorkParams& params;
};
