#include "work_thread.h"
#include "romi_robot.h"
#include <QDebug>
#include <QMutexLocker>

WorkThread::WorkThread(const QString& port_name, QObject* parent) : QThread(parent), robot(port_name) {}

void WorkThread::run() {
	robot.setGains(600, 110000, 100, 25);
	QPair<double, double> item;
	auto prev = std::chrono::high_resolution_clock::now();
	auto prev_two = std::chrono::high_resolution_clock::now();
	robot.setSetpoints(MotionCmd(0,0,10));
	while (running) {
		//qDebug() << "Here..";
		auto curr = std::chrono::high_resolution_clock::now();
		auto timediff = std::chrono::duration_cast<std::chrono::milliseconds>(curr - prev);
		//qDebug() << "Here2..";
		if(timediff.count() > 250){
			robot.sendHeartbeat();
			prev = curr;
		}
		//qDebug() << "Here3..";
		auto status = robot.readStatus();
		//qDebug() << "Robot status -> " << status.queue_size << queue.size();
		mutex.lock();/*
		if (queue.empty()) {
			cond_wait.wait(&mutex);
			continue;
		}*/
		auto curr_two = std::chrono::high_resolution_clock::now();
		auto timediff_two = std::chrono::duration_cast<std::chrono::milliseconds>(curr_two - prev_two);
		if(status.queue_size < 4 && !queue.empty() && timediff_two.count() > 30){
			item = queue.dequeue();
			int left_rpm = item.first * max_velocity + item.second * max_yaw_velocity;
			int right_rpm = item.first * max_velocity - item.second * max_yaw_velocity;
			//qDebug() << "Running setSetpoints...";
			robot.setSetpoints(MotionCmd(left_rpm, right_rpm, 1));
			//qDebug() << "Done running setSetpoints...";
			prev_two = curr_two;
		}
		mutex.unlock();
	}
}

void WorkThread::sendCommand(const double throttle, const double yaw_strength) {
	{
		const QMutexLocker locker(&mutex);
		queue.append(QPair<double, double>(throttle, yaw_strength));
	}
	//cond_wait.wakeAll();
}

void WorkThread::stop() {
	running = false;
	//cond_wait.wakeAll();
}