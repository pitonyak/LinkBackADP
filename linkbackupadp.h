#ifndef LINKBACKUPADP_H
#define LINKBACKUPADP_H

#include "backupset.h"
#include "simpleloggerroutinginfo.h"

#include <QtGui/QMainWindow>
#include <QMutex>

namespace Ui
{
    class LinkBackupADP;
}

class LinkBackupThread;

class LinkBackupADP : public QMainWindow
{
    Q_OBJECT

public:
    LinkBackupADP(QWidget *parent = 0);
    ~LinkBackupADP();

    void cancelBackup();

public slots:
  void on_actionExit_activated();
  void on_actionEditBackup_activated();
  void on_actionStartBackup_activated();
  void on_actionCancelBackup_activated();
  void formattedMessage(const QString& formattedMessage, SimpleLoggerRoutingInfo::MessageCategory category);

private:
    Ui::LinkBackupADP *ui;
    LinkBackupThread* m_backupThread;
    QString m_configFilePath;
    BackupSet m_backupSet;
    long m_numLinesInEditor;
    long m_maxLinesInEditor;
    long m_numLinesDelete;
    QMutex m_editorMutex;
};

#endif // LINKBACKUPADP_H
