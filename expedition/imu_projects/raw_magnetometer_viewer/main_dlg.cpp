#include "main_dlg.h"

#include <QDebug>
#include <QIntValidator>

MainDlg::MainDlg()
  : QDialog(nullptr, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint),
    work_params({"COM3", 25, 120}) {
  setupUi(this);
  work_thread = new WorkThread(this, mutex, queue, work_params);
  edit_duration->setValidator(new QIntValidator(0, 60 * 60, this));

  widget->setMutexAndQueue(mutex, queue);
  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
  connect(angle_x, SIGNAL(valueChanged(int)), widget, SLOT(angleXChanged(int)));
  connect(angle_y, SIGNAL(valueChanged(int)), widget, SLOT(angleYChanged(int)));
  connect(angle_z, SIGNAL(valueChanged(int)), widget, SLOT(angleZChanged(int)));
  connect(shift_center, SIGNAL(stateChanged(int)), widget, SLOT(shiftCenter(int)));
}

void MainDlg::buttonClicked() {
  work_params.port_name = edit_comm->text();
  work_params.duration_sec = edit_duration->text().toInt();
  if (button->text() == "Start") {
    widget->clearRingBuffers();
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
  widget->repaint();
  controls->setDisabled(work_thread->isRunning());
  button->setText(work_thread->isRunning() ? "Stop" : "Start");
}
