#pragma once

#include <QDialog>

#include "ui_main_dlg.h"

class MainDlg : public QDialog, private Ui::MainDlgUi {
  Q_OBJECT
public:
  MainDlg();

private slots:
  void openObjFile();
  void groupItemClicked(QListWidgetItem* item);
private:
};
