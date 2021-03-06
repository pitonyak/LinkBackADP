#include "simpleloggerroutinginfo.h"
#include "xmlutility.h"
#include <QMetaEnum>
#include <QMetaObject>
#include <QMapIterator>

SimpleLoggerRoutingInfo SimpleLoggerRoutingInfo::privateObjectForMetaData;

SimpleLoggerRoutingInfo::SimpleLoggerRoutingInfo(QObject *parent) :
  QObject(parent), m_levels(nullptr), m_routing(nullptr), m_locationRegExp(nullptr), m_messageRegExp(nullptr),
  m_locRegExpCaseSensitivity(Qt::CaseInsensitive), m_messageRegExpCaseSensitivity(Qt::CaseInsensitive),
  m_enabled(true)
{
  const QMetaObject* metaObj = metaObject();
  QMetaEnum metaEnum = metaObj->enumerator(metaObj->indexOfEnumerator("MessageCategory"));

  m_levels = new QMap<MessageCategory, int>();
  for (int i=0; i<metaEnum.keyCount(); ++i)
  {
    m_levels->insert(static_cast<MessageCategory>(metaEnum.value(i)), 0);
  }

  metaEnum = metaObj->enumerator(metaObj->indexOfEnumerator("MessageRouting"));
  m_routing = new QMap<MessageRouting, bool>();
  for (int i=0; i<metaEnum.keyCount(); ++i)
  {
    m_routing->insert(static_cast<MessageRouting>(metaEnum.value(i)), false);
  }
}

SimpleLoggerRoutingInfo::SimpleLoggerRoutingInfo(const SimpleLoggerRoutingInfo& obj, QObject *parent) :
  QObject(parent), m_levels(nullptr), m_routing(nullptr), m_locationRegExp(nullptr), m_messageRegExp(nullptr),
  m_locRegExpCaseSensitivity(Qt::CaseInsensitive), m_messageRegExpCaseSensitivity(Qt::CaseInsensitive),
  m_enabled(true)
{
  m_levels = new QMap<MessageCategory, int>();
  m_routing = new QMap<MessageRouting, bool>();
  copy(obj);
}


SimpleLoggerRoutingInfo::~SimpleLoggerRoutingInfo()
{
  internalDelete();
}

void SimpleLoggerRoutingInfo::internalClear()
{
  if (m_locationRegExp != nullptr)
  {
    delete m_locationRegExp;
    m_locationRegExp = nullptr;
  }
  if (m_messageRegExp != nullptr)
  {
    delete m_messageRegExp;
    m_messageRegExp = nullptr;
  }
  if (m_levels != nullptr)
  {
    m_levels->clear();
  }
  if (m_routing != nullptr)
  {
    m_routing->clear();
  }
  m_format.clear();
}

void SimpleLoggerRoutingInfo::internalDelete()
{
  if (m_locationRegExp != nullptr)
  {
    delete m_locationRegExp;
    m_locationRegExp = nullptr;
  }
  if (m_messageRegExp != nullptr)
  {
    delete m_messageRegExp;
    m_messageRegExp = nullptr;
  }
  if (m_levels != nullptr)
  {
    delete m_levels;
    m_levels = nullptr;
  }
  if (m_routing != nullptr)
  {
    delete m_routing;
    m_routing = nullptr;
  }
}


bool SimpleLoggerRoutingInfo::isEnabled() const
{
  return m_enabled;
}

void SimpleLoggerRoutingInfo::setEnabled(bool enabledState)
{
  m_enabled = enabledState;
}


bool SimpleLoggerRoutingInfo::isRoutingOn(MessageRouting messageRouting) const
{
  return m_routing->value(messageRouting, true);
}

void SimpleLoggerRoutingInfo::setRouting(MessageRoutings messageRoutings, bool state)
{
  const QMetaObject* metaObj = metaObject();
  QMetaEnum metaEnum = metaObj->enumerator(metaObj->indexOfEnumerator("MessageRouting"));

  for (int i=0; i<metaEnum.keyCount(); ++i)
  {
    MessageRouting messageRouting = static_cast<MessageRouting>(metaEnum.value(i));
    if ((messageRoutings & messageRouting) == messageRouting)
    {
      m_routing->insert(messageRouting, state);
    }
  }
}

void SimpleLoggerRoutingInfo::setCategoryLevel(MessageCategory category, int level)
{
  m_levels->insert(category, level);
}

int SimpleLoggerRoutingInfo::getCategoryLevel(MessageCategory category) const
{
  return m_levels->value(category, -1);
}

bool SimpleLoggerRoutingInfo::setLocationRegExp(const QString& regExp)
{
  if (regExp.length() == 0)
  {
    if (m_locationRegExp != nullptr)
    {
      delete m_locationRegExp;
      m_locationRegExp = nullptr;
    }
    return true;
  }

  QRegularExpression::PatternOptions po = (m_locationRegExp == nullptr) ? (m_locRegExpCaseSensitivity == Qt::CaseInsensitive ? QRegularExpression::CaseInsensitiveOption : QRegularExpression::NoPatternOption) : m_locationRegExp->patternOptions();

  QRegularExpression* newRegExpr = new QRegularExpression(regExp, po);

  if (!newRegExpr->isValid()) {
    delete newRegExpr;
    return false;
  }

  if (m_locationRegExp != nullptr)
  {
    delete m_locationRegExp;
  }
  m_locationRegExp = newRegExpr;
  return true;
}


QString SimpleLoggerRoutingInfo::getLocationRegExpString() const
{
  return (m_locationRegExp == nullptr) ? "" : m_locationRegExp->pattern();
}

Qt::CaseSensitivity SimpleLoggerRoutingInfo::getLocationRegExpCaseSensitivity() const
{
  return m_locRegExpCaseSensitivity;
}

void SimpleLoggerRoutingInfo::setLocationRegExpCaseSensitivity(const Qt::CaseSensitivity cs)
{
  if (cs != m_locRegExpCaseSensitivity)
  {
    m_locRegExpCaseSensitivity = cs;
    if (m_locationRegExp != nullptr)
    {
      if (cs == Qt::CaseInsensitive) {
        m_locationRegExp->setPatternOptions(m_locationRegExp->patternOptions() | QRegularExpression::CaseInsensitiveOption);
      } else {
        m_locationRegExp->setPatternOptions(m_locationRegExp->patternOptions() & ~QRegularExpression::CaseInsensitiveOption);
      }
    }
  }
}

bool SimpleLoggerRoutingInfo::setMessageRegExp(const QString& regExp)
{  
  if (regExp.length() == 0)
  {
    if (m_messageRegExp != nullptr)
    {
      delete m_messageRegExp;
      m_messageRegExp = nullptr;
    }
    return true;
  }

  QRegularExpression::PatternOptions po = (m_messageRegExp == nullptr) ? (m_messageRegExpCaseSensitivity == Qt::CaseInsensitive ? QRegularExpression::CaseInsensitiveOption : QRegularExpression::NoPatternOption) : m_messageRegExp->patternOptions();

  QRegularExpression* newRegExpr = new QRegularExpression(regExp, po);

  if (!newRegExpr->isValid()) {
    delete newRegExpr;
    return false;
  }

  if (m_messageRegExp != nullptr)
  {
    delete m_messageRegExp;
  }
  m_messageRegExp = newRegExpr;
  return true;


}

QString SimpleLoggerRoutingInfo::getMessageRegExpString() const
{
  return (m_messageRegExp == nullptr) ? "" : m_messageRegExp->pattern();
}

Qt::CaseSensitivity SimpleLoggerRoutingInfo::getMessageRegExpCaseSensitivity() const
{
  return m_messageRegExpCaseSensitivity;
}

void SimpleLoggerRoutingInfo::setMessageRegExpCaseSensitivity(const Qt::CaseSensitivity cs)
{
  if (cs != m_messageRegExpCaseSensitivity)
  {
    m_messageRegExpCaseSensitivity = cs;
    if (m_messageRegExp != nullptr)
    {
      if (cs == Qt::CaseInsensitive) {
        m_messageRegExp->setPatternOptions(m_locationRegExp->patternOptions() | QRegularExpression::CaseInsensitiveOption);
      } else {
        m_messageRegExp->setPatternOptions(m_locationRegExp->patternOptions() & ~QRegularExpression::CaseInsensitiveOption);
      }
    }
  }
}

bool SimpleLoggerRoutingInfo::passes(const QString& source, const MessageCategory& category, int level, const QString &message) const
{
  bool rc = m_enabled && level <= m_levels->value(category, 0) && source.length() > 0 && (m_locationRegExp == nullptr || m_locationRegExp->match(source).hasMatch()) && (m_messageRegExp == nullptr || m_messageRegExp->match(message).hasMatch());
  return rc;
}

void SimpleLoggerRoutingInfo::clearMessageFormat()
{
  m_format.clear();
}

void SimpleLoggerRoutingInfo::addMessageFormat(MessageComponent component, const QString& formatString)
{
  m_format.append(QPair<MessageComponent, QString>(component, formatString));
}

QString SimpleLoggerRoutingInfo::formatMessage(const QString& message, const QString& location, const QDateTime dateTime, MessageCategory category, int) const
{
  QString s;
  for (int i=0; i<m_format.length(); ++i)
  {
    const QPair<MessageComponent, QString>& pair(m_format[i]);
    switch(pair.first)
    {
    case MessageDateTime:
      if (pair.second.length() == 0)
      {
        s.append(dateTime.toString(Qt::ISODate));
      }
      else
      {
        s.append(dateTime.toString(pair.second));
      }
      break;
    case MessageType:
      s.append(categoryToString(category, pair.second.length()));
      break;
    case MessageText:
      s.append(message);
      break;
    case MessageLocation:
      // The file macro used to get the location sometimes contains path information.
      // This is related to the target build directory, so if the object files are not
      // in the same directory as the source files, a path like "../LinkBackAPD/helper.cpp" is used.
      // The following code checks for this and removes the path information.
      if (location.contains('/'))
      {
        s.append(location.right(location.length() - location.lastIndexOf('/') - 1));
      }
      else if (location.contains('\\'))
      {
        s.append(location.right(location.length() - location.lastIndexOf('\\') - 1));
      }
      else
      {
        s.append(location);
      }
      break;
    case ConstantText:
      s.append(pair.second);
      break;
    default:
      // TODO: Error handling
      break;
    }
  }
  return s;
}

const SimpleLoggerRoutingInfo& SimpleLoggerRoutingInfo::copy(const SimpleLoggerRoutingInfo& obj)
{
  return operator=(obj);
}

SimpleLoggerRoutingInfo& SimpleLoggerRoutingInfo::operator=(const SimpleLoggerRoutingInfo& obj)
{
  if (this != &obj)
  {
    m_format.clear();
    m_levels->clear();
    m_routing->clear();
    m_format.append(obj.m_format);

    if (m_locationRegExp != nullptr)
    {
      delete m_locationRegExp;
      m_locationRegExp = nullptr;
    }
    if (obj.m_locationRegExp != nullptr)
    {
      m_locationRegExp = new QRegularExpression(*(obj.m_locationRegExp));
    }

    if (m_messageRegExp != nullptr)
    {
      delete m_messageRegExp;
      m_messageRegExp = nullptr;
    }
    if (obj.m_messageRegExp != nullptr)
    {
      m_messageRegExp = new QRegularExpression(*(obj.m_messageRegExp));
    }

    *m_levels = *obj.m_levels;
    *m_routing = *obj.m_routing;
    m_enabled = obj.m_enabled;
    m_name = obj.m_name;
  }
  return *this;
}

QString SimpleLoggerRoutingInfo::categoryToString(MessageCategory category, const int maxlen, const bool brief)
{
  const QMetaObject* metaObj = privateObjectForMetaData.metaObject();
  const QMetaEnum& categoryEnum =  metaObj->enumerator(metaObj->indexOfEnumerator("MessageCategory"));
  QString s = categoryEnum.valueToKey(category);
  if (brief && s.length() > 7)
  {
    s = s.left(s.length() - 7);
  }
  return (maxlen <= 0) ? s : (s.isRightToLeft() ? s.right(maxlen) : s.left(maxlen));
}

QString SimpleLoggerRoutingInfo::componentToString(MessageComponent component, const int maxlen)
{
  const QMetaObject* metaObj = privateObjectForMetaData.metaObject();
  const QMetaEnum& componentEnum =  metaObj->enumerator(metaObj->indexOfEnumerator("MessageComponent"));
  QString s = componentEnum.valueToKey(component);
  return (maxlen <= 0) ? s : (s.isRightToLeft() ? s.right(maxlen) : s.left(maxlen));
}


QStringList SimpleLoggerRoutingInfo::getMessageRoutingStrings()
{
  QStringList qsl;
  const QMetaObject* metaObj = privateObjectForMetaData.metaObject();
  const QMetaEnum& componentEnum =  metaObj->enumerator(metaObj->indexOfEnumerator("MessageComponent"));
  for (int i=0; i<componentEnum.keyCount(); ++i)
  {
    qsl << componentEnum.valueToKey(i);
  }
  return qsl;
}

QString SimpleLoggerRoutingInfo::routingToString(MessageRouting routing, const int maxlen)
{
  const QMetaObject* metaObj = privateObjectForMetaData.metaObject();
  const QMetaEnum& routingEnum =  metaObj->enumerator(metaObj->indexOfEnumerator("MessageRouting"));
  QString s = routingEnum.valueToKey(routing);
  return (maxlen <= 0) ? s : (s.isRightToLeft() ? s.right(maxlen) : s.left(maxlen));
}


SimpleLoggerRoutingInfo::MessageCategory SimpleLoggerRoutingInfo::stringToCategory(const QString& category)
{
  const QMetaObject* metaObj = privateObjectForMetaData.metaObject();
  const QMetaEnum& categoryEnum =  metaObj->enumerator(metaObj->indexOfEnumerator("MessageCategory"));
  return static_cast<SimpleLoggerRoutingInfo::MessageCategory>(categoryEnum.keyToValue(qPrintable(category)));
}

SimpleLoggerRoutingInfo::MessageComponent SimpleLoggerRoutingInfo::stringToComponent(const QString& component)
{
  const QMetaObject* metaObj = privateObjectForMetaData.metaObject();
  const QMetaEnum& categoryEnum =  metaObj->enumerator(metaObj->indexOfEnumerator("MessageComponent"));
  return static_cast<SimpleLoggerRoutingInfo::MessageComponent>(categoryEnum.keyToValue(qPrintable(component)));
}

SimpleLoggerRoutingInfo::MessageRouting SimpleLoggerRoutingInfo::stringToRouting(const QString& routing)
{
  const QMetaObject* metaObj = privateObjectForMetaData.metaObject();
  const QMetaEnum& categoryEnum =  metaObj->enumerator(metaObj->indexOfEnumerator("MessageRouting"));
  return static_cast<SimpleLoggerRoutingInfo::MessageRouting>(categoryEnum.keyToValue(qPrintable(routing)));
}

QXmlStreamWriter& SimpleLoggerRoutingInfo::write(QXmlStreamWriter& writer) const
{
  writer.writeStartElement("SimpleLoggerRoutingInfo");

  if (m_name.length() > 0)
  {
    XMLUtility::writeElement(writer, "Name", m_name);
  }
  XMLUtility::writeElement(writer, "Enabled", XMLUtility::booleanToString(m_enabled));

  if (m_levels != nullptr && m_levels->size() > 0)
  {
    QMap<MessageCategory, int>::const_iterator i;
    for (i = m_levels->begin(); i != m_levels->end(); ++i)
    {
      XMLUtility::writeElementAttribute(writer, "Level", QString::number(i.value()), "MessageCategory", categoryToString(i.key(), -1));
    }
  }

  if (m_routing != nullptr && m_routing->size() > 0)
  {
    QMap<MessageRouting, bool>::const_iterator i;
    for (i = m_routing->begin(); i != m_routing->end(); ++i)
    {
      XMLUtility::writeElementAttribute(writer, "Routing", XMLUtility::booleanToString(i.value()), "MessageRouting", routingToString(i.key(), -1));
    }
  }

  if (m_locationRegExp != nullptr)
  {
    XMLUtility::write(writer, *m_locationRegExp, "LocationRegEx");
  }

  if (m_messageRegExp != nullptr)
  {
    XMLUtility::write(writer, *m_messageRegExp, "MessageRegEx");
  }

  if (m_format.size() > 0)
  {
    for (int i=0; i<m_format.size(); ++i)
    {
      const QPair<MessageComponent, QString>& comp = m_format.at(i);
      XMLUtility::writeElementAttribute(writer, "Format", comp.second, "MessageComponent", componentToString(comp.first, comp.second.length()));
    }
  }

  writer.writeEndElement();
  return writer;
}

QXmlStreamReader& SimpleLoggerRoutingInfo::read(QXmlStreamReader& reader)
{
  internalClear();
  QString name;
  while (!reader.atEnd()) {
    if (reader.isStartElement()) {
      name = reader.name().toString();
      if (QString::compare(name, "SimpleLoggerRoutingInfo", Qt::CaseInsensitive) == 0) {
        readInternals(reader, "1");
        return reader;
      } else {
        reader.raiseError(QObject::tr("Not a SimpleLoggerRoutingInfo"));
      }
    } else if (reader.isEndElement()) {
      return reader;
    }
    reader.readNext();
  }

  return reader;
}

void SimpleLoggerRoutingInfo::readInternals(QXmlStreamReader& reader, const QString&)
{
  // Some elements store a map with the key as an attribute such as: "<Level MessageCategory="UserMessage">3</Level>"
  // The nameAttrMapprovides the mapping between the element name and the single attribute used to contain the key.
  QMap<QString, QString> nameAttrMap;
  nameAttrMap["level"] = "MessageCategory";
  nameAttrMap["routing"] = "MessageRouting";
  nameAttrMap["format"] = "MessageComponent";

  QString name;
  QString attributeValue;
  bool foundCharacters = false;
  while (!reader.atEnd())
  {
    if (reader.isStartElement())
    {
      attributeValue = "";
      name = reader.name().toString();
      foundCharacters = false;

      if (QString::compare(name, "LocationRegEx", Qt::CaseInsensitive) == 0)
      {
        if (m_locationRegExp != nullptr)
        {
          delete m_locationRegExp;
          m_locationRegExp = nullptr;
        }
        m_locationRegExp = XMLUtility::readRegExp(reader);
      }
      else if (QString::compare(name, "MessageRegEx", Qt::CaseInsensitive) == 0)
      {
        if (m_messageRegExp != nullptr)
        {
          delete m_messageRegExp;
          m_messageRegExp = nullptr;
        }
        m_messageRegExp = XMLUtility::readRegExp(reader);
      }
      else
      {
        // At a start element that is not a regular expression, so check for the value in the attribute.
        if (nameAttrMap.contains(name.toLower()) && reader.attributes().hasAttribute(nameAttrMap[name.toLower()]))
        {
          attributeValue = reader.attributes().value(nameAttrMap[name.toLower()]).toString();
        }
      }
    }
    else if (reader.isCharacters() && !name.isEmpty())
    {
      QString value = reader.text().toString();
      if (name.compare("Enabled", Qt::CaseInsensitive) == 0)
      {
        m_enabled = XMLUtility::stringToBoolean(value);
      }
      else if (name.compare("Name", Qt::CaseInsensitive) == 0)
      {
        setName(value.trimmed());
      }
      else if (attributeValue.length() > 0)
      {
        // These next guys store their value in the attribute as specified in the nameAttrMap.
        if (name.compare("Level", Qt::CaseInsensitive) == 0)
        {
          m_levels->insert(stringToCategory(attributeValue), value.toInt());
        }
        else if (name.compare("Routing", Qt::CaseInsensitive) == 0)
        {
          m_routing->insert(stringToRouting(attributeValue), XMLUtility::stringToBoolean(value));
        }
        else if (name.compare("Format", Qt::CaseInsensitive) == 0)
        {
          m_format.append(QPair<MessageComponent, QString>(stringToComponent(attributeValue), value));
          foundCharacters = true;
        }
      }
      attributeValue = "";
    } else if (reader.isEndElement()) {
      if (QString::compare(reader.name().toString(), "SimpleLoggerRoutingInfo", Qt::CaseInsensitive) == 0)
      {
        return;
      }
      if (name.compare("Format", Qt::CaseInsensitive) == 0 && !foundCharacters)
      {
        m_format.append(QPair<MessageComponent, QString>(stringToComponent(attributeValue), ""));
      }
      name = "";
    }
    reader.readNext();
  }
}


QString SimpleLoggerRoutingInfo::getLevelsAsString() const
{
  QString s = "";
  QMapIterator<MessageCategory, int> i(*m_levels);
  while (i.hasNext())
  {
      i.next();
      if (s.length() > 0)
      {
        s = s + QString("|%1:%2").arg(categoryToString(i.key(), 1)).arg(i.value());
      }
      else
      {
        s = QString("%1:%2").arg(categoryToString(i.key(), 1)).arg(i.value());
      }
  }
  return s;
}
