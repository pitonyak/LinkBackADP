#include "dbfileentries.h"
#include "criteriaforfilematch.h"
#include "linkbackupglobals.h"
#include <QFileInfo>
#include <QTextStream>
#include <QCryptographicHash>
#include <QString>


DBFileEntries::DBFileEntries()
{
}

DBFileEntries::~DBFileEntries() {
  clear();
}

void DBFileEntries::addEntry(DBFileEntry *entry)
{
  if (entry != nullptr) {
    int n = m_entries.count();
    m_entries.append(entry);
    m_pathToEntry.insert(entry->getPath(), n);
    if (!m_hashToSizeToEntry.contains(entry->getHash())) {
      QMultiHash<quint64, int> *hash = new QMultiHash<quint64, int>();
      hash->insertMulti(entry->getSize(), n);
      m_hashToSizeToEntry.insert(entry->getHash(), hash);
    } else {
      m_hashToSizeToEntry.value(entry->getHash())->insertMulti(entry->getSize(), n);
    }
  }
}

void DBFileEntries::clear()
{
  m_pathToEntry.clear();
  qDeleteAll(m_entries);
  m_entries.clear();

  QHash<QString, QMultiHash<quint64, int>* >::iterator i = m_hashToSizeToEntry.begin();
  while (i != m_hashToSizeToEntry.end()) {
    delete i.value();
    ++i;
  }

  m_hashToSizeToEntry.clear();
}

bool DBFileEntries::entriesMatch(const CriteriaForFileMatch& criteria, const DBFileEntry* myEntry, DBFileEntry* entryToMatch, const QString& matchInitialPath)
{
  if (myEntry == nullptr || entryToMatch == nullptr)
  {
    return false;
  }
  if (criteria.isFullPath())
  {
    if (myEntry->getPath().compare(entryToMatch->getPath(), Qt::CaseSensitive) != 0)
    {
      return false;
    }
  }
  else if (criteria.isFileName())
  {
    // I do not need to check file name if the full path is the same.
    QFileInfo fileOne(myEntry->getPath());
    QFileInfo fileTwo(entryToMatch->getPath());
    if (fileOne.fileName().compare(fileTwo.fileName(), Qt::CaseSensitive))
    {
      return false;
    }
  }

  if (criteria.isFileSize() && myEntry->getSize() != entryToMatch->getSize())
  {
    return false;
  }
  if (criteria.isDateTime() && myEntry->getTime() != entryToMatch->getTime())
  {
    return false;
  }

  if (criteria.isFileHash())
  {
    if (entryToMatch->getHash().length() == 0)
    {
      if (::getCopyLinkUtil().generateHash(matchInitialPath + "/" + entryToMatch->getPath()))
      {
        entryToMatch->setHash(::getCopyLinkUtil().getLastHash());
      }
      else
      {
        return false;
      }
    }
    if (entryToMatch->getHash().compare(myEntry->getHash(), Qt::CaseInsensitive) != 0)
    {
      return false;
    }
  }
  return true;
}

const DBFileEntry* DBFileEntries::findEntry(const CriteriaForFileMatch& criteria, DBFileEntry* entry, const QString& matchInitialPath) const
{
  if (entry == nullptr) {
    return nullptr;
  }
  DBFileEntry* foundEntry = nullptr;

  // There can be only one full path, so, check that first.
  if (criteria.isFullPath())
  {
    if (!m_pathToEntry.contains(entry->getPath()))
    {
      return nullptr;
    }
    foundEntry = m_entries.value(m_pathToEntry.value(entry->getPath()));
    if (entriesMatch(criteria, foundEntry, entry, matchInitialPath))
    {
      return foundEntry;
    }
    return nullptr;
  }

  // Now, try using criteria that will reduce the size the fastest.
  if (criteria.isFileHash())
  {
    if (entry->getHash().length() == 0)
    {
      if (!::getCopyLinkUtil().generateHash(matchInitialPath + "/" + entry->getPath()))
      {
        ERROR_MSG(QString(QObject::tr("Error generating hash for %1")).arg(entry->getPath()), 1);
        return nullptr;
      }
      entry->setHash(getCopyLinkUtil().getLastHash());
    }

    // Sadly, I now enforce that the file size and the hash match, regardless.
    QMultiHash<quint64, int> *hash = m_hashToSizeToEntry.value(entry->getHash());
    if (hash != nullptr && hash->contains(entry->getSize()))
    {
      QList<int> entries = hash->values(entry->getSize());
      QList<int>::const_iterator i = entries.constBegin();
      while (i != entries.constEnd())
      {
        foundEntry = m_entries.value(*i);
        if (entriesMatch(criteria, foundEntry, entry, matchInitialPath))
        {
          return foundEntry;
        }
        ++i;
      }
    }
    // Failed to match on hash.
    return nullptr;
  }

  // We now know that we need not match full path or file hash, so, we must traverse the entire list
  // to see what matches. If we get here,
  if (criteria.isFileSize() || criteria.isDateTime() || criteria.isFileName())
  {
    // This is simply crazy, do not do this!
    foreach (foundEntry, m_entries)
    {
      if (entriesMatch(criteria, foundEntry, entry, matchInitialPath))
      {
        return foundEntry;
      }
    }
  }
  return nullptr;
}

const DBFileEntry* DBFileEntries::findEntry(const QList<CriteriaForFileMatch>& criteria, DBFileEntry* entry, const QString& matchInitialPath) const
{
  const DBFileEntry* foundEntry = nullptr;
  if (m_entries.size() > 0)
  {
    QList<CriteriaForFileMatch>::const_iterator i = criteria.constBegin();
    while (i != criteria.constEnd() && foundEntry == nullptr) {
      foundEntry = findEntry(*i, entry, matchInitialPath);
      ++i;
    }
  }
  return foundEntry;
}

DBFileEntries* DBFileEntries::read(const QString& path)
{
  DBFileEntries* rc = nullptr;
  QFile file(path);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream reader(&file);
    rc = read(reader);
  } else {
    ERROR_MSG(QString(QObject::tr("Failed to open file (%1) to read the DB File Entries")).arg(path), 1);
  }
  return rc;
}

DBFileEntries* DBFileEntries::read(QTextStream& reader)
{
  long i = 0;
  DBFileEntries* rc = new DBFileEntries();
  while (!reader.atEnd()) {
    ++i;
    DBFileEntry* entry = new DBFileEntry();
    if (!entry->readLine(reader)) {
      delete entry;
      delete rc;
      rc = nullptr;
      ERROR_MSG(QString(QObject::tr("Failed reading at DB File Entry %1")).arg(i), 1);
      return rc;
    } else {
      rc->addEntry(entry);
    }
    reader.skipWhiteSpace();
  }
  return rc;
}

bool DBFileEntries::write(const QString& path) const
{
  bool rc = false;
  QFile file(path);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream writer(&file);
    rc = write(writer);
    file.close();
  } else {
    ERROR_MSG(QString(QObject::tr("Failed to open file (%1) to write the DB File Entries")).arg(path), 1);
  }
  return rc;
}

bool DBFileEntries::write(QTextStream& writer) const
{
  DBFileEntry* entry = nullptr;
  foreach (entry,  m_entries)
  {
    if (entry == nullptr)
    {
      return false;
    }
    if (!entry->writeLine(writer))
    {
      ERROR_MSG(QString(QObject::tr("Failed to write a DB File Entry")), 1);
      return false;
    }
  }
  return true;
}
