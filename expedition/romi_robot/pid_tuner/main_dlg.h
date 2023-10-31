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
  void spButtonClicked();

private:
  QTimer timer;
  QMutex mutex;
  QQueue<WheelData> queue;
  GraphData left_wheel;
  GraphData right_wheel;
  WorkParams work_params;
  WorkThread* work_thread = nullptr;
};
