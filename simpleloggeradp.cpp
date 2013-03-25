#include "simpleloggeradp.h"
#include "simpleloggerroutinginfo.h"
#include <QFile>
#include <QMapIterator>
#include <QTextStream>
#include <QTimer>

SimpleLoggerADP::SimpleLoggerADP(QObject *parent) : QObject(parent), m_messageQueue(nullptr), m_logFile(nullptr), m_textStream(nullptr)
{
  m_messageQueue = new LogMessageQueue(this);

  //QTimer *timer = new QTimer(this);
  //connect(timer, SIGNAL(timeout()), this, SLOT(processQueuedMessages()));
  //timer->start(1000);
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
  if (m_messageQueue == nullptr)
  {
    processOneMessage(LogMessageContainer(message, location, dateTime, category, level));
  }
  else
  {
    m_messageQueue->enqueue(new LogMessageContainer(message, location, dateTime, category, level));
    processQueuedMessages();
  }
}

bool SimpleLoggerADP::isProcessing()
{
  if (m_processingMutex.tryLock())
  {
    m_processingMutex.unlock();
    return false;
  }
  return true;
}

void SimpleLoggerADP::processQueuedMessages()
{
  if (m_messageQueue != nullptr)
  {
    // Sanity check before trying to take and hold the mutex.
    if (isProcessing())
    {
      return;
    }
    QMutexLocker locker(&m_processingMutex);

    QQueue<LogMessageContainer*> queue;
    m_messageQueue->getAll(queue);
    bool flushLog = false;
    QString msgs[] = {"", ""} ;
    SimpleLoggerRoutingInfo::MessageCategory cats[] = {SimpleLoggerRoutingInfo::InformationMessage, SimpleLoggerRoutingInfo::InformationMessage };
    while (!queue.isEmpty())
    {
      LogMessageContainer* message = queue.head();

      for (int i=0; i<m_routing.count(); ++i)
      {
        SimpleLoggerRoutingInfo& routing(m_routing[i]);
        if (routing.passes(message->getLocation(), message->getCategory(), message->getLevel()))
        {
          QString finalMessage = routing.formatMessage(message->getMessage(), message->getLocation(), message->getDateTime(), message->getCategory(), message->getLevel());
          if (routing.isRoutingOn(SimpleLoggerRoutingInfo::RouteQDebug))
          {
            if (message->getCategory() != cats[0])
            {
              if (!msgs[0].isEmpty()) {
                qDebug(qPrintable(msgs[0]));
              }
              cats[0] = message->getCategory();
              msgs[0] = finalMessage;
            }
            else
            {
              msgs[0] = msgs[0] + "\n" + finalMessage;
            }
          }
          if (routing.isRoutingOn(SimpleLoggerRoutingInfo::RouteEmit))
          {
            if (message->getCategory() != cats[1])
            {
              if (!msgs[1].isEmpty()) {
                emit formattedMessage(msgs[1], cats[1]);
              }
              cats[1] = message->getCategory();
              msgs[1] = finalMessage;
            }
            else
            {
              msgs[1] = msgs[1] + "\n" + finalMessage;
            }
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
              flushLog = true;
            }
          }
        }
      }
      delete queue.dequeue();
    }
    // Just processed the entire queue.
    if (flushLog && m_logFile != nullptr)
    {
      m_logFile->flush();
    }
    if (!msgs[0].isEmpty())
    {
      qDebug(qPrintable(msgs[0]));
    }
    if (!msgs[1].isEmpty())
    {
      emit formattedMessage(msgs[1], cats[1]);
    }
  }
}

void SimpleLoggerADP::processOneMessage(const LogMessageContainer& message)
{
  QMutexLocker locker(&m_processingMutex);
  for (int i=0; i<m_routing.count(); ++i)
  {
    SimpleLoggerRoutingInfo& routing(m_routing[i]);
    if (routing.passes(message.getLocation(), message.getCategory(), message.getLevel()))
    {
      QString finalMessage = routing.formatMessage(message.getMessage(), message.getLocation(), message.getDateTime(), message.getCategory(), message.getLevel());
      if (routing.isRoutingOn(SimpleLoggerRoutingInfo::RouteQDebug))
      {
        qDebug(qPrintable(finalMessage));
      }
      if (routing.isRoutingOn(SimpleLoggerRoutingInfo::RouteEmit))
      {
        emit formattedMessage(finalMessage, message.getCategory());
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

