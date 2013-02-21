#include "simpleloggeradp.h"
#include "simpleloggerroutinginfo.h"
#include <QFile>
#include <QMapIterator>
#include <QTextStream>


SimpleLoggerADP::SimpleLoggerADP(QObject *parent) : QObject(parent), m_logFile(nullptr), m_textStream(nullptr)
{
}

SimpleLoggerADP::~SimpleLoggerADP()
{
  if (m_logFile != nullptr)
  {
    if (m_logFile->isOpen())
    {
      m_logFile->close();
    }
    delete m_logFile;
    m_logFile = nullptr;
  }
  if (m_textStream != nullptr)
  {
    delete m_textStream;
    m_textStream = nullptr;
  }
}

void SimpleLoggerADP::clearRouting()
{
  m_routing.clear();
}

void SimpleLoggerADP::addRouting(const SimpleLoggerRoutingInfo& routing)
{
  m_routing.append(routing);
}

void SimpleLoggerADP::setFileName(const QString& logFileName)
{
  if (logFileName != m_logFileName)
  {
    if (m_logFile != nullptr)
    {
      if (m_logFile->isOpen())
      {
        m_logFile->close();
      }
      delete m_logFile;
      m_logFile = nullptr;
    }
    if (m_textStream != nullptr)
    {
      delete m_textStream;
      m_textStream = nullptr;
    }
    m_logFile = new QFile(m_logFileName);
  }
}

QString SimpleLoggerADP::getFileName() const
{
  return m_logFileName;
}


void SimpleLoggerADP::receiveMessage(const QString& message, const QString& location, const QDateTime dateTime, const SimpleLoggerRoutingInfo::MessageCategory category, const int level)
{
  for (int i=0; i<m_routing.count(); ++i)
  {
    SimpleLoggerRoutingInfo& routing(m_routing[i]);
    if (routing.passes(location, category, level))
    {
      QString finalMessage = routing.formatMessage(message, location, dateTime, category, level);
      if (routing.isRoutingOn(SimpleLoggerRoutingInfo::RouteQDebug))
      {
        qDebug(qPrintable(finalMessage));
      }
      if (routing.isRoutingOn(SimpleLoggerRoutingInfo::RouteEmit))
      {
        emit formattedMessage(finalMessage, category);
      }
      if (m_logFile != nullptr && routing.isRoutingOn(SimpleLoggerRoutingInfo::RouteFile))
      {
        if (!m_logFile->isOpen())
        {
          if (!m_logFile->open(QIODevice::WriteOnly))
          {
            delete m_logFile;
            m_logFile = nullptr;
            qDebug(qPrintable(QString("Failed to open File to write : %1").arg(getFileName())));
            routing.setRoutingOff(SimpleLoggerRoutingInfo::RouteFile);
          }
          else
          {
            m_textStream = new QTextStream(m_logFile);
          }
        }
        if (m_logFile != nullptr)
        {
          *m_textStream << finalMessage << "\n";
        }
      }
    }
  }
}

QXmlStreamWriter& SimpleLoggerADP::write(QXmlStreamWriter& writer) const
{
  writer.writeStartElement("SimpleLoggerADP");
  if (m_logFileName.length() > 0)
  {
    writer.writeTextElement("FileName", m_logFileName);
  }

  for (int i=0; i<m_routing.size(); ++i)
  {
    m_routing[i].write(writer);
  }
  writer.writeEndElement();
  return writer;
}
QXmlStreamReader& SimpleLoggerADP::read(QXmlStreamReader& reader)
{
  clearRouting();
  setFileName("");

  QString name;
  while (!reader.atEnd()) {
    if (reader.isStartElement()) {
      name = reader.name().toString();
      if (QString::compare(name, "SimpleLoggerADP", Qt::CaseInsensitive) == 0) {
        readInternals(reader, "1");
        return reader;
      } else {
        reader.raiseError(QObject::tr("Not a SimpleLoggerADP"));
      }
    } else if (reader.isEndElement()) {
      return reader;
    }
    reader.readNext();
  }
  return reader;
}

void SimpleLoggerADP::readInternals(QXmlStreamReader& reader, const QString&)
{
  QString name;
  while (!reader.atEnd())
  {
    if (reader.isStartElement())
    {
      name = reader.name().toString();
      if (QString::compare(name, "SimpleLoggerRoutingInfo", Qt::CaseInsensitive) == 0)
      {
        SimpleLoggerRoutingInfo info;
        reader >> info;
        addRouting(info);
      }
    } else if (reader.isCharacters() && !name.isEmpty()) {
      if (QString::compare(name, "FileName", Qt::CaseInsensitive) == 0)
      {
        setFileName(reader.text().toString());
      }
    } else if (reader.isEndElement()) {
      if (QString::compare(reader.name().toString(), "SimpleLoggerADP", Qt::CaseInsensitive) == 0)
      {
        return;
      }
      name = "";
    }
    reader.readNext();
  }
}
