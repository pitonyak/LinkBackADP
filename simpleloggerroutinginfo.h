#ifndef SIMPLELOGGERROUTINGINFO_H
#define SIMPLELOGGERROUTINGINFO_H

#include <QObject>
#include <QMap>
#include <QPair>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

//**************************************************************************
//! Control what logging messages pass, and where they should be routed.
/*!
 *  This routine is Copyright (c) by Andrew Pitonyak, and may be used
 *  without restrictions or renumerations. I make no statement of correctness.
 ***************************************************************************/
class SimpleLoggerRoutingInfo : public QObject
{
  Q_OBJECT
  Q_ENUMS(MessageCategory)
  Q_ENUMS(MessageComponent)
  Q_ENUMS(MessageRouting)

public:
  enum MessageCategory  {TraceMessage, DebugMessage, InformationMessage, WarningMessage, ErrorMessage};
  enum MessageComponent {DateTimeComponent, MessageTypeComponent, MessageTextComponent, MessageLocationComponent, MessageText};
  enum MessageRouting   {RouteFile, RouteEmit, RouteQDebug};

  explicit SimpleLoggerRoutingInfo(QObject *parent = 0);
  SimpleLoggerRoutingInfo(const SimpleLoggerRoutingInfo& obj);
  ~SimpleLoggerRoutingInfo();

  const SimpleLoggerRoutingInfo& copy(const SimpleLoggerRoutingInfo& obj);
  const SimpleLoggerRoutingInfo& operator=(const SimpleLoggerRoutingInfo& obj);

  /*! \brief Return true if the specified routing is on. If the routing is not known, a value of true is returned.
   *
   *  \param [in] messageRouting Routing type to check.
   *  \return True if the routing is explicitly turned on, or for an unknown type.
   */
  bool isRoutingOn(MessageRouting messageRouting) const;
  void setRoutingOn(MessageRouting messageRouting);
  void setRoutingOff(MessageRouting messageRouting);

  void setCategoryLevel(MessageCategory category, int level);

  /*! \brief The message category must match this regExp. An empty string will pass everything.
   * The type is set to regular expression, case insensitive, with greedy matching.
   * The regular expression need only find the source in the regular expression, an exact match is not required.
   *  \param [in] regExp String representation of the message category.If invalid, then everything will match.
   *  \return True if the regular expression is valid, false otherwise.
   */
  bool  setRegExp(const QString& regExp);

  /*! \brief Return true if the regular expression matches the source, and the category level >= level
   *
   *  \param [in] source Must pass the regular expression, unless both are empty.
   *  \param [in] category Used to find the minimum level to pass.
   *  \param [in] level Message level for this message.
   *  \return Reference to this object.
   */
  bool passes(const QString& source, const MessageCategory& category, int level) const;

  void clearMessageFormat();
  /*! \brief Add a message format component to format the message.
   *
   *  \param [in] component What part of the text to include.
   *  \param [in] formatString How to format the component. The value is dependent on the component.
   *     For the date/time, if it is missing, ISO formatting is used.
   *  \return Formatted string.
   */
  void addMessageFormat(MessageComponent component, const QString& formatString);
  QString formatMessage(const QString& message, const QString& location, const QDateTime dateTime, MessageCategory category, int level) const;

  QXmlStreamWriter& write(QXmlStreamWriter& writer) const;
  QXmlStreamReader& read(QXmlStreamReader& reader);

  static QString categoryToString(MessageCategory category);
  static QString componentToString(MessageComponent component);
  static QString routingToString(MessageRouting routing);

  static MessageCategory stringToCategory(const QString& category);
  static MessageComponent stringToComponent(const QString& component);
  static MessageRouting stringToRouting(const QString& routing);


signals:

public slots:

private:
  void internalClear();
  void internalEmpty();
  void readInternals(QXmlStreamReader& reader, const QString& version);

  QMap<MessageCategory, int>* m_levels;
  QMap<MessageRouting, bool>* m_routing;
  QRegExp* m_regExp;
  QList< QPair<MessageComponent, QString> > m_format;

  static SimpleLoggerRoutingInfo o;
};

inline QXmlStreamWriter& operator<<(QXmlStreamWriter& writer, SimpleLoggerRoutingInfo& info)
{
  return info.write(writer);
}

inline QXmlStreamReader& operator>>(QXmlStreamReader& reader, SimpleLoggerRoutingInfo& info)
{
  return info.read(reader);
}


#endif // SIMPLELOGGERROUTINGINFO_H
