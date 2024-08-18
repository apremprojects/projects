#pragma once

#include <QDialog>
#include <QTimer>
#include <QMutex>
#include <QQueue>

#include "work_thread.h"

#include "ui_main_dlg.h"

class MainDlg : public QDialog, private Ui::MainDlgUi {
  Q_OBJECT
public:
  MainDlg();

private slots:
  void timeout();
  void startThread();

private:
  QTimer timer;

  WorkThread* work_thread;

  QMutex mutex;
  QQueue<Eigen::Quaternionf> queue;

  QString port_name;
};
