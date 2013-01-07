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
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/
class SimpleLoggerRoutingInfo : public QObject
{
  Q_OBJECT
  Q_ENUMS(MessageCategory)
  Q_ENUMS(MessageComponent)
  Q_ENUMS(MessageRouting)

public:
  enum MessageCategory  {
      TraceMessage = 0x1,
      DebugMessage = 0x2,
      InformationMessage = 0x4,
      WarningMessage = 0x8,
      ErrorMessage = 0x10,
      UserMessage = 0x20
  };
  Q_DECLARE_FLAGS(MessageCategorys, MessageCategory)

  enum MessageComponent {DateTimeComponent, MessageTypeComponent, MessageTextComponent, MessageLocationComponent, MessageText};

  enum MessageRouting   {
      RouteFile = 0x1,
      RouteEmit = 0x2,
      RouteQDebug = 0x4
  };
  Q_DECLARE_FLAGS(MessageRoutings, MessageRouting)

  /*! \brief Default constructor */
  explicit SimpleLoggerRoutingInfo(QObject *parent = 0);

  /*! \brief Copy constructor */
  SimpleLoggerRoutingInfo(const SimpleLoggerRoutingInfo& obj, QObject *parent = 0);

  /*! \brief Destructor */
  ~SimpleLoggerRoutingInfo();

  /*! \brief Copy the parameter object into this object. */
  const SimpleLoggerRoutingInfo& copy(const SimpleLoggerRoutingInfo& obj);

  /*! \brief Assignment operator. */
  const SimpleLoggerRoutingInfo& operator=(const SimpleLoggerRoutingInfo& obj);

  //**************************************************************************
  /*! \brief Return true if the specified routing is on. If the routing is not known, a value of true is returned.
   *
   *  \param [in] messageRouting Routing type to check.
   *  \return True if the routing is explicitly turned on, or for an unknown type.
   ***************************************************************************/
  bool isRoutingOn(MessageRouting messageRouting) const;
  void setRoutingOn(MessageRoutings messageRoutings) { setRouting(messageRoutings, true); }
  void setRoutingOff(MessageRoutings messageRoutings) { setRouting(messageRoutings, false); }
  void setRouting(MessageRoutings messageRoutings, bool state);


  void setCategoryLevel(MessageCategory category, int level);

  //**************************************************************************
  /*! \brief The message category must match this regExp. An empty string will pass everything.
   * The type is set to regular expression, case insensitive, with greedy matching.
   * The regular expression need only find the source in the regular expression, an exact match is not required.
   *  \param [in] regExp String representation of the message category.If invalid, then everything will match.
   *  \return True if the regular expression is valid, false otherwise.
   ***************************************************************************/
  bool  setRegExp(const QString& regExp);

  //**************************************************************************
  /*! \brief Return true if the regular expression matches the source, and the category level >= level
   *
   *  \param [in] source Must pass the regular expression, unless both are empty.
   *  \param [in] category Used to find the minimum level to pass.
   *  \param [in] level Message level for this message.
   *  \return Reference to this object.
   ***************************************************************************/
  bool passes(const QString& source, const MessageCategory& category, int level) const;

  void clearMessageFormat();

  //**************************************************************************
  /*! \brief Add a message format component to format the message.
   *
   *  \param [in] component What part of the text to include.
   *  \param [in] formatString How to format the component. The value is dependent on the component.
   *     For the date/time, if it is missing, ISO formatting is used.
   *  \return Formatted string.
   ***************************************************************************/
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
  // internal clear deletes stuff and does not re-initialize, so use it carefully.
  void internalClear();
  void internalEmpty();
  void readInternals(QXmlStreamReader& reader, const QString& version);

  QMap<MessageCategory, int>* m_levels;
  QMap<MessageRouting, bool>* m_routing;
  QRegExp* m_regExp;
  QList< QPair<MessageComponent, QString> > m_format;

  // TODO: Use the name and enabled.
  QString m_name;
  bool m_enabled;

  static SimpleLoggerRoutingInfo o;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SimpleLoggerRoutingInfo::MessageCategorys)
Q_DECLARE_OPERATORS_FOR_FLAGS(SimpleLoggerRoutingInfo::MessageRoutings)

inline QXmlStreamWriter& operator<<(QXmlStreamWriter& writer, SimpleLoggerRoutingInfo& info)
{
  return info.write(writer);
}

inline QXmlStreamReader& operator>>(QXmlStreamReader& reader, SimpleLoggerRoutingInfo& info)
{
  return info.read(reader);
}


#endif // SIMPLELOGGERROUTINGINFO_H
