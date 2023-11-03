#include <queue>
#include <QDebug>
#include <fstream>
#include <string>
#include "romi_robot.h"

 std::queue<MotionCmd> parse(const char *file_path){
	std::ifstream ifstream(file_path);
	std::queue<MotionCmd> queue;
  	if (!ifstream) {
		qDebug() << "File nonexistent";
		exit(-1);
  	}
	while (!ifstream.eof()) {
		MotionCmd cmd;
		std::string left_v, right_v, ticks;
		ifstream >> left_v >> right_v >> ticks;
		cmd.left_v = stoi(left_v);
		cmd.right_v = stoi(right_v);
		cmd.ticks = stoi(ticks);
		queue.push(cmd);
	}
	return queue;
}

int main(const int argc, const char** argv) {
  const int kP = 600;
  const int kI = 110000;
  const int kD = 100;
  const int kT = 25;
  if (argc != 3) {
	qDebug() << "Usage: velocity_pid_test [PORT] [FILE_PATH]";
	return -1;
  }
  RomiRobot robot(argv[1]);
  if (robot.isReady()) {
	robot.setGains(kP, kI, kD, kT);
  } else {
	qDebug() << "Failed to connect...";
	return -1;
  }
  qDebug() << "Battery voltage (millivolts) -> " << robot.getBatVoltage() << "\n";
  std::queue<MotionCmd> commands = parse(argv[2]);
  robot.setSetpoints(commands.front());
  commands.pop();
  bool done = false;
  while (!done) {
	const status_t status = robot.readStatus();
	if (status.tick_count % 10 == 0) {
		robot.sendHeartbeat();
	}
	if (status.queue_size < 4 && !commands.empty()) {
		auto cmd = commands.front();
		commands.pop();
		robot.setSetpoints(cmd);
	}
	if (status.queue_size == 0 && status.tick_count == status.duration_in_ticks) {
		done = true;
	}
  }
  return 0;
}