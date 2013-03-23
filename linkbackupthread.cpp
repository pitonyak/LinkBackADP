#include "linkbackupthread.h"
#include "dbfileentries.h"
#include "linkbackupglobals.h"
#include <QDir>
#include <QCryptographicHash>

LinkBackupThread::LinkBackupThread(QObject *parent) : QThread(parent), m_cancelRequested(false), m_currentEntries(nullptr), m_oldEntries(nullptr)
{
    setPriority(QThread::IdlePriority);
}

LinkBackupThread::LinkBackupThread(const BackupSet& backupSet, QObject *parent) : QThread(parent), m_cancelRequested(false), m_currentEntries(nullptr), m_oldEntries(nullptr)
{
    setPriority(QThread::IdlePriority);
    setBackupSet(backupSet);
}


LinkBackupThread::~LinkBackupThread()
{
  DBFileEntries* entries = nullptr;
  setOldEntries(entries);
  setCurrentEntries(entries);
}

void LinkBackupThread::setOldEntries(DBFileEntries* entries)
{
  if (m_oldEntries != nullptr) {
    delete m_oldEntries;
  }
  m_oldEntries = entries;
}

void LinkBackupThread::setCurrentEntries(DBFileEntries* entries)
{
  if (m_currentEntries != nullptr)
  {
    delete m_currentEntries;
  }
  m_currentEntries = entries;
}


void LinkBackupThread::setBackupSet(const BackupSet& backupSet)
{
  m_backupSet = backupSet;
}

void LinkBackupThread::run()
{
  if (!::getCopyLinkUtil().setHashType(m_backupSet.getHashMethod()))
  {
    ERROR_MSG(QString(tr("Failed to setup hash generator.")), 0);
    return;
  }
  ::getCopyLinkUtil().resetStats();

  setOldEntries(nullptr);
  setCurrentEntries(new DBFileEntries());

  m_cancelRequested = false;
  m_previousDirRoot = newestBackDirectory(m_backupSet.getToPath());
  if (m_previousDirRoot.length() > 0) {
    INFO_MSG(QString(tr("Found previous backup in %1")).arg(m_previousDirRoot), 1);

    QString previousDBFileEntries = findHashFileCaseInsensitive(m_previousDirRoot, m_backupSet.getHashMethod());
    if (previousDBFileEntries.isEmpty())
    {
        WARN_MSG(QString(tr("Although a previous backup was found in %1, file entries were not found for hash method %2.")).arg(m_previousDirRoot, m_backupSet.getHashMethod()), 1);
    }
    else
    {
        setOldEntries(DBFileEntries::read(previousDBFileEntries));
        INFO_MSG(QString(tr("Found %1 entries in previous backup.")).arg(numOldEntries()), 1);
    }
  } else {
    WARN_MSG(QString(tr("No previous backup found.")), 1);
  }
  if (m_oldEntries == nullptr)
  {
    m_oldEntries = new DBFileEntries();
  }
  m_toDirRoot = createBackDirectory(m_backupSet.getToPath());

  QDir topFromDir(m_backupSet.getFromPath());

  // Name of the directory that is backed up without the path.
  QString topFromDirName = topFromDir.dirName();
  if (!topFromDir.exists() || topFromDirName.length() == 0) {
    WARN_MSG(QString(tr("Diretcory does not exist, or no directory name in %1, aborting backup.")).arg(m_backupSet.getFromPath()), 1);
    return;
  }

  QString canonicalPath = topFromDir.canonicalPath();
  m_fromDirWithoutTopDirName = canonicalPath.left(canonicalPath.length() - topFromDirName.length());

  QDir toDirLocation(m_toDirRoot);
  toDirLocation.mkdir(topFromDirName);
  toDirLocation.cd(topFromDirName);

  DEBUG_MSG(QString(tr("toDirRoot:%1 topFromDirName:%2 m_fromDir:%3")).arg(m_toDirRoot, topFromDirName, canonicalPath), 1);

  processDir(topFromDir, toDirLocation);
  TRACE_MSG(QString("Ready to write final hash summary %1").arg(m_toDirRoot + "/" + m_backupSet.getHashMethod() + ".txt"), 1);
  m_currentEntries->write(m_toDirRoot + "/" + m_backupSet.getHashMethod() + ".txt");

  INFO_MSG(QString(tr("Backup finished.")), 1);
  INFO_MSG(::getCopyLinkUtil().getStats(), 1);
}

void LinkBackupThread::processDir(QDir& currentFromDir, QDir& currentToDir)
{
  TRACE_MSG(QString("Processing directory %1").arg(currentFromDir.canonicalPath()), 1);
  // Process directories, then process files.
  QList<QFileInfo> list = currentFromDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden | QDir::Readable);
  QFileInfo info;
  foreach (info, list) {
    if (isCancelRequested()) {
      return;
    }
    TRACE_MSG(QString("Found Dir to processes %1").arg(info.canonicalFilePath()), 2);
    if (passes(info))
    {
      DEBUG_MSG(QString("Dir Passes: %1").arg(info.canonicalFilePath()), 2);
      if (!currentToDir.mkdir(info.fileName())) {
        ERROR_MSG(QString("Failed to create directory %1/%2").arg(currentToDir.canonicalPath(), info.fileName()), 1);
      } else if (!currentToDir.cd(info.fileName())) {
        ERROR_MSG(QString("Failed to cd to directory %1/%2").arg(currentToDir.canonicalPath(), info.fileName()), 1);
      } else if (!currentFromDir.cd(info.fileName())) {
        ERROR_MSG(QString("Failed to cd to directory %1/%2").arg(currentFromDir.canonicalPath(), info.fileName()), 1);
        if (!currentToDir.cdUp()) {
          ERROR_MSG("Failed to cd up a directory for the to directory", 1);
          requestCancel();
          return;
        }
      } else {
        processDir(currentFromDir, currentToDir);
        if (!currentToDir.cdUp()) {
          ERROR_MSG("Failed to cd up a directory for the to directory", 1);
          requestCancel();
          return;
        }
        if (!currentFromDir.cdUp()) {
          ERROR_MSG("Failed to cd up a directory for the from directory", 1);
          requestCancel();
          return;
        }
      }
    }
    else
    {
      DEBUG_MSG(QString("Skipping Dir: %1").arg(info.canonicalFilePath()), 2);
    }
  }
  // Now handle files
  list = currentFromDir.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::Hidden | QDir::Readable);
  foreach (info, list) {
    if (isCancelRequested()) {
      return;
    }
    QString fullPathFileToRead = info.canonicalFilePath();
    //TRACE_MSG(QString("Found File to test %1").arg(fullPathFileToRead), 2);
    if (passes(info))
    {
      //TRACE_MSG(QString("File passes %1").arg(fullPathFileToRead), 2);
      QFile fileToRead(fullPathFileToRead);
      // Just in case someone deleted the file.
      if (fileToRead.exists())
      {
        DBFileEntry* currentEntry = new DBFileEntry(info, m_fromDirWithoutTopDirName);
        const DBFileEntry* linkEntry = m_oldEntries->findEntry(m_backupSet.getCriteria(), currentEntry, m_fromDirWithoutTopDirName);

        QString pathToLinkFile = m_previousDirRoot;

        // If not in the old backup, search the current backup.
        if (linkEntry == nullptr)
        {
          linkEntry = m_currentEntries->findEntry(m_backupSet.getCriteria(), currentEntry, m_fromDirWithoutTopDirName);
          pathToLinkFile = m_toDirRoot;
        }

        if (linkEntry == nullptr)
        {
          bool failedToCopy = false;
          QString fullFileNameToWrite = m_toDirRoot + "/" + currentEntry->getPath();
          bool needHash = (currentEntry->getHash().length() == 0);
          if (needHash)
          {
            failedToCopy = !::getCopyLinkUtil().copyFileGenerateHash(fullPathFileToRead, fullFileNameToWrite);
            if (!failedToCopy)
            {
              currentEntry->setHash(getCopyLinkUtil().getLastHash());
            }
          }
          else if (!getCopyLinkUtil().copyFile(fullPathFileToRead, fullFileNameToWrite))
          {
            failedToCopy = true;
          }

          if (failedToCopy)
          {
            ERROR_MSG(QString(tr("EC %1")).arg(currentEntry->getPath()), 1);
          }
          else
          {
            INFO_MSG(QString(tr("C  %1")).arg(currentEntry->getPath()), 1);
            currentEntry->setLinkTypeCopy();
            m_currentEntries->addEntry(currentEntry);
            currentEntry = nullptr;
          }
        }
        else
        {
          if (getCopyLinkUtil().linkFile(pathToLinkFile + "/" + linkEntry->getPath(), m_toDirRoot + "/" + currentEntry->getPath()))
          {
            currentEntry->setLinkTypeLink();
            currentEntry->setHash(linkEntry->getHash());
            m_currentEntries->addEntry(currentEntry);
            INFO_MSG(QString(tr("L %1")).arg(currentEntry->getPath()), 1);
            currentEntry = nullptr;
          }
          else
          {
            ERROR_MSG(QString(tr("EL %1")).arg(currentEntry->getPath()), 1);
          }
        }
        if (currentEntry != nullptr)
        {
          delete currentEntry;
          currentEntry= nullptr;
        }
      }
    }
  }
  TRACE_MSG(QString("Finished with directory %1").arg(currentFromDir.canonicalPath()), 1);
}

bool LinkBackupThread::passes(const QFileInfo& info) const
{
  return m_backupSet.passes(info);
}

QString LinkBackupThread::newestBackDirectory(const QString& parentPath)
{
  QDir dir(parentPath);
  if (!dir.exists()) {
    ERROR_MSG("Directory does not exist in newest Back Directory", 1);
    return "";
  }
  // File names were created so that sorting the names places them in chronoligical order.
  dir.setFilter(QDir::Dirs);
  dir.setSorting(QDir::Name | QDir::Reversed);
  QFileInfoList list = dir.entryInfoList();
  QRegExp dirNameRegExp("^\\d{8}-\\d{6}$");
  for (int i = 0; i < list.size(); ++i) {
    QFileInfo fileInfo = list.at(i);
    if (dirNameRegExp.exactMatch(fileInfo.fileName())) {
      return fileInfo.canonicalFilePath();
    }
  }
  return "";
}

QString LinkBackupThread::findHashFileCaseInsensitive(const QString& parentPath, const QString& hashName)
{
    QDir dir(parentPath);
    if (!dir.exists()) {
      ERROR_MSG("Directory does not exist in newest Back Directory", 1);
      return "";
    }
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    QRegExp dirNameRegExp(QString("^%1\\.txt$").arg(hashName));
    dirNameRegExp.setCaseSensitivity(Qt::CaseInsensitive);

    for (int i = 0; i < list.size(); ++i) {
      QFileInfo fileInfo = list.at(i);
      if (dirNameRegExp.exactMatch(fileInfo.fileName())) {
        return fileInfo.canonicalFilePath();
      }
    }
    return "";
}

QString LinkBackupThread::createBackDirectory(const QString& parentPath)
{
  QDir dir(parentPath);
  if (!dir.exists()) {
    ERROR_MSG("Directory does not exist in createBackDirectory", 1);
    return "";
  }
  QString dirName = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");
  if (dir.mkdir(dirName) && dir.cd(dirName)) {
    return dir.canonicalPath();
  }
  return "";
}

int LinkBackupThread::numOldEntries() const {
  return (m_oldEntries != nullptr) ? m_oldEntries->count() : 0;
}

void LinkBackupThread::requestCancel() {
  ::getCopyLinkUtil().setCancelRequested(true);
  m_cancelRequested = true;
}
