#include <QSerialPort>
#include <queue>
#include <QDebug>

/*
	 Kp = 24000, Ki = 300000, Kd = 1, T = 10ms.
   Kp = 600, Ki = 110000, Kd = 1, T = 25ms.
*/

char buffer[2048];

struct MotionCmd {
	MotionCmd() : MotionCmd(0, 0, 0) {};
	MotionCmd(const int lv, const int rv, const int tick)
		: left_v(lv),
		right_v(rv),
		ticks(tick) {};
	int left_v;
	int right_v;
	int ticks;
};

void read(QSerialPort& port) {
	if (!port.waitForReadyRead(2000)) {
		qDebug() << "Serial port read_timeout: " << port.error() << "\n";
		buffer[0] = '\0';
		return;
	}
	const qint64 bytes_read = port.read(buffer, 2048);
	buffer[bytes_read] = '\0';
}

void setGains(QSerialPort &port, const int kp, const int ki, const int kd, const int kt) {
	sprintf(buffer, "k %ld, %ld, %ld, %d\r\n", kp, ki, kd, kt);
	port.write(buffer);
	port.waitForBytesWritten();
	read(port);
}
void setSetpoints(QSerialPort& port, const MotionCmd &cmd) {
	sprintf(buffer, "t %d, %d, %d\r\n", cmd.left_v, cmd.right_v, cmd.ticks);
	port.write(buffer);
	port.waitForBytesWritten();
	read(port);
}

int main(const int argc, const char** argv) {
  const int kP = 600;
  const int kI = 110000;
  const int kD = 100;
  const int kT = 25;
  bool done = false;
  QSerialPort serial_port;
  serial_port.setPortName("COM5");
  serial_port.setBaudRate(QSerialPort::Baud115200);
  serial_port.setDataBits(QSerialPort::Data8);
  serial_port.setFlowControl(QSerialPort::NoFlowControl);
  serial_port.setParity(QSerialPort::NoParity);
  serial_port.open(QIODevice::ReadWrite);
  if (!serial_port.isOpen()) {
	  qDebug() << "Failed to open serial port: " << serial_port.portName();
	  return -1;
  }
  serial_port.clear();
  setGains(serial_port, kP, kI, kD, kT);
  std::queue<MotionCmd> commands;
  commands.emplace(18, 18, 80);
  commands.emplace(-18, 18, 40);
  commands.emplace(18, 18, 80);
  commands.emplace(18, -18, 40);
  setSetpoints(serial_port, commands.front());
  commands.pop();
  while (!done) {
	  //check status and display
	  read(serial_port);
	  unsigned tick_count;
	  unsigned duration_in_ticks;
	  int16_t left_v, right_v;
	  int16_t left_pwm, right_pwm;
	  unsigned long current_time;
	  int queue_size;
	  qDebug() << buffer;
	  sscanf(buffer, "s %hd, %hd, %hd, %hd, %lu, %d, %d, %d\r\n", &left_v, &right_v, &left_pwm, &right_pwm, &current_time, &duration_in_ticks, &tick_count, &queue_size);
	  if (tick_count % 10 == 0) {
		  serial_port.write("s");
	  }
	  //if queue is not full
	  if (queue_size <= 4 && !commands.empty()) {
		  //do sending things
		  auto cmd = commands.front();
		  commands.pop();
		  setSetpoints(serial_port, cmd);
	  }
	  //while queue size>=0 AND commands have not yet finished
	  if (queue_size == 0 && tick_count == duration_in_ticks) {
		  done = true;
	  }
  }
  return 0;
}