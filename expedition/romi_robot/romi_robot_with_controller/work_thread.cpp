#include "work_thread.h"
#include <QDebug>
#include <QMutexLocker>

WorkThread::WorkThread(const QString& port_name, QObject* parent)
	: QThread(parent),
    port_name(port_name) {}

void WorkThread::run() {
	RomiRobot robot(port_name, this);
	QPair<int, int> item;
	while (running) {
		{
			const QMutexLocker locker(&mutex);
			if (queue.empty()) {
				cond_wait.wait(&mutex);
				continue;
			}
			item = queue.dequeue();
			mutex.unlock();
		}
		const auto motor_state = robot.set_motors(item.first, item.second);
	}
}

void WorkThread::sendCommand(const int left, const int right) {
	{
		const QMutexLocker locker(&mutex);
		queue.append(QPair<int, int>(left, right));
	}
	cond_wait.wakeAll();
}

void WorkThread::stop() {
	running = false;
	cond_wait.wakeAll();
}