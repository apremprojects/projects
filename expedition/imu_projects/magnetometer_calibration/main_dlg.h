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
  void saveCalibration();
private:
	QTimer timer;
	QMutex mutex;
	QQueue<MagnetometerData> queue;
	WorkParams work_params;
	WorkThread* work_thread = nullptr;
	Eigen::Vector3f min_point = { 1.0f, 1.0f, 1.0f };
	Eigen::Vector3f max_point = { -1.0f, -1.0f, -1.0f };
	std::vector<Eigen::Vector3f> mag_data;
	int timeout_count = 0;
};
