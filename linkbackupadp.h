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

//**************************************************************************
//! Wrapper around the LinkBackupADP main window handing things such as starting and ending backups and exiting the application.
/*!
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

class LinkBackupADP : public QMainWindow
{
  Q_OBJECT

public:
  /*! \brief Default constructor with no backup set.
   *
   *  \param [in] parent is this object's owner and the destructor will destroys all child objects.
   */
  explicit LinkBackupADP(QWidget *parent = 0);

  /*! \brief Destructor. Cancel any currently running backup. */
  ~LinkBackupADP();

  /*! \brief Request any currently running backups to stop. */
  void cancelBackup();

public slots:
  /*! \brief Display a text message in the contained editor.
   *
   *  \param [in] formattedMessage is a fully formatted text message ready to be added to the contained text window.
   *  \param [in] category identifies the message type such as error or debug.
   */
  void formattedMessage(const QString& formattedMessage, const SimpleLoggerRoutingInfo::MessageCategory category);

private slots:
  /*!  \brief Exit the application. */
  void on_actionExit_triggered();

  /*!  \brief Edit the BackupSet object. */
  void on_actionEditBackup_triggered();

  /*!  \brief Start a backup, */
  void on_actionStartBackup_triggered();

  /*!  \brief Request a backup to stop. */
  void on_actionCancelBackup_triggered();

private:
  /*!  \brief The User Interface portion of the GUI. */
  Ui::LinkBackupADP *ui;

  /*!  \brief Any existing backup thread. */
  LinkBackupThread* m_backupThread;

  /*!  \brief Full path to the cofiguration file (for the BackupSet). */
  QString m_configFilePath;

  /*!  \brief Controls the backup, contains filters, match criteria, and path information. */
  BackupSet m_backupSet;

  /*!  \brief Number of lines currently in the text editor. */
  long m_numLinesInEditor;

  /*!  \brief Max lines to allow in the text editor. */
  long m_maxLinesInEditor;

  /*!  \brief Number of lines to delete from the text editor (when there are too many lines). */
  long m_numLinesDelete;

  /*!  \brief Prevent multiple threads from changing the editor at the same time. */
  QMutex m_editorMutex;
};

#endif // LINKBACKUPADP_H
