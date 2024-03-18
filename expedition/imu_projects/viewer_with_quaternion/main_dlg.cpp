#include "main_dlg.h"
#include "./../obj_file/obj_reader.h"

#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <QListWidgetItem>

using namespace Eigen;

MainDlg::MainDlg()
  : QDialog(nullptr, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint) {
  setupUi(this);
  widget_gl->setFocus();
  connect(open_btn, SIGNAL(clicked()), this, SLOT(openObjFile()));
  connect(groups_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(groupItemClicked(QListWidgetItem*)));
}

void MainDlg::openObjFile() {
  const QString file_path = QFileDialog::getOpenFileName(this, tr("Open"), QString(), "*.obj");
  const auto obj_data = ObjReader::load(file_path.toStdString());
  groups_list->clear();
  for (const auto& group : obj_data->groups) {
    groups_list->addItem(group.name.c_str());
    QListWidgetItem* list_item = groups_list->item(groups_list->count()-1);
    list_item->setFlags(list_item->flags() | Qt::ItemIsUserCheckable);
    list_item->setCheckState(Qt::Checked);
  }
  widget_gl->set(*obj_data);
}

void MainDlg::groupItemClicked(QListWidgetItem* item) {
  widget_gl->setVisibility(item->text().toStdString(), item->checkState() == Qt::Checked);
}