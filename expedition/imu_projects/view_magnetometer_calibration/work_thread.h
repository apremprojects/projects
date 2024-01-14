#pragma once

#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QSerialPort>

#include <Eigen/Core>

struct MagnetometerData {
  Eigen::Vector3f mag;
};

struct WorkParams {
  QString port_name;
  int sampling_time_ms;
};

class WorkThread : public QThread {
  Q_OBJECT
public:
  WorkThread(QObject* parent, QMutex& mutex, QQueue<MagnetometerData>& queue, const WorkParams& params);
  ~WorkThread();

  void stop();

private:
  void run() override;
  void read(QSerialPort& port, char* buffer);
  bool done = false;

  QMutex& mutex;
  QQueue<MagnetometerData>& queue;
  const WorkParams& params;
};
