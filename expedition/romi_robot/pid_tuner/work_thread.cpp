#include "work_thread.h"
#include <QDebug>
#include <QSerialPort>
#include <cmath>

#define M_PI (4.0f*std::atan(1.0f))

WorkThread::WorkThread(QObject* parent, QMutex& mutex, QQueue<WheelData>& queue, const WorkParams& params)
	: QThread(parent),
	  mutex(mutex),
	  queue(queue),
    params(params) {}

WorkThread::~WorkThread() {
	stop();
}

void WorkThread::run() {
	done = false;
	serial_port = new QSerialPort;
	serial_port->setPortName(params.port_name);
	serial_port->setBaudRate(QSerialPort::Baud115200);
	serial_port->setDataBits(QSerialPort::Data8);
	serial_port->setFlowControl(QSerialPort::NoFlowControl);
	serial_port->setParity(QSerialPort::NoParity);
	serial_port->open(QIODevice::ReadWrite);
	if (!serial_port->isOpen()) {
		qDebug() << "Failed to open serial port: " << serial_port->portName();
		return;
	}
	serial_port->clear();
	setGains(params.kp, params.ki, params.kd, params.sampling_time_ms);
	setSetpoints(params.left_sp, params.right_sp);
	while (!done) {
		/*if (params.sp_changed) {
			setSetpoints(params.left_sp, params.right_sp);
		}*/
		read();
		unsigned tick_count;
		int16_t left_v, right_v;
		int16_t left_pwm, right_pwm;
		unsigned long current_time;
		int duration_in_ticks;
		int queue_size;
		sscanf(buffer, "s %hd, %hd, %hd, %hd, %lu, %d, %d, %d\r\n", 
        &left_v, &right_v, 
        &left_pwm, &right_pwm, 
        &current_time, &duration_in_ticks, 
        &tick_count, &queue_size);
		qDebug() << buffer;
		//sscanf(buffer, "s %hd, %hd, %hd, %hd, %lu, %u\r\n", &left_v, &right_v, &left_pwm, &right_pwm, &current_time, &tick_count);
		if(queue_size < 4){
			setSetpoints(params.left_sp, params.right_sp);
		}
		WheelData wheel = { left_v, right_v, params.left_sp, params.right_sp, left_pwm, right_pwm, static_cast<float>(current_time) / 1000.0f };
		{
			QMutexLocker locker(&mutex);
			queue.push_back(wheel);
		}
		if ((tick_count % 10) == 0) {
			serial_port->write("s");
			serial_port->waitForBytesWritten();
		}
		if (tick_count > params.duration_sec * 1000 / params.sampling_time_ms) {
			done = true;
		}
	}
	setGains(0, 0, 0, params.sampling_time_ms);
	delete serial_port;
}

void WorkThread::read() {
	if (!serial_port->waitForReadyRead(1500)) {
		qDebug() << "Serial port read_timeout: " << serial_port->error() << "\n";
		done = true;
		buffer[0] = '\0';
		return;
	}
	const qint64 bytes_read = serial_port->read(buffer, 2048);
	buffer[bytes_read] = '\0';
}

void WorkThread::setGains(const int kp, const int ki, const int kd, const int period_ms) {
	sprintf(buffer, "k %ld, %ld, %ld, %d\r\n", kp, ki, kd, params.sampling_time_ms);
	serial_port->write(buffer);
	serial_port->waitForBytesWritten();
	read();
}
void WorkThread::setSetpoints(const int left_wheel, const int right_wheel) {
	sprintf(buffer, "t %d, %d, %d\r\n", left_wheel, right_wheel, 5);
	serial_port->write(buffer);
	serial_port->waitForBytesWritten();
	read();
}

void WorkThread::stop() {
	done = true;

	wait();
}
