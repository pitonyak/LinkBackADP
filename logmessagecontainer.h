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

  //**************************************************************************
  /*! \brief Receive a message and log it based on the routings.
   *  \param [in] message Primary message to log.
   *  \param [in] location Location provided compiler macros; usually file name and line number.
   *  \param [in] dateTime Date and time the message was created.
   *  \param [in] category Message category flags such as error, warning, informational, etc.
   *  \param [in] level Severity level. Lower is more sever, so level 100 is less likely to be logged than 3.
   ***************************************************************************/
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
  //**************************************************************************
  /*! \brief Primary message to log.
   ***************************************************************************/
  QString m_message;

  //**************************************************************************
  /*! Location provided compiler macros; usually file name and line number.
   ***************************************************************************/
  QString m_location;

  //**************************************************************************
  /*! \brief dateTime Date and time the message was created.
   ***************************************************************************/
  QDateTime m_dateTime;

  //**************************************************************************
  /*! \brief category Message category flags such as error, warning, informational, etc.
   ***************************************************************************/
  SimpleLoggerRoutingInfo::MessageCategory m_category;

  //**************************************************************************
  /*! \brief level Severity level. Lower is more sever, so level 100 is less likely to be logged than 3.
   ***************************************************************************/
  int m_level;
};

#endif // LOGMESSAGECONTAINER_H
