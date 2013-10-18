#include "restorebackup.h"

#include <QTreeView>
#include <QFileSystemModel>
#include <QHBoxLayout>

RestoreBackup::RestoreBackup(QWidget *parent) :
  QDialog(parent), m_BackupTreeview(nullptr)
{
  buildDialog();
}

void RestoreBackup::buildDialog()
{
  m_BackupTreeview = new QTreeView();

  QFileSystemModel *model = new QFileSystemModel;
  model->setRootPath(QDir::currentPath());
  //model->setRootPath("/andrew0/home/andy/Devsrc/Qt/");
  m_BackupTreeview->setModel(model);

  QHBoxLayout *hLayout = new QHBoxLayout();
  //hLayout->addChildWidget(m_BackupTreeview);
  hLayout->addWidget(m_BackupTreeview);
  setLayout(hLayout);
}

