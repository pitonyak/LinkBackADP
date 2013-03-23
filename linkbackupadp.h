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
    //**************************************************************************
    //! Constructor. Builds widgets and other things.
    /*!
     * \param [in,out] parent Object's owner. Parent object's destructor destroys all child objects.
     *
     ***************************************************************************/
    explicit LinkBackupADP(QWidget *parent = 0);

    //**************************************************************************
    //! Destructor. Cancels the backup and cleans up all UI items.
    /*!
     *
     ***************************************************************************/
    ~LinkBackupADP();

    //**************************************************************************
    //! Cancels any currently running backup threads.
    /*!
     *
     ***************************************************************************/
    void cancelBackup();

public slots:
    //**************************************************************************
    //! Display the message in the text editor.
    /*!
     * \param [in] formattedMessage Message to print.
     * \param [in] category Message category, used to color code messages.
     *
     ***************************************************************************/
  void formattedMessage(const QString& formattedMessage, SimpleLoggerRoutingInfo::MessageCategory category);

private slots:
  //**************************************************************************
  //! Handle exit application. Save window geometry and exit; causes currently running backup to stop.
  /*!
   *
   ***************************************************************************/
  void on_actionExit_triggered();

  //**************************************************************************
  //! Edit the backup parameters.
  /*!
   *
   ***************************************************************************/
  void on_actionEditBackup_triggered();

  //**************************************************************************
  //! Begin a backup.
  /*!
   *
   ***************************************************************************/
  void on_actionStartBackup_triggered();

  //**************************************************************************
  //! Cancel an active backup.
  /*!
   *
   ***************************************************************************/
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
