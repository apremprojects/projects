#pragma once

#include <QMutex>
#include <QQueue>
#include <QThread>

#include <Eigen/Core>

class WorkThread : public QThread {
  Q_OBJECT
public:
  WorkThread(QObject* parent, QMutex& mutex, QQueue<std::pair<Eigen::Vector2f, float>>& queue);
  ~WorkThread();

  void stop();

private:
  void run() override;

  bool done = false;

  QMutex& mutex;
  QQueue<std::pair<Eigen::Vector2f, float>>& queue;
};

#if 0
#include <QThread>
#include <QVariant>
#include "imu_data.h"

Q_DECLARE_METATYPE(ImuData);

class WorkThread : public QThread {
  Q_OBJECT
public:
  WorkThread(QObject* parent);

signals:
  void sensorData(const QVariant data);

private:
  void run() override;
};
#endif