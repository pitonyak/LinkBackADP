#ifndef LINKBACKUPTHREAD_H
#define LINKBACKUPTHREAD_H

#include <QThread>
#include "backupset.h"

class DBFileEntries;
class QDir;
class QCryptographicHash;

//**************************************************************************
//! Perform the actual work of generating a backup.
/*!
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

class LinkBackupThread : public QThread
{
  Q_OBJECT
public:

   //**************************************************************************
   /*! \brief Default constructor with no backup set.
    *
    *  \param [in] parent is this object's owner and the destructor will destroys all child objects.
    **************************************************************************/
  explicit LinkBackupThread(QObject *parent = 0);

  //**************************************************************************
  /*! \brief Constructor that sets this objects backupset.
     *
     *  \param [in] backupSet is the BackupSet to use for the next backup (contains filter and path information).
     *  \param [in] parent is this object's owner and the destructor will destroys all child objects.
     **************************************************************************/
  explicit LinkBackupThread(const BackupSet& backupSet, QObject *parent = 0);

  //**************************************************************************
  /*! \brief Destructor. Delete current and old file entries. */
  virtual ~LinkBackupThread();

  //**************************************************************************
  /*! \brief Return true if cancel was requested on the backup set. */
  //**************************************************************************
  bool isCancelRequested() const;

  //**************************************************************************
  /*! \brief Set the backup set, which contains path and filter information for the desired backup.
   *
   *  \param [in] backupSet is the BackupSet to use for the next backup (contains filter and path information).
   **************************************************************************/
  void setBackupSet(const BackupSet& backupSet);

  //**************************************************************************
  /*! \brief Get the full path to the most recent backup directory contained in the parentPath.
     *
     *  \param [in] parentPath is the directory containing existing backups.
     *  \return Full path to the most recent backup, or an empty string if no backup exists.
     **************************************************************************/
  static QString newestBackDirectory(const QString& parentPath);

  //**************************************************************************
  /*! \brief Get the full path to the most recent file entries object for a given hash.
     *
     * This method exists because file names are case senstive on Linux, but not on Windows.
     * I changed the hash names to no longer be upper case, which causes a problem on Linux.
     * This finds a file and matches it in a case insensitive way. The first to math
     * hashname.txt wins.
     *
     *  \param [in] parentPath is the directory containing existing backups.
     *  \param [in] hashName is the hash name to be used for the current backup.
     *  \return Full path to the most file that matches the hashname, or an empty string if no file exists.
     **************************************************************************/
  static QString findHashFileCaseInsensitive(const QString& parentPath, const QString& hashName);

  //**************************************************************************
  /*! \brief Create a directory to contain a new backup based on the current date and time in the format "yyyyMMdd-hhmmss".
     *
     *  \param [in] parentPath is the directory that will contain the new directory.
     *  \return Full path to the created directory, or an empty string if an error occurred.
     **************************************************************************/
  static QString createBackDirectory(const QString& parentPath);

protected:
  //**************************************************************************
  /*! \brief Start the thread and do the entire process. */
  //**************************************************************************
  virtual void run();

  //**************************************************************************
  /*! \brief Process an entire directory (as in backup this directory). Recurse as needed.
     *
     *  The to and from directory variables are modified as the backup is performed.
     *
     *  \param [in, out] currentFromDir is the directory which is being backedup.
     *  \param [in, out] currentToDir is the directory to which the backup is written.
     **************************************************************************/
  virtual void processDir(QDir& currentFromDir, QDir& currentToDir);

  //**************************************************************************
  /*! \brief Set the entire set of entries for the previous backup set with this one.
     *
     *  This object owns the set and will delete it, so, it must be allocated using new.
     *  Passing in a nullptr simply deletes the existing set freeing up the memory, leaving no entries.
     *
     *  \param Previous backup set.
     **************************************************************************/
  void setOldEntries(DBFileEntries* entries);

  //**************************************************************************
  /*! \brief Set the entire set of current entries (current backup set) with this one.
     *
     *  This object owns the set and will delete it, so, it must be allocated using new.
     *  Passing in a nullptr simply deletes the existing set freeing up the memory, leaving no entries.
     *
     *  \param New set of file entries.
     **************************************************************************/
  void setCurrentEntries(DBFileEntries* entries);

  //**************************************************************************
  /*! \brief Determine if a file or directory will be processed.
     *
     *  This is a thin wrapper around the passes method for the current BackupSet.
     *
     *  \param [in] info QFileInfo object referencing the file to be checked.
     *  \return True if the file or directory passes the backup set filters, false otherwise.
     *  \sa BackupSet::passes()
     **************************************************************************/
  bool passes(const QFileInfo& info) const;

  //**************************************************************************
  /*! \brief Return the number of entries in the previous backup object. */
  //**************************************************************************
  int numOldEntries() const;

signals:

public slots:
  //**************************************************************************
  /*! \brief Set the "cancel requested" flag so that the thread knows it should stop. */
  //**************************************************************************
  void requestCancel();

private:
  //**************************************************************************
  /*! \brief Set when the thread should stop running. */
  //**************************************************************************
  bool m_cancelRequested;

  //**************************************************************************
  /*! \brief List of entries built as files are processed. */
  //**************************************************************************
  DBFileEntries* m_currentEntries;

  //**************************************************************************
  /*! \brief . List of entries from the previous backup. */
  //**************************************************************************
  DBFileEntries* m_oldEntries;

  //**************************************************************************
  /*! \brief Path to the new backup, this includes the time/date stamp but not the head directory name where the backup begins. */
  //**************************************************************************
  QString m_toDirRoot;

  //**************************************************************************
  /*! \brief Path to the "from" directory without the first diretcory name. */
  //**************************************************************************
  QString m_fromDirWithoutTopDirName;

  //**************************************************************************
  /*! \brief Path to the last backup, this includes the time/date stamp but not the first root portion. */
  //**************************************************************************
  QString m_previousDirRoot;

  //**************************************************************************
  /*! \brief Contains all backup paratmers such filters and criteria. */
  //**************************************************************************
  BackupSet m_backupSet;
};

inline bool LinkBackupThread::isCancelRequested() const {
  return m_cancelRequested;
}



#endif // LINKBACKUPTHREAD_H
