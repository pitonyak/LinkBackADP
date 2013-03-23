#include "logmessagequeue.h"

LogMessageQueue::LogMessageQueue(QObject *parent) :
  QObject(parent)
{
}

void LogMessageQueue::enqueue(const QString &message, const QString &location, const QDateTime dateTime, const SimpleLoggerRoutingInfo::MessageCategory category, const int level)
{
  QMutexLocker locker(&m_mutex);
  m_queue.enqueue(LogMessageContainer(message, location , dateTime, category, level));
}

void LogMessageQueue::enqueue(const LogMessageContainer &message)
{
  QMutexLocker locker(&m_mutex);
  m_queue.enqueue(message);
}

bool LogMessageQueue::isEmpty() const
{
  QMutexLocker locker(&m_mutex);
  return m_queue.isEmpty();
}

void LogMessageQueue::receiveMessage(const QString &message, const QString &location, const QDateTime dateTime, const SimpleLoggerRoutingInfo::MessageCategory category, const int level)
{
  enqueue(message, location, dateTime, category, level);
}
