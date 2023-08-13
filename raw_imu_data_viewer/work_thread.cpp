#include "work_thread.h"

#include <QTime>
#include <cmath>
#include <iostream>

#define M_PI (4.0f*std::atan(1.0f))

WorkThread::WorkThread(const QString& port_name, QObject* parent)
	: QThread(parent),
    port_name(port_name) {}

void WorkThread::run() {
	char buffer[2048];
	QSerialPort serial_port;
	serial_port.setPortName(port_name);
	serial_port.setBaudRate(QSerialPort::Baud115200);
	serial_port.setDataBits(QSerialPort::Data8);
	serial_port.setFlowControl(QSerialPort::NoFlowControl);
	serial_port.setParity(QSerialPort::NoParity);
	serial_port.open(QIODevice::ReadWrite);
	if (!serial_port.isOpen()) {
		std::cout << "Failed to open serial port: " << serial_port.portName().toStdString() << "\n";
		emit readFailed();
		exit(0);
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
				std::cout << "Serial port write timeout: " << serial_port.error() << "\n";
				done = true;
				emit readFailed();
				continue;
			}
		}
		++count;
		if (!serial_port.waitForReadyRead(1000)) {
			std::cout << "Serial port read_timeout: " << serial_port.error() << "\n";
			done = true;
			emit readFailed();
			continue;
		}
		const qint64 bytes_read = serial_port.read(buffer, 2048);
		buffer[bytes_read] = '\0';
		sscanf(buffer, "s: %d, %d, %d, %d, %d, %d, %d, %d, %d, %f, %f, %f, %lu\r\n",
			&acc.x(), &acc.y(), &acc.z(),
			&gyro.x(), &gyro.y(), &gyro.z(),
			&mag.x(), &mag.y(), &mag.z(),
			&pressure, &altitude, &temperature, &current_time);
		{
			QMutexLocker locker(&mutex);
			imu_data.gyro = 0.035f * (M_PI / 180.0f) * gyro.cast<float>();
			imu_data.acc = 9.8 * 0.488 * acc.cast<float>() / 1000.0f;
			imu_data.mag = mag.cast<float>()*static_cast<float>(4.0/32768.0);
		}
	}
}

ImuData WorkThread::read() {
	QMutexLocker locker(&mutex);
	return imu_data;
}

void WorkThread::stop() {
	done = true;
	wait();
}