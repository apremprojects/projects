#include <QCoreApplication>
#include "work_thread.h"

#include <QtGamepad/QGamepadManager>
#include <QtGamepad/QGamepad>

#include <cmath>
#include <iostream>

int main(int argc, char** argv) {
	QCoreApplication app(argc, argv);

  do {
    app.processEvents();
    std::cout << "\n\rWaiting for Gamepad ...";
  } while (QGamepadManager::instance()->connectedGamepads().isEmpty());
  int device_id = QGamepadManager::instance()->connectedGamepads()[0];
  std::cout << "\n\rGamepad found: " << device_id << "\n";

	WorkThread work_thread("/dev/ttyACM0", &app);
	work_thread.start();

  auto gamepad = new QGamepad(device_id, &app);
  auto prev = std::chrono::high_resolution_clock::now();
  while (gamepad->isConnected()) {
    auto curr = std::chrono::high_resolution_clock::now();
    const auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(curr - prev);
    if (time_diff.count() >= 50) {
      if (gamepad->buttonL1() || gamepad->buttonR1()) {
        const int pwm = static_cast<int>(std::ceil(-75.0 * gamepad->axisLeftY()));
        work_thread.sendCommand(pwm, -pwm);
      } else {
        const int pwm = static_cast<int>(std::ceil(-300.0 * gamepad->axisLeftY()));
        work_thread.sendCommand(pwm, pwm);
      }
      prev = curr;
    }
    app.processEvents();
  }
  work_thread.stop();
  return 0;
}
