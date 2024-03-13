#include "work_thread.h"

#include <QDebug>

#include <Eigen/Core>
#include <cmath>
#include <random>

using Eigen::Vector2f;
using Eigen::Vector2d;

class PidControl {
public: 
  PidControl(const float kp, const float ki, const float kd) :Kp(kp), Ki(ki), Kd(kd) {};
  void setTarget(const float t) { 
    target = t; 
  }
  float process(const float obs, const float dt) { 
    const float err = obs - target; 
    const float der_err = (err - prev_err) / dt; 
    err_integral += (err * dt); 
    prev_err = err; 
    const float out = (Kp * err) + (Ki * err_integral) + (Kd * der_err); 
    return out; 
  }

private: 
  const float Kp;
  const float Ki;
  const float Kd;

  float target = 0.0f;
  float prev_err = 0.0f;
  float err_integral = 0.0f;
};

class Wheel {
public:
  Wheel() : rd{}, gen{ rd() }, dist(0.0f, 1.0f) {}
  
  void setTarget(const int v) {
    t = v; 
    dist = std::normal_distribution<double>(static_cast<double>(t), 1.0f); 
  }
  
  int velocity() { 
    return static_cast<int>(std::round(dist(gen)));
  }
  
  int target() { 
    return t; 
  }

private: 
  int t = 0; 
  std::random_device rd; 
  std::mt19937 gen; 
  std::normal_distribution<double> dist;
};

int clamp(const float val, const int min_val, const int max_val) {
  const int val_int = std::round(val); 
  const int v = std::min(std::max(val_int, min_val), max_val); 
  //qDebug() << "------ " << val << " : " << v;
  return v;
}

WorkThread::WorkThread(QObject* parent, QMutex& mutex, QQueue<std::pair<Eigen::Vector2f, float>>& queue)
	: QThread(parent),
	mutex(mutex),
	queue(queue) {}

WorkThread::~WorkThread() {
	stop();
}

void WorkThread::run() {
  const int vl = 20;
  const int vr = 20;
  const double dt = 0.025;

  const auto ticksToDist = [](const double enc_ticks) {
    return (0.07 * 4.0 * std::atan(1.0) / 1440.0) * enc_ticks;
  };
  const auto distToTicks = [](const double dist) {
    return std::round((1440.0 * dist) / (0.07 * 4.0 * std::atan(1.0)));
  };
  const auto ticksToAngle = [&ticksToDist](const double enc_ticks) {
    return ticksToDist(enc_ticks) / 0.14;
  };
  const auto radToDegree = [](const double theta) {
    return (180.0 * theta) / (4.0 * std::atan(1.0));
  };
  const auto radToTicks = [](double theta_rad) {
    return std::round((1440.0 * theta_rad) / (8.0 * std::atan(1.0)));
  };
  const auto targetHeading = [](const Vector2d& dest, const Vector2d& src) {
    const double dx = dest.x() - src.x();
    const double dy = dest.y() - src.y();
    return std::atan2(dy, dx);
  };

  Wheel left;
  Wheel right;

  PidControl pid(0.125f, 0.0f, 0.0f);
  pid.setTarget(0.0f);

  double heading = 0.0;
  Vector2d pos(0.0, 0.0);
  const Vector2d dest(2.0, 0.0);

  size_t i = 0;
  double d = distToTicks((dest-pos).norm());
  double heading_diff = radToTicks(targetHeading(dest, pos));
  const int control = clamp(pid.process(d, 0), 0, 20);
  left.setTarget(control);
  right.setTarget(control);

  done = false;

  while (!done) {
    const int lv = left.velocity();
    const int rv = right.velocity();
    if (std::abs(d) < 100 && std::abs(heading_diff) < 10) {
      qDebug() << "--------------- " << d << " -------------------";
      done = true;
      continue;
    }
    const double target_heading = targetHeading(dest, pos);
    const double cv = static_cast<double>(lv + rv) / 2.0;
    heading += ticksToAngle(lv - rv);
    const double dist = ticksToDist(cv);
    pos += Vector2d(dist * cos(heading), dist * sin(heading));
    {
      QMutexLocker lock(&mutex);
      queue.push_back(std::make_pair(pos.cast<float>(), heading));
    }

    ++i;
    {
      
      heading_diff = radToTicks(heading - targetHeading(dest, pos));
      if (std::abs(heading_diff) > 10) {
        const int control = clamp(pid.process(heading_diff, dt), -10, 10);
        left.setTarget(control);
        right.setTarget(-control);
        qDebug() << "Heading correction: " << control << ", " << heading << ", " << targetHeading(dest, pos);
      } else {
        d = distToTicks((dest - pos).norm());
        const int control = clamp(pid.process(d, dt), 0, 20);
        left.setTarget(control);
        right.setTarget(control);
      }
      //qDebug() << pos.x() << ", " << pos.y();
      //qDebug() << left.target() << ", " << control << ", " << d << ", (" << pos.transpose() << "), (" << dest.transpose() << ")";
    }
    msleep(25);
  }
}

void WorkThread::stop() {
	done = true;
	wait();
}


#if 0
WorkThread::WorkThread(QObject* parent) :QThread(parent) {
  start();
  connect(this, SIGNAL(finished()), SLOT(deleteLater()));
}

void WorkThread::run() {
  int count = 1;
  while (isRunning()) {
    const ImuData imu_data = { Eigen::Vector3i(count, count + 1, count + 2),
                               Eigen::Vector3i(count + 3, count + 4, count + 5),
                               Eigen::Vector3i(count + 6, count + 7, count + 8) };
    QVariant var_data;
    var_data.setValue(imu_data);
    emit sensorData(var_data);
    msleep(200);
    count += 9;
  }
}
#endif
