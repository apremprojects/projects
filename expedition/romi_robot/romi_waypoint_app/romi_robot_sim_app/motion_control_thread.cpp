#include "motion_control_thread.h"

#include <QDebug>

#include <Eigen/Core>
#include <cmath>
#include <random>

using Eigen::Vector2d;
using Eigen::Vector3d;

class PidControl {
public: 
  PidControl(const float kp, const float ki, const float kd) :Kp(kp), Ki(ki), Kd(kd) {};
  void setTarget(const float t) { 
    target = t; 
  }

  float process(const float obs, const float dt, const float min_range, const float max_range) {
    const float err = target-obs; 
    const float der_err = (err - prev_err) / dt;
    prev_err = err; 
    err_integral += (err * dt); 
    float out = (Kp * err) + (Ki * err_integral) + (Kd * der_err);
    if (out < min_range) {
      out = min_range;
      err_integral -= (err*dt);
    } else if (out > max_range) {
      out = max_range;
      err_integral -= (err*dt);
    }
    return -out; 
  }

  float integralError() {
    return err_integral;
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
    //dist = std::normal_distribution<double>(static_cast<double>(t), 3.0f);
    dist = std::uniform_real_distribution<double>(v-5.0, v+2.0);
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
  //std::normal_distribution<double> dist;
  std::uniform_real_distribution<double> dist;
};

int clamp(const float val, const float min_val, const float max_val) {
  return std::min(std::max(val, min_val), max_val); 
}

MotionControlThread::MotionControlThread(QObject* parent, QMutex& mutex, QQueue<Eigen::Vector3d>& queue)
   : QThread(parent),
	heading(0.0),
  pos(0.0, 0.0),
  mutex(mutex),
	queue(queue) {}

MotionControlThread::~MotionControlThread() {
	stop();
}

void MotionControlThread::run() {
  const double dt = 0.025;

  const auto ticksToDist = [](const double enc_ticks) {
    return (0.07 * 4.0 * std::atan(1.0) / 1440.0) * enc_ticks;
  };
  const auto distToTicks = [](const double dist) {
    return std::round((1440.0 * dist) / (0.07 * 4.0 * std::atan(1.0)));
  };
  const auto ticksToAngle = [&ticksToDist](const double enc_ticks) {
    return ticksToDist(enc_ticks) / 0.07;
  };
  const auto radToDegree = [](const double theta) {
    return (180.0 * theta) / (4.0 * std::atan(1.0));
  };
  const auto radToTicks = [&distToTicks](double theta_rad) {
    return distToTicks(0.07*theta_rad);
  };
  const auto headingDiff = [](const double& heading, const Vector2d& dest_dir) {
    if (dest_dir.x() == 0.0 && dest_dir.y() == 0.0) {
      return 0.0;
    }
    const Vector2d head_dir(cos(heading), sin(heading));
    const double cross_val = head_dir.x()*dest_dir.y() - dest_dir.x()*head_dir.y();
    const double angle = std::acos(head_dir.dot(dest_dir)/dest_dir.norm());
    return (cross_val < 0.0)?(-angle):angle;
  };

  Wheel left;
  Wheel right;

  PidControl dist_pid(0.5f, 0, 0); //1000.0f, 0.01f);
  PidControl heading_pid(0.5f, 5.0f, 0.0f);
  dist_pid.setTarget(0.0f);
  heading_pid.setTarget(0.0f);

  Vector2d dest(0.0, 0.0);

  done = false;
  bool is_oriented = false;

  while (!done) {
    const double d = distToTicks((dest - pos).norm());
    const double heading_diff = radToTicks(headingDiff(heading, (dest-pos)));

    float left_v, right_v, correction;
    if (!is_oriented) {
      if (std::abs(heading_diff) < 8.0) {
        is_oriented = true;
      } else {
        correction = heading_pid.process(heading_diff, dt, -18.0, 18.0);
        left_v = correction;
        right_v = -correction;
      }
    }

    if (is_oriented) {
      const int velocity = dist_pid.process(d, dt, -15.0, 15.0);
      correction = heading_pid.process(heading_diff, dt, -3.0, 3.0);
      left_v = clamp(velocity + correction, -18.0, 18.0);
      right_v = clamp(velocity - correction, -18.0, 18.0);
    }
    //qDebug() << "Heading: " << correction << ", left: " << left_v << ", right: " << right_v;

    left.setTarget(left_v);
    right.setTarget(right_v);

    if (std::abs(d) < 25) {
      qDebug() << "--------------- " << d << " -------------------";
      QMutexLocker lock(&cond_mutex);
      if (way_points.empty()) {
        cond_wait.wait(&cond_mutex);
        continue;
      } else {
        dest = way_points.front();
        way_points.pop_front();
        qDebug() << "dest: " << dest.x() << ", " << dest.y();
        is_oriented = false;
      }
    }

    const int lv = left.velocity();
    const int rv = right.velocity();
    const double cv = static_cast<double>(lv + rv) / 2.0;
    heading += ticksToAngle(lv - rv);
    const double dist = ticksToDist(cv);
    {
      QMutexLocker lock(&mutex);
      pos += Vector2d(dist * cos(heading), dist * sin(heading));
      queue.push_back(Vector3d(pos.x(), pos.y(), heading));
    }
    msleep(25);
  }
}

void MotionControlThread::stop() {
	done = true;
  cond_wait.notify_all();
	wait();
}

void MotionControlThread::addWayPoints(const double x, const double y) {
  {
    QMutexLocker lock(&cond_mutex);
    way_points.push_back(Eigen::Vector2d(x, y));
  }
  cond_wait.notify_all();
}

Eigen::Vector3d MotionControlThread::state() {
  Eigen::Vector3d head;
  {
    QMutexLocker lock(&mutex);
    head = Vector3d(pos.x(), pos.y(), heading);
  }
  return head;
}
