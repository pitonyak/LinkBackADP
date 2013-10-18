#include "logmessagecontainer.h"

LogMessageContainer::LogMessageContainer()
{
}

LogMessageContainer::LogMessageContainer(const LogMessageContainer &x)
{
  operator=(x);
}


LogMessageContainer::LogMessageContainer(const QString& message, const QString& location, const QDateTime dateTime, const SimpleLoggerRoutingInfo::MessageCategory category, const int level) :
  m_message(message), m_location(location), m_dateTime(dateTime), m_category(category), m_level(level)
{
}

LogMessageContainer& LogMessageContainer::operator=(const LogMessageContainer& x) {
  if (&x != this) {
      m_message = x.m_message;
      m_location = x.m_location;
      m_dateTime = x.m_dateTime;
      m_category = x.m_category;
      m_level = x.m_level;
  }
  return *this;
}
