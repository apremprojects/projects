#include "main_dlg.h"
#include <QDebug>
#include <QIntValidator>

#define M_PI (4.0f*std::atan(1.0f))

MainDlg::MainDlg()
  : QDialog(nullptr, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint),
    work_params({"COM3", 25, 0}),
    acc_data(60 * 40),
    gyro_data(60 * 40) {
  mag_data.reserve(5 * 60 * 40);
  setupUi(this);
  work_thread = new WorkThread(this, mutex, queue, work_params);
  gyro_widget->setParams({ 48 * 40, 320, 0.0, 60, 24, -8 * M_PI, 8 * M_PI, 16, 25, "Gyro Data"});
  acc_widget->setParams({ 48 * 40, 320, 0.0, 60, 24, -16.0f * g, 16.0f * g, 16, 25, "Accelerometer Data"});
  edit_duration->setValidator(new QIntValidator(0, 60 * 60, this));
  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
  connect(angle_x, SIGNAL(valueChanged(int)), mag_widget, SLOT(angleXChanged(int)));
  connect(angle_y, SIGNAL(valueChanged(int)), mag_widget, SLOT(angleYChanged(int)));
  connect(angle_z, SIGNAL(valueChanged(int)), mag_widget, SLOT(angleZChanged(int)));
  connect(shift_center, SIGNAL(stateChanged(int)), mag_widget, SLOT(shiftCenter(int)));
}

void MainDlg::buttonClicked() {
  work_params.port_name = edit_comm->text();
  work_params.duration_sec = edit_duration->text().toInt();
  if (button->text() == "Start") {
    timer.start(25);
    work_thread->start(QThread::HighestPriority);
  } else {
    work_thread->stop();
    timer.stop();
  }
  controls->setDisabled(work_thread->isRunning());
  button->setText(work_thread->isRunning() ? "Stop" : "Start");
}

void MainDlg::timeout() {
    {
        QMutexLocker lock(&mutex);
        while (!queue.empty()) {
            const auto data = queue.front();
            queue.pop_front();
            acc_data.x.push_back(data.acc.x());
            acc_data.y.push_back(data.acc.y());
            acc_data.z.push_back(data.acc.z());
            acc_data.t.push_back(data.timestamp);
            gyro_data.x.push_back(data.gyro.x());
            gyro_data.y.push_back(data.gyro.y());
            gyro_data.z.push_back(data.gyro.z());
            gyro_data.t.push_back(data.timestamp);
            mag_data.push_back(data.mag);
        }
    }
    acc_widget->setData(acc_data);
    gyro_widget->setData(gyro_data);
    mag_widget->setData(mag_data);
    acc_data.clear();
    gyro_data.clear();
    mag_data.clear();
    controls->setDisabled(work_thread->isRunning());
    button->setText(work_thread->isRunning() ? "Stop" : "Start");
}
