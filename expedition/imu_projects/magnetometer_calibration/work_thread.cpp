#include "work_thread.h"

#include <QDebug>
#include <QSerialPort>

#include <cmath>

WorkThread::WorkThread(QObject* parent, QMutex& mutex, QQueue<MagnetometerData>& queue, const WorkParams& params)
	: QThread(parent),
	  mutex(mutex),
	  queue(queue),
    params(params) {}

WorkThread::~WorkThread() {
	stop();
}

void WorkThread::run() {
	done = false;
	char buffer[2048];
	QSerialPort serial_port;
	serial_port.setPortName(params.port_name);
	serial_port.setBaudRate(QSerialPort::Baud115200);
	serial_port.setDataBits(QSerialPort::Data8);
	serial_port.setFlowControl(QSerialPort::NoFlowControl);
	serial_port.setParity(QSerialPort::NoParity);
	serial_port.open(QIODevice::ReadWrite);
	if (!serial_port.isOpen()) {
		qDebug() << "Failed to open serial port: " << serial_port.portName();
		return;
	}
	serial_port.clear();

	size_t count = 0;
	Eigen::Vector3i acc;
	Eigen::Vector3i mag;
	Eigen::Vector3i gyro;
	unsigned long int current_time;
	float pressure, altitude, temperature;
	while (!done) {
		if ((count % 10) == 0) {
			serial_port.write("s");
			if (!serial_port.waitForBytesWritten()) {
				qDebug() << "Serial port write timeout: " << serial_port.error() << "\n";
				done = true;
				continue;
			}
		}
		++count;
		read(serial_port, buffer);
		sscanf(buffer, "s: %d, %d, %d, %d, %d, %d, %d, %d, %d, %lu\r\n",
			&acc.x(), &acc.y(), &acc.z(),
			&gyro.x(), &gyro.y(), &gyro.z(),
			&mag.x(), &mag.y(), &mag.z(), &current_time);
		{
			QMutexLocker locker(&mutex);
			MagnetometerData data{
				mag.cast<float>() * static_cast<float>(4.0 / 32768.0)
			};
			queue.push_back(data);
		}
	}
	{
		QMutexLocker lock(&mutex);
		queue.clear();
	}
}

void WorkThread::read(QSerialPort& port, char* buffer) {
	if (!port.waitForReadyRead(1000)) {
		qDebug() << "Serial port read_timeout: " << port.error() << "\n";
		done = true;
	}
	const qint64 bytes_read = port.read(buffer, 2048);
	buffer[bytes_read] = '\0';
}

void WorkThread::stop() {
	done = true;
	wait();
}
