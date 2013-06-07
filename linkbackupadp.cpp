#include <QtPrintSupport/QPrinter>
#include "linkbackupadp.h"
#include "ui_linkbackupadp.h"
#include "linkbackupthread.h"
#include "simpleloggerroutinginfo.h"
#include "linkbackupglobals.h"
#include "logroutinginfodialog.h"

#include "backupsetdialog.h"
#include "ui_backupsetdialog.h"

#include <QDir>
#include <QFileInfoList>
#include <QRegExp>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

LinkBackupADP::LinkBackupADP(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LinkBackupADP), m_backupThread(0),
      m_numLinesInEditor(0), m_maxLinesInEditor(1024), m_numLinesDelete(100)
{
    ui->setupUi(this);
    setCentralWidget(ui->logTextEdit);

    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    // create docks, toolbars, etc...
    restoreState(settings.value("mainWindowState").toByteArray());

    // Connect the timer to the processing timer every two seconds.
    QTimer* logProcessingTimer = new QTimer();
    connect(logProcessingTimer, SIGNAL(timeout()), &getLogger(), SLOT(processQueuedMessages()));
    logProcessingTimer->start(2000);
    getLogger().enableMessageQueue();
}

LinkBackupADP::~LinkBackupADP()
{
  cancelBackup();
  delete ui;
}

void LinkBackupADP::on_actionExit_triggered()
{
  QSettings settings;
  settings.setValue("mainWindowGeometry", saveGeometry());
  settings.setValue("mainWindowState", saveState());
  QApplication::quit();
}

void LinkBackupADP::on_actionEditBackup_triggered()
{
  BackupSetDialog dlg(m_backupSet, this);
  dlg.setConfigFilePath(m_configFilePath);
  if (dlg.exec() == QDialog::Accepted) {
    m_backupSet = dlg.getBackupSet();
    m_configFilePath = dlg.getConfigFilePath();
  }
}

void LinkBackupADP::on_actionStartBackup_triggered()
{
  cancelBackup();
  if (m_backupThread != 0) {
    delete m_backupThread;
    m_backupThread = 0;
  }
  m_backupThread = new LinkBackupThread(m_backupSet, this);

  m_backupThread->start(m_backupSet.stringToPriority(m_backupSet.getPriority(), QThread::InheritPriority));
}

void LinkBackupADP::on_actionCancelBackup_triggered()
{
  cancelBackup();
}

void LinkBackupADP::cancelBackup()
{
  if (m_backupThread != 0) {
    m_backupThread->requestCancel();
  }
}

void LinkBackupADP::formattedMessage(const QString& formattedMessage, const SimpleLoggerRoutingInfo::MessageCategory category)
{
    if (formattedMessage.length() == 0)
    {
        return;
    }

    try
    {
        m_editorMutex.lock();
        if (m_numLinesInEditor > m_maxLinesInEditor)
        {
            QTextCursor tc = ui->logTextEdit->textCursor();
            tc.beginEditBlock();
            tc.movePosition( QTextCursor::Start );
            tc.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, m_numLinesDelete);
            tc.removeSelectedText();
            tc.endEditBlock();
            m_numLinesInEditor -= m_numLinesDelete;
            //ui->logTextEdit->setTextCursor( tc );
        }
        bool colorChanged = true;
        QColor savedColor = ui->logTextEdit->textColor();
        switch (category)
        {
        case SimpleLoggerRoutingInfo::ErrorMessage:
            ui->logTextEdit->setTextColor(QColor(Qt::red));
            break;
        case SimpleLoggerRoutingInfo::WarningMessage:
            ui->logTextEdit->setTextColor(QColor(Qt::magenta));
            break;
        case SimpleLoggerRoutingInfo::DebugMessage:
            ui->logTextEdit->setTextColor(QColor(Qt::green));
            break;
        case SimpleLoggerRoutingInfo::TraceMessage:
            ui->logTextEdit->setTextColor(QColor(Qt::gray));
            break;
        default:
            colorChanged = false;
        }

        ++m_numLinesInEditor;
        ui->logTextEdit->append(formattedMessage);
        if (colorChanged)
        {
            ui->logTextEdit->setTextColor(savedColor);
        }
        m_editorMutex.unlock();
    } catch(...) {
        m_editorMutex.unlock();
    }

}


void LinkBackupADP::on_actionConfigureLog_triggered()
{

  QList<SimpleLoggerRoutingInfo> & all_Routes = getLogger().getRouting();
  if (all_Routes.size() > 0)
  {
    LogRoutingInfoDialog dlg(all_Routes[0], this);
    dlg.exec();
  }
  else
  {
    QMessageBox msgBox;
    msgBox.setText(tr("Nothing to configure"));
    msgBox.setInformativeText("There are no routes");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}
