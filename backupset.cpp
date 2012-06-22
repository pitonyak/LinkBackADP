#include "backupset.h"
#include "linkbackupglobals.h"
#include <QFile>
#include <QFileInfo>

BackupSet::BackupSet()
{
}

BackupSet::BackupSet(const BackupSet& backupSet)
{
  operator=(backupSet);
}

const BackupSet& BackupSet::operator=(const BackupSet& backupSet)
{
  if (this != &backupSet)
  {
    setFromPath(backupSet.m_fromPath);
    setToPath(m_toPath = backupSet.m_toPath);
    setHashMethod(m_hashMethod = backupSet.m_hashMethod);
    setFilters(backupSet.getFilters());
    setCriteria(backupSet.getCriteria());
  }
  return *this;
}

const QList<LinkBackFilter>& BackupSet::getFilters() const
{
    return m_filters;
}

void BackupSet::setFilters(const QList<LinkBackFilter>& filters)
{
    m_filters.clear();
    m_filters.append(filters);
}

const QList<CriteriaForFileMatch>& BackupSet::getCriteria() const
{
    return m_criteria;
}

void BackupSet::setCriteria(const QList<CriteriaForFileMatch>& criteria)
{
    m_criteria.clear();
    m_criteria.append(criteria);
}

BackupSet::~BackupSet()
{
  clear();
}

void BackupSet::clear()
{
  m_fromPath.clear();
  m_toPath.clear();
  m_filters.clear();
}

bool BackupSet::readFile(const QString& fullPath)
{
    QFile file(fullPath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamReader reader(&file);
    this->operator >>(reader);
    return reader.hasError() ? false : true;
}

bool BackupSet::writeFile(const QString& fullPath)
{
    QFile file(fullPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    this->operator <<(writer);
    return true;
}


QXmlStreamWriter& BackupSet::operator<<(QXmlStreamWriter& writer) const
{
    writer.writeStartElement("BackupSet");
    writer.writeAttribute("version", "1");
    writer.writeTextElement("From", getFromPath());
    writer.writeTextElement("To", getToPath());
    writer.writeTextElement("Hash", getHashMethod());

    writer.writeStartElement("Filters");
    LinkBackFilter filter;
    foreach (filter, m_filters)
    {
        writer << filter;
    }
    writer.writeEndElement();

    writer.writeStartElement("MatchCriteria");
    CriteriaForFileMatch criteria;
    foreach (criteria, m_criteria)
    {
        writer << criteria;
    }

    writer.writeEndElement();
    writer.writeEndElement();

    return writer;
}

QXmlStreamReader& BackupSet::operator>>(QXmlStreamReader& reader)
{
    return readFilter(reader);
}

QXmlStreamReader& BackupSet::readFilter(QXmlStreamReader& reader)
{
    setAllDefault();
    QString version = "1";
    QString name;
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
            if (QString::compare(name, "BackupSet", Qt::CaseInsensitive) == 0) {
                readInternals(reader);
                return reader;
            } else {
                reader.raiseError(QObject::tr("Not a BackupSet"));
            }
        } else if (reader.isEndElement()) {
            return reader;
        }
        reader.readNext();
    }
    return reader;
}

void BackupSet::readInternals(QXmlStreamReader& reader)
{
    QString name;
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
            if (QString::compare(name, "BackupSet", Qt::CaseInsensitive) == 0) {
                //name = "BackupSet";
            } else if (QString::compare(name, "From", Qt::CaseInsensitive) == 0) {
                //name = "From";
            } else if (QString::compare(name, "To", Qt::CaseInsensitive) == 0) {
                //name = "To";
            } else if (QString::compare(name, "Hash", Qt::CaseInsensitive) == 0) {
                //name = "Hash";
            } else if (QString::compare(name, "Filters", Qt::CaseInsensitive) == 0) {
                readFilters(reader);
            } else if (QString::compare(name, "MatchCriteria", Qt::CaseInsensitive) == 0) {
                readCriteria(reader);
            } else {
                reader.raiseError(QObject::tr("Unrecognized BackupSet Element %1").arg(name));
                name = "";
            }
            // What to do here?
            name = name;
        } else if (reader.isCharacters()) {
            if (QString::compare(name, "From", Qt::CaseInsensitive) == 0) {
                setFromPath(reader.text().toString());
            } else if (QString::compare(name, "To", Qt::CaseInsensitive) == 0) {
                setToPath(reader.text().toString());
            } else if (QString::compare(name, "Hash", Qt::CaseInsensitive) == 0) {
                setHashMethod(reader.text().toString());
            }
        } else if (reader.isEndElement()) {
            if (QString::compare(reader.name().toString(), "BackupSet", Qt::CaseInsensitive) == 0)
            {
                return;
            }
            name = "";
        }
        reader.readNext();
    }
}

void BackupSet::readFilters(QXmlStreamReader& reader)
{
    QString name;
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
            if (QString::compare(reader.name().toString(), "Filters", Qt::CaseInsensitive) == 0) {
            } else if (QString::compare(name, "Filter", Qt::CaseInsensitive) == 0) {
                LinkBackFilter filter;
                reader >> filter;
                m_filters.append(filter);
            } else {
                reader.raiseError(QObject::tr("Unrecognized Filters Element %1").arg(name));
            }
        } else if (reader.isCharacters()) {
        } else if (reader.isEndElement()) {
            if (QString::compare(reader.name().toString(), "Filters", Qt::CaseInsensitive) == 0)
            {
                return;
            }
        }
        reader.readNext();
    }
}

void BackupSet::readCriteria(QXmlStreamReader& reader)
{
    QString name;
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
            if (QString::compare(reader.name().toString(), "MatchCriteria", Qt::CaseInsensitive) == 0) {
            } else if (QString::compare(name, "Criteria", Qt::CaseInsensitive) == 0) {
                CriteriaForFileMatch criteria;
                reader >> criteria;
               m_criteria.append(criteria);
            } else {
                reader.raiseError(QObject::tr("Unrecognized MatchCriteria Element %1").arg(name));
            }
        } else if (reader.isCharacters()) {
        } else if (reader.isEndElement()) {
            if (QString::compare(reader.name().toString(), "MatchCriteria", Qt::CaseInsensitive) == 0)
            {
                return;
            }
        }
        reader.readNext();
    }
}


bool BackupSet::passes(const QFileInfo& info) const
{
  // I do it this way so that a new copy is not created every time.
  // I used to use a foreach loop.
  for (int i=0; i<m_filters.count(); ++i) {
    const LinkBackFilter& filter(m_filters[i]);
    if (filter.passes(info)) {
      TRACE_MSG(QString("Path %1 passed with filter (%2)").arg(info.absoluteFilePath(), filter.getMainValueAsString()), 5);
      return filter.isFilterMeansAccept();
    }
    else
    {
      TRACE_MSG(QString("Path %1 did not pass with filter (%2)").arg(info.absoluteFilePath(), filter.getMainValueAsString()), 5);
    }
  }
  return false;
}
