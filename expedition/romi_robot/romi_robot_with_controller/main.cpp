#include <QCoreApplication>
#include <QDebug>
#include <QtGamepad/QGamepadManager>
#include <QtGamepad/QGamepad>
#include "work_thread.h"
#include <cmath>

int main(int argc, char** argv) {
	QCoreApplication app(argc, argv);

  while (QGamepadManager::instance()->connectedGamepads().isEmpty()) {
    app.processEvents();
    qDebug() << "\n\rWaiting for Gamepad ...";
  }
  int device_id = QGamepadManager::instance()->connectedGamepads()[0];
  qDebug() << "\n\rGamepad found: " << device_id << "\n";

	WorkThread work_thread("/dev/ttyACM0", &app);
	work_thread.start();

  auto gamepad = new QGamepad(device_id, &app);
  auto prev = std::chrono::high_resolution_clock::now();
  while (gamepad->isConnected()) {
    auto curr = std::chrono::high_resolution_clock::now();
    const auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(curr - prev);
    if (time_diff.count() >= 50) {
      const double throttle = -gamepad->axisLeftY();
      const double yaw = gamepad->axisLeftX();
      work_thread.sendCommand(throttle, yaw);
      prev = curr;
    }
    app.processEvents();
  }
  work_thread.stop();
  return 0;
}
