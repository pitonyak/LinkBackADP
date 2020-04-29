#include "simpleloggeradp.h"
#include "simpleloggerroutinginfo.h"
#include <QFile>
#include <QMapIterator>
#include <QTextStream>
#include <QDebug>

SimpleLoggerADP::SimpleLoggerADP(QObject *parent) : QObject(parent), m_failedProcessingAttempts(0), m_numErrors(0), m_messageQueue(nullptr), m_logFile(nullptr), m_textStream(nullptr)
{
}

void SimpleLoggerADP::enableMessageQueue()
{
  if (m_messageQueue == nullptr)
  {
    // Auto deleted because the logger owns it.
    m_messageQueue = new LogMessageQueue(this);
  }
}

void SimpleLoggerADP::disableMessageQueue()
{
  if (m_messageQueue != nullptr)
  {
    m_messageQueue = nullptr;
  }
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
  if (m_messageQueue != nullptr)
  {
    // The destructor will delete the contents.
    delete m_messageQueue;
    m_messageQueue = nullptr;
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

void SimpleLoggerADP::addRouting(const QList<SimpleLoggerRoutingInfo>& routings)
{
  m_routing.append(routings);
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
  if (category == SimpleLoggerRoutingInfo::ErrorMessage)
  {
    ++m_numErrors;
  }
  if (m_messageQueue == nullptr)
  {
    processOneMessage(LogMessageContainer(message, location, dateTime, category, level));
  }
  else if (m_messageQueue->enqueue(new LogMessageContainer(message, location, dateTime, category, level)) > 100)
  {
    // What if this is HUGE?
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
      ++m_failedProcessingAttempts;
      if (m_failedProcessingAttempts > 10) {
        m_messageQueue = nullptr;
        QString message("Too many failed attempts to process queued messages");
        QString location("processQueuedMessages");
        QDateTime dateTime = QDateTime::currentDateTime();
        SimpleLoggerRoutingInfo::MessageCategory category = SimpleLoggerRoutingInfo::ErrorMessage;
        int level = -1;
        // Do not call processOneMessage because it attempts to lock the mutex!
        // At this point, too many failed attempts, to just dump it out!
       //processOneMessage(LogMessageContainer(message, location, dateTime, category, level));
        processOneMessageInFailureMode(LogMessageContainer(message, location, dateTime, category, level));
      }
      return;
    }
    m_failedProcessingAttempts = 0;
    QMutexLocker locker(&m_processingMutex);

    QQueue<LogMessageContainer*> queue;
    m_messageQueue->getAll(queue);
    // We will no longer touch this message queue in this routine,
    // so new messages can be added to the queue without affecting us.

    bool flushLog = false;
    QString msgs[] = {"", ""} ;
    SimpleLoggerRoutingInfo::MessageCategory cats[] = {SimpleLoggerRoutingInfo::InformationMessage, SimpleLoggerRoutingInfo::InformationMessage };
    while (!queue.isEmpty())
    {
      LogMessageContainer* message = queue.head();

      for (int i=0; i<m_routing.count(); ++i)
      {
        SimpleLoggerRoutingInfo& routing(m_routing[i]);
        if (routing.passes(message->getLocation(), message->getCategory(), message->getLevel(), message->getMessage()))
        {
          QString finalMessage = routing.formatMessage(message->getMessage(), message->getLocation(), message->getDateTime(), message->getCategory(), message->getLevel());
          if (routing.isRoutingOn(SimpleLoggerRoutingInfo::RouteQDebug))
          {
            if (message->getCategory() != cats[0])
            {
              if (!msgs[0].isEmpty()) {
                qDebug() << msgs[0];
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
                qDebug() << QString("Failed to open File to write : %1").arg(getFileName());
                routing.setRoutingOff(SimpleLoggerRoutingInfo::RouteFile);
              }
              else
              {
                if (m_textStream != nullptr)
                {
                  delete m_textStream;
                  m_textStream = nullptr;
                }
                m_textStream = new QTextStream(m_logFile);
              }
            }
            if (m_textStream != nullptr)
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
      qDebug() << msgs[0];
    }
    if (!msgs[1].isEmpty())
    {
      emit formattedMessage(msgs[1], cats[1]);
    }
  }
}

void SimpleLoggerADP::processOneMessageInFailureMode(const LogMessageContainer& message)
{
    switch(message.getCategory()) {
    case SimpleLoggerRoutingInfo::TraceMessage:
    case SimpleLoggerRoutingInfo::DebugMessage:
        qDebug() << message.getMessage();
        break;

    case SimpleLoggerRoutingInfo::UserMessage:
    case SimpleLoggerRoutingInfo::InformationMessage:
        qInfo(qPrintable(message.getMessage()));
        break;

    case SimpleLoggerRoutingInfo::WarningMessage:
        qWarning(qPrintable(message.getMessage()));
        break;

    case SimpleLoggerRoutingInfo::ErrorMessage:
        qCritical(qPrintable(message.getMessage()));
        break;

    default:
        // qFatal exits I believe.
        // qFatal(message.getMessage());
        qCritical(qPrintable(message.getMessage()));
        break;
    }
}

void SimpleLoggerADP::processOneMessage(const LogMessageContainer& message)
{
  QMutexLocker locker(&m_processingMutex);
  for (int i=0; i<m_routing.count(); ++i)
  {
    SimpleLoggerRoutingInfo& routing(m_routing[i]);
    if (routing.passes(message.getLocation(), message.getCategory(), message.getLevel(), message.getMessage()))
    {
      QString finalMessage = routing.formatMessage(message.getMessage(), message.getLocation(), message.getDateTime(), message.getCategory(), message.getLevel());
      if (routing.isRoutingOn(SimpleLoggerRoutingInfo::RouteQDebug))
      {
        qDebug() << finalMessage;
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
            qDebug() << QString("Failed to open File to write : %1").arg(getFileName());
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

