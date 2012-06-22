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

DBFileEntry::DBFileEntry(const QFileInfo& info, const QString& rootPath)
{
  m_time = info.lastModified();
  m_size = info.size();
  QString fullPath = info.canonicalFilePath();
  if (rootPath.length() == 0)
  {
    m_path = fullPath;
  } else if (fullPath.startsWith(rootPath, Qt::CaseSensitive)) {
    m_path = fullPath.right(fullPath.length() - rootPath.length());
  } else {
    qDebug(qPrintable(QString("File path (%1) does not contain root path (%2)").arg(fullPath, rootPath)));
    m_path = fullPath;
  }
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
