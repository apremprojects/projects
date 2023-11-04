#pragma once

#include <QThread>
#include <QQueue>
#include <QPair>
#include <QMutex>
#include <QWaitCondition>
#include "romi_robot.h"

class WorkThread : public QThread {
	Q_OBJECT
public:
	WorkThread(const QString& port_name, QObject* parent);

	void sendCommand(const double throttle, const double yaw_strength);

	void stop();

private:
	void run() override;
	bool running = true;
	RomiRobot robot;
	const double max_velocity = 90;
	const double max_yaw_velocity = 90;
	QMutex mutex;
	QWaitCondition cond_wait;
	QQueue<QPair<double, double>> queue;
};