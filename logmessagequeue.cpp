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

void LogMessageQueue::enqueue(LogMessageContainer* message)
{
  if (message != nullptr)
  {
    QMutexLocker locker(&m_mutex);
    m_queue.enqueue(message);
  }
}

bool LogMessageQueue::isEmpty() const
{
  QMutexLocker locker(&m_mutex);
  return m_queue.isEmpty();
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
