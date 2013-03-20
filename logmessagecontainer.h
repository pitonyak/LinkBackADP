#ifndef LOGMESSAGECONTAINER_H
#define LOGMESSAGECONTAINER_H

#include <QString>
#include <QDateTime>
#include "simpleloggerroutinginfo.h"

//**************************************************************************
//! Encapsulate a message for logging.
/*!
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2013
 ***************************************************************************/
class LogMessageContainer
{
public:
  LogMessageContainer();
  LogMessageContainer(const LogMessageContainer& x);
  LogMessageContainer(const QString& message, const QString& location, const QDateTime dateTime, const SimpleLoggerRoutingInfo::MessageCategory category, const int level);

  LogMessageContainer& operator=(const LogMessageContainer& x);

  const QString& getMessage() const { return m_message; }
  const QString& getLocation() const {return m_location; }
  const QDateTime& getDateTime() const {return m_dateTime; }
  SimpleLoggerRoutingInfo::MessageCategory getCategory() const {return m_category; }
  int getLevel() const {return m_level; }

  void setMessage(const QString& message) { m_message = message; }
  void setLocation(const QString& location) { m_location = location; }
  void setDateTime(const QDateTime& dateTime) { m_dateTime = dateTime; }
  void setCategory(const SimpleLoggerRoutingInfo::MessageCategory category) { m_category = category; }
  void setLevel(const int level) { m_level = level; }

private:
  QString m_message;
  QString m_location;
  QDateTime m_dateTime;
  SimpleLoggerRoutingInfo::MessageCategory m_category;
  int m_level;
};

#endif // LOGMESSAGECONTAINER_H
