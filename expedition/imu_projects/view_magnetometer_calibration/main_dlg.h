#pragma once

#include <QDialog>
#include <QTimer>
#include <QMutex>

#include "work_thread.h"

#include "ui_main_dlg.h"

class MainDlg : public QDialog, private Ui::MainDlgUi {
  Q_OBJECT
public:
  MainDlg();

private slots:
  void timeout();
  void buttonClicked();
  void loadCalibration();
private:
	QTimer timer;
	QMutex mutex;
	QQueue<MagnetometerData> queue;
	WorkParams work_params;
	WorkThread* work_thread = nullptr;
	Eigen::Matrix3f M = Eigen::Matrix3f::Zero();
	Eigen::Vector3f origin = Eigen::Vector3f::Zero();
};
