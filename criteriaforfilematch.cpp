#include "criteriaforfilematch.h"
#include "linkbackupglobals.h"


#include <QMetaEnum>
#include <QMetaObject>

CriteriaForFileMatch::CriteriaForFileMatch(QObject *parent) : QObject(parent), m_criteria(new QMap<CriteriaField, bool>())
{
    setField(CriteriaField::FileName | CriteriaField::Time, false);
    setField(CriteriaField::FullPath | CriteriaField::Size | CriteriaField::Hash, true);
}

CriteriaForFileMatch::CriteriaForFileMatch(const CriteriaForFileMatch& criteria, QObject *parent) : QObject(parent), m_criteria(new QMap<CriteriaField, bool>())
{
  copy(criteria);
}

CriteriaForFileMatch::~CriteriaForFileMatch()
{
    if (m_criteria != nullptr)
    {
        delete m_criteria;
        m_criteria = nullptr;
    }
}

CriteriaForFileMatch& CriteriaForFileMatch::operator=(const CriteriaForFileMatch& criteria)
{
  if (this != & criteria)
  {
      *m_criteria = *criteria.m_criteria;
  }
  return *this;
}

const CriteriaForFileMatch& CriteriaForFileMatch::copy(const CriteriaForFileMatch& criteria)
{
  return operator=(criteria);
}

bool CriteriaForFileMatch::getField(CriteriaField field) const
{
    return ((m_criteria != nullptr) && (m_criteria->contains(field))) ? (*m_criteria)[field] : false;
}

void CriteriaForFileMatch::setField(CriteriaFields fields, bool value)
{
    TRACE_MSG(QString(tr("Enter CriteriaForFileMatch::setField(%1, %2)")).arg(fields).arg(value), 10);
    if (m_criteria == nullptr)
    {
        m_criteria = new QMap<CriteriaField, bool>();
    }

    const QMetaObject* metaObj = metaObject();
    QMetaEnum metaEnum = metaObj->enumerator(metaObj->indexOfEnumerator("CriteriaField"));
    for (int i=0; i<metaEnum.keyCount(); ++i)
    {
      CriteriaField criteria = static_cast<CriteriaField>(metaEnum.value(i));
      if ((fields & criteria) == criteria)
      {
        TRACE_MSG(QString(tr("Inserting(%1, %2)")).arg(criteria).arg(value), 15);
        m_criteria->insert(criteria, value);
      }
    }
}

QXmlStreamWriter& CriteriaForFileMatch::operator<<(QXmlStreamWriter& writer) const
{
    writer.writeStartElement("Criteria");

    const QMetaObject* metaObj = metaObject();
    QMetaEnum metaEnum = metaObj->enumerator(metaObj->indexOfEnumerator("CriteriaField"));
    for (int i=0; i<metaEnum.keyCount(); ++i)
    {
      CriteriaField criteria = static_cast<CriteriaField>(metaEnum.value(i));
      writer.writeTextElement(metaEnum.key(i), getField(criteria) ? "True" : "False");
    }
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
    TRACE_MSG("Enter readCriteria", 10);
    setAllDefault();
    QString name;
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
            if (QString::compare(name, "Criteria", Qt::CaseInsensitive) == 0) {
                readInternals(reader);
                TRACE_MSG("Return readCriteria", 10);
                return reader;
            } else {
                reader.raiseError(QObject::tr("Not Criteria"));
            }
        } else if (reader.isEndElement()) {
            return reader;
        }
        reader.readNext();
    }
    TRACE_MSG("Exit readCriteria", 10);
    return reader;
}

void CriteriaForFileMatch::readInternals(QXmlStreamReader& reader)
{
    TRACE_MSG("Enter readInternals", 10);
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
                TRACE_MSG("Return readInternals", 10);
                return;
            }
            name = "";
        }
        reader.readNext();
    }
    TRACE_MSG("Exit readInternals", 10);
}
