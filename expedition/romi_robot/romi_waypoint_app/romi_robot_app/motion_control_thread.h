#pragma once

#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QWaitCondition>
#include <QSerialPort>
#include <QDebug>

#include <Eigen/Core>

class RomiRobot {
public:
	struct Gain {
		long int kp;
		long int ki;
		long int kd;
	};
	struct State {
		double x;
		double y;
		double yaw;
		bool is_idle;
		bool is_wps_q_full;
		double timestamp;
	};

	RomiRobot(const QString& port_name) {
		rx_buffer.reserve(1000);
		tx_buffer.reserve(1000);
		serial_port.setPortName(port_name);
		serial_port.setBaudRate(QSerialPort::Baud115200);
		serial_port.setDataBits(QSerialPort::Data8);
		serial_port.setFlowControl(QSerialPort::NoFlowControl);
		serial_port.setParity(QSerialPort::NoParity);
		serial_port.open(QIODevice::ReadWrite);
		if (!serial_port.isOpen()) {
			qDebug() << "Failed to open serial port: " << serial_port.portName() << "\n";
			exit(0);
		}
		serial_port.setDataTerminalReady(true);
	}

	void setGains(const Gain& gains_v, const Gain& gains_d, const Gain& gains_y) {
		qDebug() << "SetGains()";
		const int num_chars =
			sprintf(tx_buffer.data(),
				"k %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %d\r\n",
				gains_v.kp, gains_v.ki, gains_v.kd,
				gains_d.kp, gains_d.ki, gains_d.kd,
				gains_y.kp, gains_y.ki, gains_y.kd,
				25);
		if (num_chars < 0) {
			qDebug() << "SetGains Failed";
			exit(0);
		}
		serial_port.write(tx_buffer.data());
		serial_port.waitForBytesWritten();
		serial_port.waitForReadyRead(2000);
		const int bytes_read = serial_port.read(rx_buffer.data(), rx_buffer.capacity());
		rx_buffer[bytes_read] = '\0';
		qDebug() << bytes_read << " - " << rx_buffer.data();
	}

	void setWayPoint(const int x, const int y) {
		const int num_chars = sprintf(tx_buffer.data(), "w %d, %d\r\n", x, y);
		if (num_chars < 0) {
			qDebug() << "SetWayPoint Failed";
			exit(0);
		}
		serial_port.write(tx_buffer.data());
		serial_port.waitForBytesWritten();
		serial_port.waitForReadyRead(2000);
		const int bytes_read = serial_port.read(rx_buffer.data(), rx_buffer.capacity());
		rx_buffer[bytes_read] = '\0';
		qDebug() << bytes_read << " - " << rx_buffer.data();
	}

	State getCurrentState() {
		qDebug() << "getCurrentState()";
		State state;
		const int num_chars = sprintf(tx_buffer.data(), "s\r\n");
		if (num_chars < 0) {
			qDebug() << "SetWayPoint Failed";
			exit(0);
		}
		serial_port.write(tx_buffer.data(), 3);
		qDebug() << "HERE1\n";
		//serial_port.waitForBytesWritten();
		serial_port.waitForReadyRead(2000);
		qDebug() << "HERE2\n";
		const int bytes_read = serial_port.read(rx_buffer.data(), rx_buffer.capacity());
		qDebug() << bytes_read;
		rx_buffer[bytes_read] = '\0';
		unsigned long int timestamp;
		int is_idle, is_wps_q_full;
		sscanf(rx_buffer.data(), "s: %lf, %lf, %lf, %lu, %d, %d\r\n",
			&state.x, &state.y, &state.yaw,
			&timestamp, &is_wps_q_full, &is_idle);
		qDebug() << bytes_read << " - " << rx_buffer.data();
		state.timestamp = static_cast<double>(timestamp) / 1000.0;
		state.is_wps_q_full = (bool)is_wps_q_full;
		state.is_idle = (bool)is_idle;
		return state;
	}

	void reset(const int x, const int y, const int theta) {
		const int num_chars = sprintf(tx_buffer.data(), "r %d, %d, %d\r\n", x, y, theta);
		if (num_chars < 0) {
			qDebug() << "reset failed";
			exit(0);
		}
		qDebug() << "reset";
		serial_port.write(tx_buffer.data());
		serial_port.waitForBytesWritten();
	}

private:
	QByteArray rx_buffer;
	QByteArray tx_buffer;
	QSerialPort serial_port;
};

class MotionControlThread : public QThread {
	Q_OBJECT
public:
	MotionControlThread(QObject* parent, QMutex& mutex, QQueue<Eigen::Vector3d>& queue);
	~MotionControlThread();

	void stop();

	void addWayPoints(const int x, const int y);
	void reset(const int x, const int y, const int theta);

	Eigen::Vector3d state();

private:
	void run() override;

	bool done = false;

	double heading;
	Eigen::Vector2d pos;

	QMutex& mutex;
	QQueue<Eigen::Vector3d>& queue;

	QMutex cond_mutex;
	QWaitCondition cond_wait;
	QQueue<Eigen::Vector2i> way_points;
	QQueue<Eigen::Vector3i> reset_commands;

	RomiRobot romi;
};
