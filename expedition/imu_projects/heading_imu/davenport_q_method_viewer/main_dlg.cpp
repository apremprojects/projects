#include "main_dlg.h"

#include <QTime>
#include <QDebug>
#include <QFileDialog>

#include <vector>
#include <fstream>

#include <Eigen/Core>

using namespace Eigen;

MainDlg::MainDlg()
  : QDialog(nullptr, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint) {
  setupUi(this);

  widget_gl->setFocus();

  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(start_btn, SIGNAL(clicked()), this, SLOT(startThread()));

  work_thread = new WorkThread(this, mutex, queue, port_name);

  timer.setInterval(25);
  timer.start();
}

void MainDlg::timeout() {
  bool found = false;
  Quaternionf q;
  {
    QMutexLocker lock(&mutex);
    while (!queue.empty()) {
      found = true;
      q = queue.front();
      queue.pop_front();
    }
  }

  if (found) {
    widget_gl->add(q);
  }
}

void MainDlg::startThread() {
  if (start_btn->text() == "Start") {
    port_name = port_edit->text();
    work_thread->start();
    start_btn->setText("Stop");
    port_edit->setDisabled(true);
  } else {
    work_thread->stop();
    start_btn->setText("Start");
    port_edit->setDisabled(false);
  }
}

/*void MainDlg::loadCalibration() {
  const QString file_path = QFileDialog::getOpenFileName(this, "Load calibration", QString(), "*.mag_cal");
  std::FILE* file = fopen(file_path.toStdString().c_str(), "r");
  if (file != nullptr) {
    Eigen::Vector3d b;
    Eigen::Matrix3d cal_mat;
    int count1 = std::fscanf(file, "%lf, %lf, %lf\n", &b.x(), &b.y(), &b.z());
    int count2 = std::fscanf(file, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n", 
                        &cal_mat(0, 0), &cal_mat(0, 1), &cal_mat(0, 2),
                        &cal_mat(1, 0), &cal_mat(1, 1), &cal_mat(1, 2),
                        &cal_mat(2, 0), &cal_mat(2, 1), &cal_mat(2, 2));
    if (count1 == 3 && count2 == 9) {
      widget_gl->set(b.cast<float>(), cal_mat.cast<float>());
      qDebug() << b.x() << ", " << b.y() << ", " << b.z();
      qDebug() << cal_mat(0, 0) << ", " << cal_mat(0, 1) << ", " << cal_mat(0, 2);
      qDebug() << cal_mat(1, 0) << ", " << cal_mat(1, 1) << ", " << cal_mat(1, 2);
      qDebug() << cal_mat(2, 0) << ", " << cal_mat(2, 1) << ", " << cal_mat(2, 2);
    }
  }
  std::fclose(file);
}*/
