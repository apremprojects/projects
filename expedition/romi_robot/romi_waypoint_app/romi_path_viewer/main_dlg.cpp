#include "main_dlg.h"

#include <QDebug>

MainDlg::MainDlg()
  : QDialog(nullptr, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint) {
  setupUi(this);
  points.reserve(1000);
  path_view->setFocus();
  connect(connect_btn, SIGNAL(clicked()), this, SLOT(connectToRobot()));
  connect(reset_button, SIGNAL(clicked()), this, SLOT(reset()));
  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(path_view, SIGNAL(newWayPoint(const int, const int)), 
    this, SLOT(addWayPoints(const int, const int)));
  timer.setInterval(25);
  timer.start();
}

void MainDlg::timeout() {
  float yaw = 0.0f;
  {
    QMutexLocker lock(&mutex);
    points.resize(0);
    while (!queue.empty()) {
      const auto item = queue.front();
      yaw = static_cast<float>(item.z());
      points.emplace_back(Eigen::Vector2f(item.x(), item.y()));
      queue.pop_back();
    }
  }
  if (!points.empty()) {
    path_view->add(yaw, points);
  }
  
  if (comms_thread) {
    path_view->setEnabled(comms_thread->isRunning());
    host_ip->setDisabled(comms_thread->isRunning());
    connect_btn->setDisabled(comms_thread->isRunning());
    reset_button->setEnabled(comms_thread->isRunning());
  } else {
    path_view->setDisabled(true);
    reset_button->setDisabled(true);
  }
}

void MainDlg::addWayPoints(const int x, const int y) {
  if (comms_thread) {
    comms_thread->sendWayPoint(x, y);
  }
}

void MainDlg::connectToRobot() {
  comms_thread = new CommsThread(this, host_ip->text(), mutex, queue);
  comms_thread->start();
  host_ip->setDisabled(true);
  connect_btn->setDisabled(true);
  path_view->clear();
}

void MainDlg::reset() {
    comms_thread->reset(x_box->text().toInt(), y_box->text().toInt(), theta_box->text().toInt());
    path_view->clear();
}
