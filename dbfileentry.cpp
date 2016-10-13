#include "dbfileentry.h"
#include "stringhelper.h"

#include <QTextStream>
#include <QStringList>
#include <QFileInfo>

QString DBFileEntry::dateTimeFormat= "yyyyMMddThh:mm:ss.zzz";
QChar DBFileEntry::fieldSeparator = ',';

DBFileEntry::DBFileEntry() : m_size(0), m_linkType('C')
{
}

DBFileEntry::DBFileEntry(const DBFileEntry& entry) : DBFileEntry()
{
  operator=(entry);
}

DBFileEntry::DBFileEntry(const QFileInfo& info, const QString& rootPath) : m_size(info.size()), m_time(info.lastModified()), m_path(info.canonicalFilePath())
{
  if (rootPath.length() > 0)
  {
    if (m_path.startsWith(rootPath, Qt::CaseSensitive)) {
      // Remove the root-path from the full path.
      m_path = m_path.right(m_path.length() - rootPath.length());
    } else {
      // Unsure how this happened, the full path should always contain the root path.
      qDebug(qPrintable(QString("File path (%1) does not contain root path (%2)").arg(m_path, rootPath)));
    }
  }
}

DBFileEntry::~DBFileEntry()
{
}


bool DBFileEntry::readLine(QTextStream& stream)
{
  if (stream.status() != QTextStream::Ok || stream.atEnd()) {
    return false;
  }
  stream.skipWhiteSpace();
  QString line = stream.readLine();
  if (line.isNull()) {
    return false;
  }
  //This fails if the path contains a comma.
  //QStringList tokens = line.split(fieldSeparator);
  QStringList tokens = StringHelper::split(fieldSeparator, line, 5);
  if (tokens.count() < 5) {
    return false;
  }
  if (tokens[0].length() > 0) {
    m_linkType = tokens[0].at(0);
  }
  m_time = QDateTime::fromString(tokens[1], dateTimeFormat);
  m_hash = tokens[2];
  m_hash = m_hash.toUpper();
  bool ok;
  m_size = tokens[3].toULongLong(&ok, 10);
  if (!ok) {
    return false;
  }
  m_path = tokens[4];
  return true;
}

bool DBFileEntry::writeLine(QTextStream& stream)
{
  stream << m_linkType << fieldSeparator;
  stream << m_time.toString(dateTimeFormat) << fieldSeparator;
  stream << m_hash << fieldSeparator;
  stream << m_size << fieldSeparator;
  stream << m_path << "\n";
  return (stream.status() == QTextStream::Ok);
}

DBFileEntry& DBFileEntry::operator=(const DBFileEntry& entry)
{
  if (&entry != this)
  {
    m_size = entry.m_size;
    m_linkType = entry.m_linkType;
    m_time = entry.m_time;
    m_path = entry.m_path;
    m_hash = entry.m_hash;
  }
  return *this;
}
