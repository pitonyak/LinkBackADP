#ifndef LINKBACKUPADP_H
#define LINKBACKUPADP_H

#include "backupset.h"
#include "simpleloggerroutinginfo.h"

#include <QMainWindow>
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
    explicit LinkBackupADP(QWidget *parent = 0);
    ~LinkBackupADP();

    void cancelBackup();

public slots:
  void formattedMessage(const QString& formattedMessage, SimpleLoggerRoutingInfo::MessageCategory category);

private slots:
  void on_actionExit_triggered();
  void on_actionEditBackup_triggered();
  void on_actionStartBackup_triggered();
  void on_actionCancelBackup_triggered();

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
