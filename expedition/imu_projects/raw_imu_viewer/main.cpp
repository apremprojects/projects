#include <QApplication>

#include "main_dlg.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);  
  MainDlg dlg;
  dlg.showMaximized();
  return app.exec();
}
