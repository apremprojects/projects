#pragma once

#include <QThread>
#include <QQueue>
#include <QPair>
#include <QMutex>
#include <QWaitCondition>

#include "./../romi_robot/romi_robot.h"

class WorkThread : public QThread {
	Q_OBJECT
public:
	WorkThread(const QString& port_name, QObject* parent);

	void sendCommand(const int left, const int right);

	void stop();

private:
	void run() override;

	bool running = true;
	const QString port_name;

	QMutex mutex;
	QWaitCondition cond_wait;
	QQueue<QPair<int, int>> queue;
};