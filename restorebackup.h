#ifndef RESTOREBACKUP_H
#define RESTOREBACKUP_H

#include <QDialog>

class QTreeView;

class RestoreBackup : public QDialog
{
  Q_OBJECT
public:
  explicit RestoreBackup(QWidget *parent = 0);
  
signals:
  
public slots:

private:
  void buildDialog();

  QTreeView* m_BackupTreeview;
};

#endif // RESTOREBACKUP_H
