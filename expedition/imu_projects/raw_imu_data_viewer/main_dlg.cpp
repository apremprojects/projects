#include "main_dlg.h"

#include <QLabel>
#include <QMessageBox>

#include <array>

MainDlg::MainDlg()
    : QDialog(nullptr, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint),
      timer(this) {
  setWindowTitle(tr("Raw IMU data viewer"));
  auto layout = new QGridLayout(this);
  {
    std::array<QString, 4> strs = { "Sensor", "X", "Y", "Z" };
    for (size_t i = 0; i < 4; i++) {
      auto label = new QLabel(strs[i], this);
      label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      layout->addWidget(label, 0, i);
    }
  }
  addToLayout(1, "Acc: ", layout, acc_edit);
  addToLayout(2, "Gyr: ", layout, gyro_edit);
  addToLayout(3, "Mag: ", layout, mag_edit);
  {
    auto label = new QLabel("Port:", this);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(label, 4, 1);
    port_edit = new QLineEdit(this);
    layout->addWidget(port_edit, 4, 2);
    start_button = new QPushButton("Start", this);
    layout->addWidget(start_button, 4, 3);
  }
  setLayout(layout);
  connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
  connect(start_button, SIGNAL(pressed()), SLOT(startThread()));
  timer.setInterval(100);
  timer.start();
}

void MainDlg::addToLayout(const int row, const QString& name, QGridLayout* layout, std::array<QLineEdit*, 3>& line_edit) {
  auto label = new QLabel(name, this);
  layout->addWidget(label, row, 0);
  for (size_t i = 0; i < 3; i++) {
    line_edit[i] = new QLineEdit(this);
    line_edit[i]->setReadOnly(true);
    line_edit[i]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(line_edit[i], row, i + 1);
  }
}

void MainDlg::onTimeout() {
  if (!work_thread) {
    for (size_t i = 0; i < 3; i++) {
      acc_edit[i]->setText("");
      gyro_edit[i]->setText("");
      mag_edit[i]->setText("");
    }
    return;
  }
  const auto imu_data = work_thread->read();
  for (size_t i = 0; i < 3; i++) {
    acc_edit[i]->setText(QString("%1").arg(imu_data.acc[i]));
    gyro_edit[i]->setText(QString("%1").arg(imu_data.gyro[i]));
    mag_edit[i]->setText(QString("%1").arg(imu_data.mag[i]));
  }
}

void MainDlg::startThread() {
  if (work_thread) {
    work_thread->stop();
    delete work_thread;
    work_thread = nullptr;
    start_button->setText("Start");
  } else {
    work_thread = new WorkThread(port_edit->text(), this);
    connect(work_thread, SIGNAL(readFailed()), this, SLOT(serialReadFailed()));
    work_thread->start();
    start_button->setText("Stop");
  }
}

void MainDlg::serialReadFailed() {
  QMessageBox::critical(this, "Failure:"+port_edit->text(), port_edit->text() + ": read Failed");
}
