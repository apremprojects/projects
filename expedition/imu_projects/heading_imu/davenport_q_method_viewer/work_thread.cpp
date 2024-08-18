#include "work_thread.h"

#include <random>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/EigenValues>

#include <QDebug>
#include <QSerialPort>

#include <iostream>

using Eigen::Vector3i;
using Eigen::Vector3f;
using Eigen::Vector3d;
using Eigen::Matrix3f;
using Eigen::Matrix4f;
using Eigen::AngleAxisf;
using Eigen::Quaternionf;
using Eigen::EigenSolver;

#define TO_RAD(theta_deg) ((std::atan(1.0f) / 45.0f) * theta_deg)

static const float mag_dec_angle = TO_RAD(12.0f + (58.0f / 60.0f));
static const float mag_inc_angle = -TO_RAD(60.0f + (53.0f / 60.0f));

static const Vector3f g_ned(0.0f, 0.0f, 1.0f);
static const Vector3f m_ned(cos(mag_inc_angle) * cos(mag_dec_angle), 
														cos(mag_inc_angle) * sin(mag_dec_angle), 
														sin(mag_inc_angle));

Vector3f gyroToFloat(const Vector3i& gyro) {
	return gyro.cast<float>()*TO_RAD(0.035f);
}

Vector3f accToFloat(const Vector3i& acc) {
	return acc.cast<float>()*(0.488f/1000.0f);
}

Vector3f magToFloat(const Vector3i& mag) {
	return mag.cast<float>()*(4.0f/32768.0f);
}

Quaternionf davenportQ(const Matrix3f& B) {
	const float sigma = B.trace();
	const Matrix3f S = B + B.transpose();
	const Vector3f z(
		B(1, 2) - B(2, 1),
		B(2, 0) - B(0, 2),
		B(0, 1) - B(1, 0)
	);
	Matrix4f K;
	//K.block<3, 3>(0, 1) = z.transpose;
	K(0,0) = sigma;
	K.block<1,3>(0, 1) = z.transpose();
	
	K.block<3,1>(1, 0) = z;

	K.block<3, 3>(1, 1) = S - (sigma * Matrix3f::Identity());

	const EigenSolver<Matrix4f> sv(K);
	Matrix4f::Index ind;

	const auto eigenvalues = sv.eigenvalues().real().maxCoeff(&ind);
	const auto v_q = sv.eigenvectors().real().col(ind);
	
	Quaternionf q(v_q(0), v_q(1), v_q(2), v_q(3));
	q.normalize();

	return q;
}
  
WorkThread::WorkThread(QObject* parent, QMutex& mutex, QQueue<Quaternionf>& queue, const QString& port_name)
  : QThread(parent),
    mutex(mutex),
    queue(queue),
		port_name(port_name) {}

WorkThread::~WorkThread() {
  stop();
}

void WorkThread::stop() {
  done = true;
  wait();
}

void WorkThread::run() {
  done = false;
  
  QSerialPort serial_port;
	serial_port.setPortName(port_name);
	serial_port.setBaudRate(QSerialPort::Baud115200);
	serial_port.setDataBits(QSerialPort::Data8);
	serial_port.setFlowControl(QSerialPort::NoFlowControl);
	serial_port.setParity(QSerialPort::NoParity);
	serial_port.open(QIODevice::ReadWrite);
	if (!serial_port.isOpen()) {
		qDebug() << "Failed to open serial port: " << serial_port.portName();
		done = true;
	}
	serial_port.clear();

	Matrix3f M;
	M << 2.80288f, 0.055417f, 0.0267958f,
		0.055417f, 2.90052f, -0.000561382f,
		0.0267958f, -0.000561382f, 2.78627f;
	const Vector3f b(-0.594979f, 0.270666f, -0.0379652f);

	size_t count = 0;
	char buffer[2048];

	Vector3i acc;
	Vector3i mag;
	Vector3i gyro;
	
	float f1, f2, f3;
	unsigned long int prev_time;
	unsigned long int current_time;
	const float w = 0.5;

	Matrix3f B = Matrix3f::Zero();

	while (!done) {
		if ((count % 10) == 0) {
			serial_port.write("s");
			if (!serial_port.waitForBytesWritten()) {
				qDebug() << "Serial port write timeout: " << serial_port.error();
				done = true;
				continue;
			}
		}
		++count;
		if (!serial_port.waitForReadyRead(1000)) {
			qDebug() << "Serial port read_timeout: " << serial_port.error();
			done = true;
			continue;
		}
		const qint64 bytes_read = serial_port.read(buffer, 2048);
		buffer[bytes_read] = '\0';
		sscanf(buffer, "s: %d, %d, %d, %d, %d, %d, %d, %d, %d, %f, %f, %f, %lu\r\n",
					 &acc.x(), &acc.y(), &acc.z(),
					 &gyro.x(), &gyro.y(), &gyro.z(),
					 &mag.x(), &mag.y(), &mag.z(),
					 &f1, &f2, &f3,
					 &current_time);

		const Vector3f acc_data = accToFloat(acc).normalized();
		const Vector3f mag_data = (M*(magToFloat(mag) - b)).normalized();
		B += (w * acc_data * g_ned.transpose() + (1 - w) * mag_data * m_ned.transpose());

		if ((count % 20) == 0) {
			
			QMutexLocker locker(&mutex);
			Quaternionf q = davenportQ(B);
			qDebug() << "here";
			queue.push_back(q);

			B = Matrix3f::Zero();
		}
	}
}
