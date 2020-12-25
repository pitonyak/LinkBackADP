#include "linkbackfilter.h"
#include "linkbackupglobals.h"

#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QRegExp>
#include <QRegExp>
#include <QMetaObject>
#include <QMetaEnum>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "stringhelper.h"

LinkBackFilter::LinkBackFilter(QObject *parent) :
  QObject(parent), m_compareType(LinkBackFilter::Equal), m_compareField(LinkBackFilter::FullPath), m_caseSensitivity(Qt::CaseInsensitive), m_invertFilterResult(false), m_filterMeansAccept(true), m_filterFiles(true), m_filterDirs(true), m_multiValued(false), m_values(nullptr), m_expressions(nullptr)
{
  // After this, the lists are guaranteed to exist.
  clearLists(false, true);
}


LinkBackFilter::LinkBackFilter(const LinkBackFilter& filter, QObject *parent) :
  QObject(parent), m_compareType(LinkBackFilter::Equal), m_compareField(LinkBackFilter::FullPath), m_caseSensitivity(Qt::CaseInsensitive), m_invertFilterResult(false), m_filterMeansAccept(true), m_filterFiles(true), m_filterDirs(true), m_multiValued(false), m_values(nullptr), m_expressions(nullptr)
{
  // After this, the lists are guaranteed to exist.
  operator=(filter);
}

LinkBackFilter::LinkBackFilter(const LinkBackFilter& filter) :
  QObject(nullptr), m_compareType(LinkBackFilter::Equal), m_compareField(LinkBackFilter::FullPath), m_caseSensitivity(Qt::CaseInsensitive), m_invertFilterResult(false), m_filterMeansAccept(true), m_filterFiles(true), m_filterDirs(true), m_multiValued(false), m_values(nullptr), m_expressions(nullptr)
{
  // After this, the lists are guaranteed to exist.
  operator=(filter);
}


LinkBackFilter::~LinkBackFilter()
{
  clearLists(true, false);
}

void LinkBackFilter::clearLists(bool deleteLists, bool createIfDoNotExist)
{
  if (m_values != nullptr)
  {
    m_values->clear();
    if (deleteLists)
    {
      delete m_values;
      m_values = nullptr;
    }
  }
  if (m_expressions != nullptr)
  {
    qDeleteAll(*m_expressions);
    m_expressions->clear();
    if (deleteLists)
    {
      delete m_expressions;
      m_expressions = nullptr;
    }
  }
  if (createIfDoNotExist)
  {
    if (m_values == nullptr)
    {
      m_values = new QList<QVariant>();
    }
    if (m_expressions == nullptr)
    {
      m_expressions = new QList<QRegExp*>();
    }
  }
}

LinkBackFilter& LinkBackFilter::operator=(const LinkBackFilter& filter)
{
  if (this != & filter)
  {
    // set in this order so that multiple values will be cleared as needed.
    clearLists(false, true);
    m_multiValued = false;
    setValueDefault();
    setCaseSensitivity(filter.getCaseSensitivity());
    setInvertFilterResult(filter.isInvertFilterResult());
    setFilterMeansAccept(filter.isFilterMeansAccept());
    setCompareField(filter.getCompareField());
    setCompareType(filter.getCompareType());
    setFilterFiles(filter.isFilterFiles());
    setFilterDirs(filter.isFilterDirs());
    setMultiValued(filter.isMultiValued());
    setValue(filter.getValue());
  }
  return *this;
}

LinkBackFilter* LinkBackFilter::clone(QObject *parent) const
{
  return new LinkBackFilter(*this, parent);
}


void LinkBackFilter::setCompareType(CompareType compareType)
{
    if (compareType == LinkBackFilter::RegularExpression) {
        compareType = LinkBackFilter::RegExpFull;
    }
  if (compareType != m_compareType)
  {
    m_compareType = compareType;
    emit compareTypeChanged(compareType);
  }
}

void LinkBackFilter::setCompareField(CompareField compareField)
{
  if (compareField != m_compareField)
  {
    m_compareField = compareField;
    emit compareFieldChanged(compareField);
  }
}

void LinkBackFilter::setCaseSensitivity(Qt::CaseSensitivity caseSensitivity)
{
  if (caseSensitivity != m_caseSensitivity)
  {
    m_caseSensitivity = caseSensitivity;
    for (int i=0; i<m_expressions->size(); ++i)
    {
      m_expressions->at(i)->setCaseSensitivity(m_caseSensitivity);
    }
    emit caseSensitivityChanged(caseSensitivity);
  }
}

void LinkBackFilter::setInvertFilterResult(bool invertFilterResult)
{
  if (invertFilterResult != m_invertFilterResult)
  {
    m_invertFilterResult = invertFilterResult;
    emit invertFilterResultChanged(invertFilterResult);
  }
}

bool LinkBackFilter::isMultiValued() const
{
  return m_multiValued;
}

void LinkBackFilter::setMultiValuedDefault()
{
  setMultiValued(false);
}

void LinkBackFilter::setMultiValued(bool multiValued)
{
  if (multiValued != m_multiValued)
  {
    m_multiValued = multiValued;
    clearLists(false, true);
    emit multiValuedChanged(multiValued);
  }
}

void LinkBackFilter::setFilterMeansAccept(bool filterMeansAccept)
{
  if (filterMeansAccept != m_filterMeansAccept)
  {
    m_filterMeansAccept = filterMeansAccept;
    emit filterMeansAcceptChanged(filterMeansAccept);
  }
}

bool LinkBackFilter::isFilterDirs() const
{
  return m_filterDirs;
}

void LinkBackFilter::setFilterDirs(bool filterDirs)
{
  if (m_filterDirs != filterDirs)
  {
    m_filterDirs = filterDirs;
    emit filterDirsChanged(m_filterDirs);
  }
}

void LinkBackFilter::setFilterDirsDefault()
{
  setFilterDirs();
}


bool LinkBackFilter::isFilterFiles() const
{
  return m_filterFiles;
}

void LinkBackFilter::setFilterFiles(bool filterFiles)
{
  if (m_filterFiles != filterFiles)
  {
    m_filterFiles = filterFiles;
    emit filterFilesChanged(m_filterFiles);
  }
}

void LinkBackFilter::setFilterFilesDefault()
{
  setFilterFiles();
}

void LinkBackFilter::setValue(const QVariant& value)
{
  if (value != m_value)
  {
    m_value = value;
    createLists();
    emit valueChanged(m_value);
  }
}

void LinkBackFilter::createLists()
{
  clearLists(false, true);
  if (!isMultiValued() || m_value.type() != QVariant::String)
  {
    m_values->append(m_value);
  }
  else
  {
    QStringList list = m_value.toString().split(',', QString::SkipEmptyParts);
    foreach (QString s, list)
    {
      m_values->append(QVariant(s));
    }
  }
  createRegularExpressions();
}

void LinkBackFilter::createRegularExpressions()
{
  foreach (QVariant value, *m_values)
  {
    if (value.isValid() && !value.isNull())
    {
      if (value.type() == QVariant::RegExp)
      {
        m_expressions->append(new QRegExp(value.toRegExp()));
      }
      else if (m_compareType == LinkBackFilter::RegularExpression || m_compareType == LinkBackFilter::RegExpFull || m_compareType == LinkBackFilter::RegExpPartial)
      {
        m_expressions->append(new QRegExp(value.toString(), m_caseSensitivity, QRegExp::RegExp2));
      }
      else if (m_compareType == LinkBackFilter::FileSpec)
      {
        m_expressions->append(new QRegExp(value.toString(), m_caseSensitivity, QRegExp::WildcardUnix));
      }
      else
      {
        // Ignore because the type is neither a regular expression, nor a file spec.
      }
    }
  }
}

void LinkBackFilter::setValueDefault()
{
  switch (m_compareField)
  {
  case LinkBackFilter::Date:
    setValue(QDate::currentDate());
    break;
  case LinkBackFilter::DateTime:
    setValue(QDateTime::currentDateTime());
    break;
  case LinkBackFilter::Name:
  case LinkBackFilter::FullPath:
  case LinkBackFilter::PathOnly:
    setValue(QString());
    break;
  case LinkBackFilter::Size:
    setValue(0);
    break;
  case LinkBackFilter::Time:
    setValue(QTime::currentTime());
    break;
  default:
    setValue(QString());
    break;
  }
}

bool LinkBackFilter::applicable(const QFileInfo& fileInfo) const
{
  if (isFilterFiles() && fileInfo.isFile())
  {
    return true;
  }
  if (isFilterDirs() && fileInfo.isDir())
  {
    return true;
  }
  return false;
}

bool LinkBackFilter::passes(const QFileInfo& fileInfo) const
{
  if (!applicable(fileInfo))
  {
    TRACE_MSG(QString("Filter (%1) not applicable").arg(getMainValueAsString()), 5);
    return false;
  }

  bool filterPass = false;

  switch (m_compareField)
  {
  case LinkBackFilter::Date:
    filterPass = compareValues(fileInfo.lastModified().date());
    break;
  case LinkBackFilter::DateTime:
    filterPass = compareValues(fileInfo.lastModified());
    break;
  case LinkBackFilter::Name:
    filterPass = compareValues(fileInfo.fileName());
    break;
  case LinkBackFilter::FullPath:
    filterPass = compareValues(fileInfo.canonicalFilePath());
    break;
  case LinkBackFilter::PathOnly:
    filterPass = compareValues(fileInfo.canonicalPath());
    break;
  case LinkBackFilter::Size:
    filterPass = compareValues(fileInfo.size());
    break;
  case LinkBackFilter::Time:
    filterPass = compareValues(fileInfo.lastModified().time());
    break;
  default:
    // An invalid compare will never pass, even for an inverted filter.
    qDebug() << "Invalid compare field";
    return false;
    break;
  }

  return isInvertFilterResult() ? !filterPass : filterPass;
}

bool LinkBackFilter::compareValues(const qlonglong aSize) const
{
  switch (m_compareType)
  {
  case LinkBackFilter::RegExpFull:
  case LinkBackFilter::RegularExpression:
  case LinkBackFilter::FileSpec:
    foreach (QRegExp* expression, *m_expressions)
    {
      if ((expression != nullptr) && expression->exactMatch(QString::number(aSize)))
      {
        return true;
      }
    }
    break;
  case LinkBackFilter::RegExpPartial:
    foreach (QRegExp* expression, *m_expressions)
    {
      if ((expression != nullptr) && (expression->indexIn(QString::number(aSize)) >= 0))
      {
        return true;
      }
    }
    break;
  default:
    foreach (QVariant v, *m_values)
    {
      switch (m_compareType)
      {
      case LinkBackFilter::Less:
        if (aSize < v.toLongLong())
        {
          return true;
        }
        break;
      case LinkBackFilter::LessEqual:
        if (aSize <= v.toLongLong())
        {
          return true;
        }
        break;
      case LinkBackFilter::Equal:
        if (aSize == v.toLongLong())
        {
          return true;
        }
        break;
      case LinkBackFilter::GreaterEqual:
        if (aSize >= v.toLongLong())
        {
          return true;
        }
        break;
      case LinkBackFilter::Greater:
        if (aSize > v.toLongLong())
        {
          return true;
        }
        break;
      case LinkBackFilter::NotEqual:
        if (aSize != v.toLongLong())
        {
          return true;
        }
        break;
      case LinkBackFilter::Contains:
        if (QString::number(aSize).contains(v.toString(), m_caseSensitivity))
        {
          return true;
        }
        break;
      default:
        break;
      }
    }
  }

  return false;
}


bool LinkBackFilter::compareValues(const QTime& aTime) const
{
  switch (m_compareType)
  {
  case LinkBackFilter::RegExpFull:
  case LinkBackFilter::RegularExpression:
  case LinkBackFilter::FileSpec:
    foreach (QRegExp* expression, *m_expressions)
    {
      if ((expression != nullptr) && expression->exactMatch(aTime.toString(Qt::TextDate)))
      {
        return true;
      }
    }
    break;
  case LinkBackFilter::RegExpPartial:
    foreach (QRegExp* expression, *m_expressions)
    {
      if ((expression != nullptr) && (expression->indexIn(aTime.toString(Qt::TextDate)) >= 0))
      {
        return true;
      }
    }
    break;
  default:
    foreach (QVariant v, *m_values)
    {
      switch (m_compareType)
      {
      case LinkBackFilter::Less:
        if (aTime < v.toTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::LessEqual:
        if (aTime <= v.toTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::Equal:
        if (aTime == v.toTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::GreaterEqual:
        if (aTime >= v.toTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::Greater:
        if (aTime > v.toTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::NotEqual:
        if (aTime != v.toTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::Contains:
        if (aTime.toString(Qt::TextDate).contains(v.toString(), m_caseSensitivity))
        {
          return true;
        }
        break;
      default:
        break;
      }
    }
  }
  return false;
}

bool LinkBackFilter::compareValues(const QDate& aDate) const
{
  switch (m_compareType)
  {
  case LinkBackFilter::RegExpFull:
  case LinkBackFilter::RegularExpression:
  case LinkBackFilter::FileSpec:
    foreach (QRegExp* expression, *m_expressions)
    {
      if ((expression != nullptr) && expression->exactMatch(aDate.toString(Qt::TextDate)))
      {
        return true;
      }
    }
    break;
  case LinkBackFilter::RegExpPartial:
    foreach (QRegExp* expression, *m_expressions)
    {
      if ((expression != nullptr) && (expression->indexIn(aDate.toString(Qt::TextDate)) >= 0))
      {
        return true;
      }
    }
    break;
  default:
    foreach (QVariant v, *m_values)
    {
      switch (m_compareType)
      {
      case LinkBackFilter::Less:
        if (aDate < v.toDate())
        {
          return true;
        }
        break;
      case LinkBackFilter::LessEqual:
        if (aDate <= v.toDate())
        {
          return true;
        }
        break;
      case LinkBackFilter::Equal:
        if (aDate == v.toDate())
        {
          return true;
        }
        break;
      case LinkBackFilter::GreaterEqual:
        if (aDate >= v.toDate())
        {
          return true;
        }
        break;
      case LinkBackFilter::Greater:
        if (aDate > v.toDate())
        {
          return true;
        }
        break;
      case LinkBackFilter::NotEqual:
        if (aDate != v.toDate())
        {
          return true;
        }
        break;
      case LinkBackFilter::Contains:
        if (aDate.toString(Qt::TextDate).contains(v.toString(), m_caseSensitivity))
        {
          return true;
        }
        break;
      default:
        break;
      }
    }
  }
  return false;
}

bool LinkBackFilter::compareValues(const QDateTime& aDateTime) const
{
  switch (m_compareType)
  {
  case LinkBackFilter::RegExpFull:
  case LinkBackFilter::RegularExpression:
  case LinkBackFilter::FileSpec:
    foreach (QRegExp* expression, *m_expressions)
    {
      if ((expression != nullptr) && expression->exactMatch(aDateTime.toString(Qt::TextDate)))
      {
        return true;
      }
    }
    break;
  case LinkBackFilter::RegExpPartial:
    foreach (QRegExp* expression, *m_expressions)
    {
      if ((expression != nullptr) && (expression->indexIn(aDateTime.toString(Qt::TextDate)) >= 0))
      {
        return true;
      }
    }
    break;
  default:
    foreach (QVariant v, *m_values)
    {
      switch (m_compareType)
      {
      case LinkBackFilter::Less:
        if (aDateTime < v.toDateTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::LessEqual:
        if (aDateTime <= v.toDateTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::Equal:
        if (aDateTime == v.toDateTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::GreaterEqual:
        if (aDateTime >= v.toDateTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::Greater:
        if (aDateTime > v.toDateTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::NotEqual:
        if (aDateTime != v.toDateTime())
        {
          return true;
        }
        break;
      case LinkBackFilter::Contains:
        if (aDateTime.toString(Qt::TextDate).contains(v.toString(), m_caseSensitivity))
        {
          return true;
        }
        break;
      default:
        break;
      }
    }
  }
  return false;
}

bool LinkBackFilter::compareValues(const QString& filePortion) const
{
  int i;
  switch (m_compareType)
  {
  case LinkBackFilter::RegExpFull:
  case LinkBackFilter::RegularExpression:
  case LinkBackFilter::FileSpec:
    for (i=0; i<m_expressions->size(); ++i)
    {
      QRegExp* expression = m_expressions->at(i);
      TRACE_MSG(QString("Checking fileportion (%1) against (%2)").arg(filePortion, m_values->at(i).toString()), 6);
      if ((expression != nullptr) && expression->exactMatch(filePortion))
      {
        TRACE_MSG(QString("PASSED fileportion (%1) against (%2)").arg(filePortion, m_values->at(i).toString()), 5);
        return true;
      }
      else
      {
        TRACE_MSG(QString("FAILED fileportion (%1) against (%2)").arg(filePortion, m_values->at(i).toString()), 5);
      }
    }
    break;
  case LinkBackFilter::RegExpPartial:
    foreach (QRegExp* expression, *m_expressions)
    {
      if ((expression != nullptr) && (expression->indexIn(filePortion) >= 0))
      {
        return true;
      }
    }
    break;
  default:
    foreach (QVariant v, *m_values)
    {
      switch (m_compareType)
      {
      case LinkBackFilter::Less:
        if (QString::compare(filePortion, v.toString(), m_caseSensitivity) < 0)
        {
          return true;
        }
        break;
      case LinkBackFilter::LessEqual:
        if (QString::compare(filePortion, v.toString(), m_caseSensitivity) <= 0)
        {
          return true;
        }
        break;
      case LinkBackFilter::Equal:
        if (QString::compare(filePortion, v.toString(), m_caseSensitivity) == 0)
        {
          return true;
        }
        break;
      case LinkBackFilter::GreaterEqual:
        if (QString::compare(filePortion, v.toString(), m_caseSensitivity) >= 0)
        {
          return true;
        }
        break;
      case LinkBackFilter::Greater:
        if (QString::compare(filePortion, v.toString(), m_caseSensitivity) > 0)
        {
          return true;
        }
        break;
      case LinkBackFilter::NotEqual:
        if (QString::compare(filePortion, v.toString(), m_caseSensitivity) != 0)
        {
          return true;
        }
        break;
      case LinkBackFilter::Contains:
        if (filePortion.contains(v.toString(), m_caseSensitivity))
        {
          return true;
        }
        break;
      default:
        break;
      }
    }
  }
  return false;
}

QString LinkBackFilter::getCompareTypeAsString() const
{
  const QMetaObject* metaObj = metaObject();
  return  metaObj->enumerator(metaObj->indexOfEnumerator("CompareType")).valueToKey(getCompareType());
}

QString LinkBackFilter::getCompareFieldAsString() const
{
  const QMetaObject* metaObj = metaObject();
  return  metaObj->enumerator(metaObj->indexOfEnumerator("CompareField")).valueToKey(getCompareField());
}

QXmlStreamWriter& LinkBackFilter::operator<<(QXmlStreamWriter& writer) const
{
  writer.writeStartElement("Filter");
  writer.writeTextElement("CompareType", getCompareTypeAsString());
  writer.writeTextElement("CompareField", getCompareFieldAsString());
  writer.writeTextElement("Value", m_value.toString());
  writer.writeTextElement("CaseSensitive", (m_caseSensitivity==Qt::CaseSensitive) ? "True" : "False");
  writer.writeTextElement("Invert", m_invertFilterResult ? "True" : "False");
  writer.writeTextElement("Accept", m_filterMeansAccept ? "True" : "False");
  writer.writeTextElement("Files", m_filterFiles ? "True" : "False");
  writer.writeTextElement("MultiValued", m_multiValued ? "True" : "False");
  writer.writeTextElement("Directories", m_filterDirs ? "True" : "False");
  writer.writeEndElement();
  return writer;
}

void LinkBackFilter::setAllDefault()
{
  setValueDefault();
  setFilterFilesDefault();
  setFilterDirsDefault();
  setFilterMeansAcceptDefault();
  setInvertFilterResultDefault();
  setCaseSensitivityDefault();
  setCompareFieldDefault();
  setCompareTypeDefault();
  setMultiValuedDefault();
}

QXmlStreamReader& LinkBackFilter::operator>>(QXmlStreamReader& reader)
{
  return readFilter(reader);
}

QXmlStreamReader&  LinkBackFilter::readFilter(QXmlStreamReader& reader)
{
  setAllDefault();
  QString version = "1";
  QString name;
  while (!reader.atEnd()) {
    if (reader.isStartElement()) {
      name = reader.name().toString();
      if (QString::compare(name, "Filter", Qt::CaseInsensitive) == 0) {
        if (reader.attributes().hasAttribute("Version"))
        {
          version = reader.attributes().value("Version").toString();
        }
        readInternals(reader, version);
        return reader;
      } else {
        reader.raiseError(QObject::tr("Not a Filter"));
      }
    } else if (reader.isEndElement()) {
      return reader;
    }
    reader.readNext();
  }
  return reader;
}

void LinkBackFilter::readInternals(QXmlStreamReader& reader, const QString&)
{
  const QMetaObject* metaObj = metaObject();
  QString name;
  while (!reader.atEnd()) {
    if (reader.isStartElement()) {
      name = reader.name().toString();
    } else if (reader.isCharacters()) {
      QString value = reader.text().toString();
      if (name.isEmpty()) {
        // Ignore an empty name
      } else if (QString::compare(name, "CompareType", Qt::CaseInsensitive) == 0) {
        if (value.isEmpty()) {
          setCompareTypeDefault();
        } else {
          //TRACE_MSG(QString("Setting CompareType (%1) for filter").arg(value), 1);
          // ??? indexIn >= 0 is a partial match.
          setCompareType(static_cast<LinkBackFilter::CompareType>(metaObj->enumerator(metaObj->indexOfEnumerator("CompareType")).keyToValue(qPrintable(value))));
        }
      } else if (QString::compare(name, "CompareField", Qt::CaseInsensitive) == 0) {
        if (value.isEmpty()) {
          setCompareFieldDefault();
        } else {
          //TRACE_MSG(QString("Setting CompareField (%1) for filter").arg(value), 1);
          setCompareField(static_cast<LinkBackFilter::CompareField>(metaObj->enumerator(metaObj->indexOfEnumerator("CompareField")).keyToValue(qPrintable(value))));
        }
      } else if (QString::compare(name, "Value", Qt::CaseInsensitive) == 0) {
        setValue(value);
      } else if (QString::compare(name, "CaseSensitive", Qt::CaseInsensitive) == 0) {
        if (value.isEmpty()) {
          setCaseSensitivityDefault();
        } else {
          setCaseSensitivity(QString::compare(value, "True") == 0 ? Qt::CaseSensitive : Qt::CaseInsensitive);
        }
      } else if (QString::compare(name, "Invert", Qt::CaseInsensitive) == 0) {
        if (value.isEmpty()) {
          setInvertFilterResultDefault();
        } else {
          setInvertFilterResult(QString::compare(value, "True") == 0);
        }
      } else if (QString::compare(name, "MultiValued", Qt::CaseInsensitive) == 0) {
        if (value.isEmpty()) {
          setMultiValuedDefault();
        } else {
          TRACE_MSG(QString("Setting Multi-Valued (%1) for filter").arg(QString::compare(value, "True") == 0), 5);
          setMultiValued(QString::compare(value, "True") == 0);
        }
      } else if (QString::compare(name, "Accept", Qt::CaseInsensitive) == 0) {
        if (value.isEmpty()) {
          setFilterMeansAcceptDefault();
        } else {
          setFilterMeansAccept(QString::compare(value, "True") == 0);
        }
      } else if (QString::compare(name, "Files", Qt::CaseInsensitive) == 0) {
        if (value.isEmpty()) {
          setFilterFilesDefault();
        } else {
          setFilterFiles(QString::compare(value, "True") == 0);
        }
      } else if (QString::compare(name, "Directories", Qt::CaseInsensitive) == 0) {
        if (value.isEmpty()) {
          setFilterDirsDefault();
        } else {
          setFilterDirs(QString::compare(value, "True") == 0);
        }
      } else if (QString::compare(name, "Filter", Qt::CaseInsensitive) == 0) {
      } else {
        reader.raiseError(QObject::tr("Unrecognized Filter Element %1").arg(name));
      }
    } else if (reader.isEndElement()) {
      if (QString::compare(reader.name().toString(), "Filter", Qt::CaseInsensitive) == 0)
      {
        return;
      }
      name = "";
    }
    reader.readNext();
  }
}
