#include "motion_control_thread.h"

#include <QDebug>
#include <QByteArray>
#include <QSerialPort>

#include <Eigen/Core>
#include <cmath>
#include <random>

using Eigen::Vector2i;
using Eigen::Vector2d;
using Eigen::Vector3i;
using Eigen::Vector3d;

MotionControlThread::MotionControlThread(QObject* parent, QMutex& mutex, QQueue<Eigen::Vector3d>& queue)
	: QThread(parent),
	heading(0.0),
	pos(0.0, 0.0),
	mutex(mutex),
	queue(queue),
	romi("COM5") {}

MotionControlThread::~MotionControlThread() {
	stop();
}

void MotionControlThread::run() {
	qDebug() << "run()";
	//RomiRobot romi("COM8");

	const RomiRobot::Gain gains_v = { 600, 110000, -1 };
	const RomiRobot::Gain gains_d = { 500, 100000, -1 };
	const RomiRobot::Gain gains_y = { 500, 100000, -1 };

	romi.setGains(gains_v, gains_d, gains_y);

	RomiRobot::State state = romi.getCurrentState();
	{
		QMutexLocker lock(&mutex);
		queue.push_back(Vector3d(state.x, state.y, state.yaw));
	}

	Vector2i dest(state.x * 1000, state.y * 1000);

	done = false;

	while (!done) {
		{
			QMutexLocker lock(&cond_mutex);
			if (!way_points.empty()) {
				if (!state.is_wps_q_full) {
					dest = way_points.front();
					way_points.pop_front();
					romi.setWayPoint(dest.x(), dest.y());
					qDebug() << "---- Here: " << dest.x() << ", " << dest.y();
				}
			}
			if (!reset_commands.isEmpty()) {
				Eigen::Vector3i origin = reset_commands.front();
				reset_commands.pop_front();
				romi.reset(origin.x(), origin.y(), origin.z());
			}
		}

		state = romi.getCurrentState();
		{
			QMutexLocker lock(&mutex);
			queue.push_back({ state.x, state.y, state.yaw });
		}
		msleep(100);
	}
}

void MotionControlThread::stop() {
	done = true;
	cond_wait.notify_all();
	wait();
}

void MotionControlThread::addWayPoints(const int x, const int y) {
	qDebug() << "addWayPoints() -> " << x << ", " << y;
	{
		QMutexLocker lock(&cond_mutex);
		way_points.push_back({ x, y });
	}
	cond_wait.notify_all();
}

void MotionControlThread::reset(const int x, const int y, const int theta) {
	{
		QMutexLocker lock(&cond_mutex);
		way_points.clear();
		way_points.push_back({ x, y });
		reset_commands.push_back({x, y, theta });
	}
}

Eigen::Vector3d MotionControlThread::state() {
	qDebug() << "state()";
	Eigen::Vector3d head;
	{
		QMutexLocker lock(&mutex);
		head = Vector3d(pos.x(), pos.y(), heading);
	}
	return head;
}
