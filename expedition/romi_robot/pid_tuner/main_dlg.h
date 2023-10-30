#pragma once

#include <QDialog>
#include <QTimer>

#include "work_thread.h"

#include "ui_main_dlg.h"

class MainDlg : public QDialog, private Ui::MainDlgUi {
  Q_OBJECT
public:
  MainDlg();

private slots:
  void timeout();
  void buttonClicked();

private:
  QTimer timer;
  QMutex mutex;
  QQueue<ImuData> queue;
  GraphData acc_data;
  GraphData gyro_data;
  std::vector<Eigen::Vector3f> mag_data;
  WorkParams work_params;
  WorkThread* work_thread = nullptr;
};
