#include "main_dlg.h"
#include <iostream>
#include <fstream>

#include <Eigen/Eigenvalues>
#include <QDebug>
#include <QFileDialog>

MainDlg::MainDlg()
  : QDialog(nullptr, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint),
    work_params({"COM3", 25}) {
  setupUi(this);
  work_thread = new WorkThread(this, mutex, queue, work_params);
  //edit_duration->setValidator(new QIntValidator(0, 60 * 60, this));

  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(start_btn, SIGNAL(clicked()), this, SLOT(buttonClicked()));
  connect(spin_x, SIGNAL(valueChanged(int)), widget, SLOT(angleXChanged(int)));
  connect(spin_y, SIGNAL(valueChanged(int)), widget, SLOT(angleYChanged(int)));
  connect(spin_z, SIGNAL(valueChanged(int)), widget, SLOT(angleZChanged(int)));
  connect(load_calib_btn, SIGNAL(clicked()), this, SLOT(loadCalibration()));
  //connect(shift_center, SIGNAL(stateChanged(int)), widget, SLOT(shiftCenter(int)));
}

void MainDlg::buttonClicked() {
  work_params.port_name = port_edit->text();
  if (start_btn->text() == "Start") {
    widget->clearRingBuffers();
    timer.start(25);
    work_thread->start(QThread::HighestPriority);
  } else {
    work_thread->stop();
    timer.stop();
  }
  port_edit->setDisabled(work_thread->isRunning());
  start_btn->setText(work_thread->isRunning() ? "Stop" : "Start");
}

void MainDlg::timeout() {
    std::vector<Eigen::Vector3f> points;
    {
        QMutexLocker lock(&mutex);
        points.reserve(queue.size());
        while (!queue.empty()) {
            const auto data = queue.front();
            queue.pop_front();
            points.push_back(0.5 * M * (data.mag-origin));
        }
    }
    widget->setData(points);
    port_edit->setDisabled(work_thread->isRunning());
    start_btn->setText(work_thread->isRunning() ? "Stop" : "Start");
}

void MainDlg::loadCalibration() {
    const QString filepath = QFileDialog::getOpenFileName(this, "Input calibration: ", QString(), "*.mag_cal");
    std::FILE* file = std::fopen(filepath.toStdString().c_str(), "r");
    std::fscanf(file, "%f %f %f\n", &origin.x(), &origin.y(), &origin.z());
    for (int i = 0; i < 3; i++) {
        std::fscanf(file, "%f %f %f\n", &M(i, 0), &M(i, 1), &M(i, 2));
    }
    std::fclose(file);
    
}