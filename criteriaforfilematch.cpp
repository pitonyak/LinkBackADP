#include "criteriaforfilematch.h"

CriteriaForFileMatch::CriteriaForFileMatch(QObject *parent) : QObject(parent)
{
  m_fileName = false;
  m_fullPath = true;
  m_dateTime = false;
  m_fileSize = true;
  m_fileHash = true;
}


CriteriaForFileMatch::CriteriaForFileMatch(const CriteriaForFileMatch& criteria, QObject *parent) : QObject(parent)
{
  copy(criteria);
}

const CriteriaForFileMatch& CriteriaForFileMatch::operator=(const CriteriaForFileMatch& criteria)
{
  return copy(criteria);
}

const CriteriaForFileMatch& CriteriaForFileMatch::copy(const CriteriaForFileMatch& criteria)
{
  if (this != & criteria)
  {
    m_fileName = criteria.m_fileName;
    m_fullPath = criteria.m_fullPath;
    m_dateTime = criteria.m_dateTime;
    m_fileSize = criteria.m_fileSize;
    m_fileHash = criteria.m_fileHash;
  }
  return *this;
}

bool CriteriaForFileMatch::getData(CriteriaField field) const
{
  switch (field) {
  case FileName:
    return isFileName();
    break;
  case FullPath:
    return isFullPath();
    break;
  case DateTime:
    return isDateTime();
    break;
  case FileSize:
    return isFileSize();
    break;
  case FileHash:
    return isFileHash();
    break;
  default:
    return false;
    break;
  }
}
void CriteriaForFileMatch::setField(CriteriaField field, bool value)
{
  switch (field) {
  case FileName:
    m_fileName = value;
    break;
  case FullPath:
    m_fullPath = value;
    break;
  case DateTime:
    m_dateTime = value;
    break;
  case FileSize:
    m_fileSize = value;
    break;
  case FileHash:
    m_fileHash = value;
    break;
  default:
    break;
  }
}

QXmlStreamWriter& CriteriaForFileMatch::operator<<(QXmlStreamWriter& writer) const
{
    writer.writeStartElement("Criteria");
    writer.writeTextElement("FullPath", m_fullPath ? "True" : "False");
    writer.writeTextElement("FileName", m_fileName ? "True" : "False");
    writer.writeTextElement("Time", m_dateTime ? "True" : "False");
    writer.writeTextElement("Size", m_fileSize ? "True" : "False");
    writer.writeTextElement("Hash", m_fileHash ? "True" : "False");
    writer.writeEndElement();
    return writer;
}

void CriteriaForFileMatch::setAllDefault()
{
    setFileName();
    setFullPath();
    setDateTime();
    setFileSize();
    setFileHash();
}


QXmlStreamReader& CriteriaForFileMatch::readCriteria(QXmlStreamReader& reader)
{
    setAllDefault();
    QString name;
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
            if (QString::compare(name, "Criteria", Qt::CaseInsensitive) == 0) {
                readInternals(reader);
                return reader;
            } else {
                reader.raiseError(QObject::tr("Not Criteria"));
            }
        } else if (reader.isEndElement()) {
            return reader;
        }
        reader.readNext();
    }
    return reader;
}

void CriteriaForFileMatch::readInternals(QXmlStreamReader& reader)
{
    QString name;
    bool valueIsEmpty;
    bool value;
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
        } else if (reader.isCharacters()) {
            valueIsEmpty = reader.text().isEmpty();
            value = QString::compare(reader.text().toString(), "True") == 0;
            if (name.isEmpty()) {
                // Do Nothing.
            } else if (QString::compare(name, "FullPath", Qt::CaseInsensitive) == 0) {
                if (valueIsEmpty) {
                    setFullPath();
                } else {
                    setFullPath(value);
                }
            } else if (QString::compare(name, "FileName", Qt::CaseInsensitive) == 0) {
                if (valueIsEmpty) {
                    setFileName();
                } else {
                    setFileName(value);
                }
            } else if (QString::compare(name, "Time", Qt::CaseInsensitive) == 0) {
                if (valueIsEmpty) {
                    setDateTime();
                } else {
                    setDateTime(value);
                }
            } else if (QString::compare(name, "Size", Qt::CaseInsensitive) == 0) {
                if (valueIsEmpty) {
                    setFileSize();
                } else {
                    setFileSize(value);
                }
            } else if (QString::compare(name, "Hash", Qt::CaseInsensitive) == 0) {
                if (valueIsEmpty) {
                    setFileHash();
                } else {
                    setFileHash(value);
                }
            } else if (QString::compare(name, "Criteria", Qt::CaseInsensitive) == 0) {
            } else {
                reader.raiseError(QObject::tr("Unrecognized Filter Element %1").arg(name));
            }
        } else if (reader.isEndElement()) {
            if (QString::compare(reader.name().toString(), "Criteria", Qt::CaseInsensitive) == 0)
            {
                return;
            }
            name = "";
        }
        reader.readNext();
    }
}
