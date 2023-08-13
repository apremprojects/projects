#pragma once

#include <QMutex>
#include <QThread>
#include <QSerialPort>

#include <queue>
#include <Eigen/Core>

struct ImuData {
  Eigen::Vector3f acc;
  Eigen::Vector3f gyro;
  Eigen::Vector3f mag;
};
class WorkThread : public QThread {
  Q_OBJECT
public:
  WorkThread(const QString& port_name, QObject* parent);
  ~WorkThread() { stop(); }

  ImuData read();

  void stop();

signals:
  void readFailed();

private:
  void run() override;

  QMutex mutex;
  ImuData imu_data;
  bool done = false;

  const QString port_name;
};
