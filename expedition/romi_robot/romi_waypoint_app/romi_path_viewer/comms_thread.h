#pragma once

#include <QQueue>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include <QTcpSocket>

#include <Eigen/Core>

class CommsThread : public QThread {
	Q_OBJECT
public:
	CommsThread(QObject* parent, const QString& host_name, QMutex& mutex, QQueue<Eigen::Vector3d>& queue);
	~CommsThread();

	void stop();
	void reset(const int x, const int y, const int theta);

	void sendWayPoint(const int x, const int y);

private:
	void run() override;

	bool done = true;

	const QString host_name;

	QQueue<Eigen::Vector2i> way_points;
	QQueue<Eigen::Vector3i> reset_commands;

	QMutex& mutex;
	QQueue<Eigen::Vector3d>& queue;

	QTcpSocket* tcp_socket;
};
