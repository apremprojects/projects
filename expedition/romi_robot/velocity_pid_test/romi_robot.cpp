#include "romi_robot.h"
#include <QSerialPort>
#include <queue>
#include <QDebug>

RomiRobot::RomiRobot(const QString port_name) {
    port.setPortName(port_name);
    port.setBaudRate(QSerialPort::Baud115200);
    port.setDataBits(QSerialPort::Data8);
    port.setFlowControl(QSerialPort::NoFlowControl);
    port.setParity(QSerialPort::NoParity);
    port.open(QIODevice::ReadWrite);
    if (!port.isOpen()) {
        qDebug() << "Failed to open serial port: " << port.portName();
        ready = false;
    }
    port.clear();
}

void RomiRobot::read() {
    if (!port.waitForReadyRead(30000)) {
        qDebug() << "Serial port read_timeout: " << port.error() << "\n";
        buffer[0] = '\0';
        ready = false;
    }
    const qint64 bytes_read = port.read(buffer, 2048);
    buffer[bytes_read] = '\0';
    //qDebug() << buffer;
}

void RomiRobot::setGains(const int kp, const int ki, const int kd, const int kt) {
    sprintf(buffer, "k %ld, %ld, %ld, %d\r\n", kp, ki, kd, kt);
    port.write(buffer);
    port.waitForBytesWritten();
    read();
}

void RomiRobot::setSetpoints(const MotionCmd &cmd) {
    sprintf(buffer, "t %d, %d, %d\r\n", cmd.left_v, cmd.right_v, cmd.ticks);
    port.write(buffer);
    port.waitForBytesWritten();
    read();
}

status_t RomiRobot::readStatus() {
    read();
    //qDebug() << buffer;
	status_t status;
    sscanf(buffer, "s %hd, %hd, %hd, %hd, %lu, %d, %d, %d\r\n", 
        &status.left_v, &status.right_v, 
        &status.left_pwm, &status.right_pwm, 
        &status.current_time, &status.duration_in_ticks, 
        &status.tick_count, &status.queue_size);
    return status;
}

bool RomiRobot::isReady() {
    return ready;
}

void RomiRobot::sendHeartbeat() {
    port.write("s");
}

uint16_t RomiRobot::getBatVoltage() {
    port.write("b");
    read();
    uint16_t val;
    sscanf(buffer, "b: %hu\r\n", &val);
    return val;
}