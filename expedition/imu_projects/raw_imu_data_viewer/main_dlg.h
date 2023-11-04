#pragma once

#include <QTimer>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

#include <array>
#include "work_thread.h"

class MainDlg : public QDialog {
  Q_OBJECT
public:
  MainDlg();

private slots:
  void onTimeout();
  void startThread();
  void serialReadFailed();

private:
  void addToLayout(const int row, const QString& name, QGridLayout* layout, std::array<QLineEdit*, 3>& line_edit);

  std::array<QLineEdit*, 3> acc_edit;
  std::array<QLineEdit*, 3> gyro_edit;
  std::array<QLineEdit*, 3> mag_edit;
  QLineEdit* port_edit;
  QPushButton* start_button;

  QTimer timer;
  WorkThread* work_thread = nullptr;
};
