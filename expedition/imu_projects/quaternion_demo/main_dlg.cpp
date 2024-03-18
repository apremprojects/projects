#include "main_dlg.h"
//#include "./../obj_file/obj_reader.h"

#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <QListWidgetItem>

using namespace Eigen;

#define DEG_TO_RAD(theta) ((std::atan(1.0f) / 45.0f) * static_cast<float>(theta))

MainDlg::MainDlg()
  : QDialog(nullptr, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint) {
  setupUi(this);
  widget_gl->setFocus();
  
  connect(x_spin, SIGNAL(valueChanged(double)), this, SLOT(xValueChanged(double)));
  connect(y_spin, SIGNAL(valueChanged(double)), this, SLOT(yValueChanged(double)));
  connect(z_spin, SIGNAL(valueChanged(double)), this, SLOT(zValueChanged(double)));
  connect(angle_spin, SIGNAL(valueChanged(int)), this, SLOT(angleValueChanged(int)));
}

void MainDlg::update() {
  Vector3d v(x_spin->value(), y_spin->value(), z_spin->value());
  if (v.norm() != 0.0) {
    const float theta = DEG_TO_RAD(angle_spin->value());
    widget_gl->set(theta, v.normalized().cast<float>());
  }
}

void MainDlg::xValueChanged(double x) {
  update();
}

void MainDlg::yValueChanged(double y) {
  update();
}

void MainDlg::zValueChanged(double z) {
  update();
}

void MainDlg::angleValueChanged(int angle) {
  update();
}
