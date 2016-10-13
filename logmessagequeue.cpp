#include "logmessagequeue.h"

LogMessageQueue::LogMessageQueue(QObject *parent) :
  QObject(parent)
{
}

LogMessageQueue::~LogMessageQueue()
{
    while (!m_queue.isEmpty())
    {
        delete m_queue.dequeue();
    }
}

void LogMessageQueue::enqueue(const QString &message, const QString &location, const QDateTime dateTime, const SimpleLoggerRoutingInfo::MessageCategory category, const int level)
{
  enqueue(new LogMessageContainer(message, location , dateTime, category, level));
}

int LogMessageQueue::enqueue(LogMessageContainer* message)
{
  QMutexLocker locker(&m_mutex);
  if (message != nullptr)
  {
    m_queue.enqueue(message);
  }
  return m_queue.isEmpty();
}

bool LogMessageQueue::isEmpty() const
{
  QMutexLocker locker(&m_mutex);
  return m_queue.isEmpty();
}

int LogMessageQueue::queueSize() const
{
  QMutexLocker locker(&m_mutex);
  return m_queue.size();
}

void LogMessageQueue::getAll(QQueue<LogMessageContainer *> &queue)
{
    queue.clear();
    QMutexLocker locker(&m_mutex);
    queue.swap(m_queue);
}

void LogMessageQueue::receiveMessage(const QString &message, const QString &location, const QDateTime dateTime, const SimpleLoggerRoutingInfo::MessageCategory category, const int level)
{
  enqueue(message, location, dateTime, category, level);
}
