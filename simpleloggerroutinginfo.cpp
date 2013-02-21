#include "simpleloggerroutinginfo.h"
#include "xmlutility.h"
#include <QRegExp>
#include <QMetaEnum>
#include <QMetaObject>

SimpleLoggerRoutingInfo SimpleLoggerRoutingInfo::privateObjectForMetaData;

SimpleLoggerRoutingInfo::SimpleLoggerRoutingInfo(QObject *parent) : QObject(parent), m_levels(nullptr), m_routing(nullptr), m_regExp(nullptr), m_enabled(true)
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

SimpleLoggerRoutingInfo::SimpleLoggerRoutingInfo(const SimpleLoggerRoutingInfo& obj, QObject *parent) : QObject(parent), m_levels(nullptr), m_routing(nullptr), m_regExp(nullptr)
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
    if (m_regExp != nullptr)
    {
        delete m_regExp;
        m_regExp = nullptr;
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
    if (m_regExp != nullptr)
    {
        delete m_regExp;
        m_regExp = nullptr;
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

bool SimpleLoggerRoutingInfo::setRegExp(const QString& regExp)
{
    if (m_regExp != nullptr)
    {
        delete m_regExp;
        m_regExp = nullptr;
    }
    if (regExp.length() > 0)
    {
        m_regExp = new QRegExp(regExp, Qt::CaseInsensitive, QRegExp::RegExp2);
        if (!m_regExp->isValid())
        {
            delete m_regExp;
            m_regExp = nullptr;
            return false;
        }
    }
    return true;
}

bool SimpleLoggerRoutingInfo::passes(const QString& source, const MessageCategory& category, int level) const
{
    //qDebug(qPrintable(QString("Message level %1").arg(level)));
    //qDebug(qPrintable(QString("Contains %1").arg(m_levels->contains(category))));
    //int containedLevel = m_levels->value(category, 0);
    //qDebug(qPrintable(QString("Contained level %1").arg(containedLevel)));
    bool rc = m_enabled && level <= m_levels->value(category, 0) && source.length() > 0 && (m_regExp == nullptr || m_regExp->indexIn(source) >= 0);
    //qDebug(qPrintable(QString("Passes %1").arg(rc)));
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
        case DateTimeComponent:
            if (pair.second.length() == 0)
            {
                s.append(dateTime.toString(Qt::ISODate));
            }
            else
            {
                s.append(dateTime.toString(pair.second));
            }
            break;
        case MessageTypeComponent:
            s.append(categoryToString(category));
            break;
        case MessageTextComponent:
            s.append(message);
            break;
        case MessageLocationComponent:
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
        case MessageText:
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
    if (this != &obj)
    {
        m_format.clear();
        m_levels->clear();
        m_routing->clear();
        m_format.append(obj.m_format);

        if (m_regExp != nullptr)
        {
            delete m_regExp;
            m_regExp = nullptr;
        }
        if (obj.m_regExp != nullptr)
        {
            m_regExp = new QRegExp(*m_regExp);
        }
        *m_levels = *obj.m_levels;
        *m_routing = *obj.m_routing;
        m_enabled = obj.m_enabled;
    }
    return *this;
}

const SimpleLoggerRoutingInfo& SimpleLoggerRoutingInfo::operator=(const SimpleLoggerRoutingInfo& obj)
{
    return copy(obj);
}

QString SimpleLoggerRoutingInfo::categoryToString(MessageCategory category)
{
    const QMetaObject* metaObj = privateObjectForMetaData.metaObject();
    const QMetaEnum& categoryEnum =  metaObj->enumerator(metaObj->indexOfEnumerator("MessageCategory"));
    return categoryEnum.valueToKey(category);
}

QString SimpleLoggerRoutingInfo::componentToString(MessageComponent component)
{
    const QMetaObject* metaObj = privateObjectForMetaData.metaObject();
    const QMetaEnum& categoryEnum =  metaObj->enumerator(metaObj->indexOfEnumerator("MessageComponent"));
    return categoryEnum.valueToKey(component);
}

QString SimpleLoggerRoutingInfo::routingToString(MessageRouting routing)
{
    const QMetaObject* metaObj = privateObjectForMetaData.metaObject();
    const QMetaEnum& categoryEnum =  metaObj->enumerator(metaObj->indexOfEnumerator("MessageRouting"));
    return categoryEnum.valueToKey(routing);
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
    if (m_levels != nullptr && m_levels->size() > 0)
    {
        QMap<MessageCategory, int>::const_iterator i;
        for (i = m_levels->begin(); i != m_levels->end(); ++i)
        {
            XMLUtility::writeElementAttribute(writer, "Level", QString::number(i.value()), "MessageCategory", categoryToString(i.key()));
        }
    }

    if (m_routing != nullptr && m_routing->size() > 0)
    {
        QMap<MessageRouting, bool>::const_iterator i;
        for (i = m_routing->begin(); i != m_routing->end(); ++i)
        {
            XMLUtility::writeElementAttribute(writer, "Routing", XMLUtility::booleanToString(i.value()), "MessageRouting", routingToString(i.key()));
        }
    }

    if (m_regExp != nullptr)
    {
        XMLUtility::write(writer, *m_regExp, "RegEx");
    }

    if (m_format.size() > 0)
    {
        for (int i=0; i<m_format.size(); ++i)
        {
            const QPair<MessageComponent, QString>& comp = m_format.at(i);
            XMLUtility::writeElementAttribute(writer, "Format", comp.second, "MessageComponent", componentToString(comp.first));
        }
    }

    XMLUtility::writeElement(writer, "Enabled", XMLUtility::booleanToString(m_enabled));

    writer.writeEndElement();
    return writer;
}

QXmlStreamReader& SimpleLoggerRoutingInfo::read(QXmlStreamReader& reader)
{
    internalClear();
    qDebug("Ready SimpleLoggerRoutingInfo::read");
    QString name;
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
            qDebug(qPrintable(QString("Found start element thingy name %1").arg(name)));
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
            qDebug(qPrintable(QString("Found start element %1").arg(name)));

            if (QString::compare(name, "RegEx", Qt::CaseInsensitive) == 0)
            {
                if (m_regExp != nullptr)
                {
                    delete m_regExp;
                    m_regExp = nullptr;
                }
                m_regExp = XMLUtility::readRegExp(reader);
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
            qDebug(qPrintable(QString("Found characters for name %1 and value (%2)").arg(name, value)));
            if (name.compare("Enabled", Qt::CaseInsensitive) == 0)
            {
              m_enabled = XMLUtility::stringToBoolean(value);
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
