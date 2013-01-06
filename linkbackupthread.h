#ifndef LINKBACKUPTHREAD_H
#define LINKBACKUPTHREAD_H

#include <QThread>
#include "backupset.h"

class DBFileEntries;
class QDir;
class QCryptographicHash;

//**************************************************************************
//! TODO:
/*!
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

class LinkBackupThread : public QThread
{
    Q_OBJECT
public:
    explicit LinkBackupThread(QObject *parent = 0);
    explicit LinkBackupThread(const BackupSet& m_backupSet, QObject *parent = 0);
    virtual ~LinkBackupThread();
    bool isCancelRequested() const;
    void setBackupSet(const BackupSet& m_backupSet);

    QString newestBackDirectory(const QString& parentPath) const;
    QString createBackDirectory(const QString& parentPath) const;

protected:
    virtual void run();
    virtual void processDir(QDir& currentFromDir, QDir& currentToDir);
    void setOldEntries(DBFileEntries* entries);
    void setCurrentEntries(DBFileEntries* entries);
    bool passes(const QFileInfo& info) const;

    int numOldEntries() const;

signals:

public slots:
    void requestCancel();

private:
    /*! \brief Set when the thread should stop running. */
    bool m_cancelRequested;
    /*! \brief List of entries built as files are processed. */
    DBFileEntries* m_currentEntries;
    /*! \brief . List of entries from the previous backup. */
    DBFileEntries* m_oldEntries;
    /*! \brief Path to the new backup, this includes the time/date stamp but not the head directory name where the backup begins. */
    QString m_toDirRoot;
    /*! \brief Path to the "from" directory without the first diretcory name. */
    QString m_fromDirWithoutTopDirName;
    /*! \brief Path to the last backup, this includes the time/date stamp but not the first root portion. */
    QString m_previousDirRoot;
    /*! \brief Contains all backup paratmers such filters and criteria. */
    BackupSet m_backupSet;
};

inline bool LinkBackupThread::isCancelRequested() const {
  return m_cancelRequested;
}



#endif // LINKBACKUPTHREAD_H
