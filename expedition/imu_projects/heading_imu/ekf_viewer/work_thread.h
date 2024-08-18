#pragma once

#include <QQueue>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include <Eigen/Core>
#include <Eigen/Geometry>

class WorkThread : public QThread {
  Q_OBJECT
public:
  WorkThread(QObject* parent, QMutex& mutex, QQueue<Eigen::Quaternionf>& queue, const QString& port_name);
  ~WorkThread();

  void stop();

private:
  void run() override;

  bool done = false;

  QMutex& mutex;
  QQueue<Eigen::Quaternionf>& queue;

  const QString& port_name;
};
