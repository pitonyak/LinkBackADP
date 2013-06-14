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
#include <QRegExp>
#include <QStringList>

//**************************************************************************
/*! \class SimpleLoggerRoutingInfo
 * \brief Control what logging messages pass and where they should be routed.
 *
 * This object accepts or rejects and then formats messages.
 * This object contains the filter criteria that determines if the message is logged and how the message is formatted.
 * This object knows if the message should be logged using qDebug, loging to a file, or emitting a signal, but
 * it is the containing classes job to actually perform the action.
 *
 * TODO: Adding a filename here would allow more flexibility.
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
  SimpleLoggerRoutingInfo& operator=(const SimpleLoggerRoutingInfo& obj);

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

  bool isEnabled() const;
  void setEnabled(bool enabledState);


  void setCategoryLevel(MessageCategory category, int level);
  int getCategoryLevel(MessageCategory category) const;

  //**************************************************************************
  /*! \brief The message location string (filename and line number) must match this regExp. An empty string passes everything.
   *
   * The regular expression need only find the source in the regular expression, an exact match is not required unless the regular expression dictates it is so.
   * If the regular expression is not valid, the regular expression is left unchanged.
   *
   *  \param [in] regExp String representation of the message category.If invalid, then everything will match.
   *  \return True if the regular expression is valid, false otherwise.
   ***************************************************************************/
  bool  setLocationRegExp(const QString& regExp);

  //**************************************************************************
  /*! \brief Get the regular expression string for Location (filename and line number).
   *
   *  \return Regular expression string for Location.
   ***************************************************************************/
  QString getLocationRegExpString() const;

  //**************************************************************************
  /*! \brief Get the regular expression pattern syntax for Location (filename and line number).
   *
   * <table border>
   * <tr><td><b>Syntax</b></td><td><b>Description</b></td></tr>
   * <tr><td>QRegExp::RegExp</td><td>Perl-like pattern matching syntax without greedy quantifiers.</td></tr>
   * <tr><td>QRegExp::RegExp2</td><td>Perl-like pattern matching syntax with greedy quantifiers.</td></tr>
   * <tr><td>QRegExp::Wildcard</td><td>Dimple pattern matching syntax similar to that used by shells (command interpreters) for "file globbing".</td></tr>
   * <tr><td>QRegExp::WildcardUnix</td><td>Wildcard with the behavior of a Unix shell. The wildcard characters can be escaped with the character "\".</td></tr>
   * <tr><td>QRegExp::FixedString</td><td>The pattern is a fixed string. This is equivalent to using the RegExp pattern on a string in which all metacharacters are escaped using escape().</td></tr>
   * <tr><td>QRegExp::W3CXmlSchema11</td><td>The pattern is a regular expression as defined by the W3C XML Schema 1.1 specification.</td></tr>
   * </table>
   *
   *  \return Regular expression pattern syntax for Location.
   ***************************************************************************/
  QRegExp::PatternSyntax getLocationRegExpPatternSyntax() const;

  //**************************************************************************
  /*! \brief Set the regular expression pattern syntax for Location (filename and line number).
   *
   * <table border>
   * <tr><td><b>Syntax</b></td><td><b>Description</b></td></tr>
   * <tr><td>QRegExp::RegExp</td><td>Perl-like pattern matching syntax without greedy quantifiers.</td></tr>
   * <tr><td>QRegExp::RegExp2</td><td>Perl-like pattern matching syntax with greedy quantifiers.</td></tr>
   * <tr><td>QRegExp::Wildcard</td><td>Dimple pattern matching syntax similar to that used by shells (command interpreters) for "file globbing".</td></tr>
   * <tr><td>QRegExp::WildcardUnix</td><td>Wildcard with the behavior of a Unix shell. The wildcard characters can be escaped with the character "\".</td></tr>
   * <tr><td>QRegExp::FixedString</td><td>The pattern is a fixed string. This is equivalent to using the RegExp pattern on a string in which all metacharacters are escaped using escape().</td></tr>
   * <tr><td>QRegExp::W3CXmlSchema11</td><td>The pattern is a regular expression as defined by the W3C XML Schema 1.1 specification.</td></tr>
   * </table>
   *
   *  \param [in] syntax Regular expression pattern syntax.
   ***************************************************************************/
  void setLocationRegExpPatternSyntax(const QRegExp::PatternSyntax syntax);

  //**************************************************************************
  /*! \brief Get case sensitivity flag for the location (filename and line number) regExp.
   *
   * \return Case sensitivity flag for the location (filename and line number) regExp.
   ***************************************************************************/
  Qt::CaseSensitivity getLocationRegExpCaseSensitivity() const;

  //**************************************************************************
  /*! \brief Set the case sensitivity flag for the location (filename and line number) regExp.
   *
   *  \param [in] cs Case Sensitivity flag to use.
   ***************************************************************************/
  void setLocationRegExpCaseSensitivity(const Qt::CaseSensitivity cs = Qt::CaseInsensitive);

  //**************************************************************************
  /*! \brief The unformatted message string must match this regExp. An empty string passes everything.
   *
   * The regular expression need only find the source in the regular expression, an exact match is not required unless the regular expression dictates it is so.
   * If the regular expression is not valid, the regular expression is left unchanged.
   *
   *  \param [in] regExp String representation of the message category.If invalid, then everything will match.
   *  \return True if the regular expression is valid, false otherwise.
   ***************************************************************************/
  bool  setMessageRegExp(const QString& regExp);

  //**************************************************************************
  /*! \brief Get the regular expression string for the unformatted message.
   *
   *  \return Regular expression string for Message.
   ***************************************************************************/
  QString getMessageRegExpString() const;

  //**************************************************************************
  /*! \brief Get the regular expression pattern syntax for the unformatted message.
   *
   * <table border>
   * <tr><td><b>Syntax</b></td><td><b>Description</b></td></tr>
   * <tr><td>QRegExp::RegExp</td><td>Perl-like pattern matching syntax without greedy quantifiers.</td></tr>
   * <tr><td>QRegExp::RegExp2</td><td>Perl-like pattern matching syntax with greedy quantifiers.</td></tr>
   * <tr><td>QRegExp::Wildcard</td><td>Dimple pattern matching syntax similar to that used by shells (command interpreters) for "file globbing".</td></tr>
   * <tr><td>QRegExp::WildcardUnix</td><td>Wildcard with the behavior of a Unix shell. The wildcard characters can be escaped with the character "\".</td></tr>
   * <tr><td>QRegExp::FixedString</td><td>The pattern is a fixed string. This is equivalent to using the RegExp pattern on a string in which all metacharacters are escaped using escape().</td></tr>
   * <tr><td>QRegExp::W3CXmlSchema11</td><td>The pattern is a regular expression as defined by the W3C XML Schema 1.1 specification.</td></tr>
   * </table>
   *
   *  \return Regular expression pattern syntax for Message.
   ***************************************************************************/
  QRegExp::PatternSyntax getMessageRegExpPatternSyntax() const;

  //**************************************************************************
  /*! \brief Set the regular expression pattern syntax for the unformatted message.
   *
   * <table border>
   * <tr><td><b>Syntax</b></td><td><b>Description</b></td></tr>
   * <tr><td>QRegExp::RegExp</td><td>Perl-like pattern matching syntax without greedy quantifiers.</td></tr>
   * <tr><td>QRegExp::RegExp2</td><td>Perl-like pattern matching syntax with greedy quantifiers.</td></tr>
   * <tr><td>QRegExp::Wildcard</td><td>Dimple pattern matching syntax similar to that used by shells (command interpreters) for "file globbing".</td></tr>
   * <tr><td>QRegExp::WildcardUnix</td><td>Wildcard with the behavior of a Unix shell. The wildcard characters can be escaped with the character "\".</td></tr>
   * <tr><td>QRegExp::FixedString</td><td>The pattern is a fixed string. This is equivalent to using the RegExp pattern on a string in which all metacharacters are escaped using escape().</td></tr>
   * <tr><td>QRegExp::W3CXmlSchema11</td><td>The pattern is a regular expression as defined by the W3C XML Schema 1.1 specification.</td></tr>
   * </table>
   *
   *  \param [in] syntax Regular expression pattern syntax.
   ***************************************************************************/
  void setMessageRegExpPatternSyntax(const QRegExp::PatternSyntax syntax);

  //**************************************************************************
  /*! \brief Get case sensitivity flag for the unformatted message regExp.
   *
   * \return Case sensitivity flag for the Message (filename and line number) regExp.
   ***************************************************************************/
  Qt::CaseSensitivity getMessageRegExpCaseSensitivity() const;

  //**************************************************************************
  /*! \brief Set the case sensitivity flag for the unformatted message regExp.
   *
   *  \param [in] cs Case Sensitivity flag to use.
   ***************************************************************************/
  void setMessageRegExpCaseSensitivity(const Qt::CaseSensitivity cs = Qt::CaseInsensitive);

  QString getName() const { return m_name; }
  void setName(const QString& name) { m_name = name; }

  //**************************************************************************
  /*! \brief Return true if the regular expression matches the source, and the category level >= level
   *
   *  \param [in] source Must pass the regular expression, unless both are empty.
   *  \param [in] category Used to find the minimum level to pass.
   *  \param [in] level Message level for this message.
   *  \return Reference to this object.
   ***************************************************************************/
  bool passes(const QString& source, const MessageCategory& category, int level, const QString& message) const;

  /*! \brief Clear the message formats. */
  void clearMessageFormat();

  //**************************************************************************
  /*! \brief Add a message format component to format the message.
   *
   *  \param [in] component What part of the text to include.
   *  \param [in] formatString How to format the component. The value is dependent on the component.
   *     For the date/time, if it is missing, ISO formatting is used.
   ***************************************************************************/
  void addMessageFormat(MessageComponent component, const QString& formatString);

  //**************************************************************************
  /*! \brief Format a message for logging.
   *
   *  \param [in] message Primary message to log.
   *  \param [in] location Location provided compiler macros; usually file name and line number.
   *  \param [in] dateTime Date and time the message was created.
   *  \param [in] category Message category flags such as error, warning, informational, etc.
   *  \param [in] level Severity level. Lower is more sever, so level 100 is less likely to be logged than 3.
   *  \return Formatted string.
   ***************************************************************************/
  QString formatMessage(const QString& message, const QString& location, const QDateTime dateTime, MessageCategory category, int level) const;

  //**************************************************************************
  /*! \brief Write this object as XML to a stream.
   *
   *  \param [in,out] writer Stream writer to which the XML is written.
   *  \return The stream writer, which makes it easier to implement a stream operator.
   ***************************************************************************/
  QXmlStreamWriter& write(QXmlStreamWriter& writer) const;

  //**************************************************************************
  /*! \brief Read this object as XML from a stream.
   *
   *  \param [in,out] reader Stream reader from which the XML is read.
   *  \return The stream reader, which makes it easier to implement a stream operator.
   ***************************************************************************/
  QXmlStreamReader& read(QXmlStreamReader& reader);

  //**************************************************************************
  /*! \brief Convert the MessageCategory to a string for display purposes and streaming to XML.
   *
   *  \param [in] category MessageCategory to convert to a string.
   *  \param [in] maxlen limits the length of the returned string. If less than 1, the entire string is returned.
   *  \param [in] brief Remove the trailing "Message" text from the string.
   *  \return String representation of the MessageCategory.
   ***************************************************************************/
  static QString categoryToString(MessageCategory category, const int maxlen = -1, const bool brief=false);

  //**************************************************************************
  /*! \brief Convert the MessageComponent to a string for display purposes and streaming to XML.
   *
   *  \param [in] component MessageComponent to convert to a string.
   *  \param [in] maxlen limits the length of the returned string. If less than 1, the entire string is returned.
   *  \return String representation of the MessageComponent.
   ***************************************************************************/
  static QString componentToString(MessageComponent component, const int maxlen = -1);

  //**************************************************************************
  /*! \brief Convert the MessageRouting to a string for display purposes and streaming to XML.
   *
   *  \param [in] routing MessageRouting to convert to a string.
   *  \param [in] maxlen limits the length of the returned string. If less than 1, the entire string is returned.
   *  \return String representation of the MessageRouting.
   ***************************************************************************/
  static QString routingToString(MessageRouting routing, const int maxlen = -1);

  //**************************************************************************
  /*! \brief Convert the string (as saved as XML) to a MessageCategory.
   *
   *  \param [in] category String to convert.
   *  \return Properly typed enum object.
   ***************************************************************************/
  static MessageCategory stringToCategory(const QString& category);

  //**************************************************************************
  /*! \brief Convert the string (as saved as XML) to a MessageComponent.
   *
   *  \param [in] component String to convert.
   *  \return Properly typed enum object.
   ***************************************************************************/
  static MessageComponent stringToComponent(const QString& component);

  //**************************************************************************
  /*! \brief Convert the string (as saved as XML) to a MessageRouting.
   *
   *  \param [in] routing String to convert.
   *  \return Properly typed enum object.
   ***************************************************************************/
  static MessageRouting stringToRouting(const QString& routing);

  static QStringList getMessageRoutingStrings();

  const QList< QPair<MessageComponent, QString> > & getMessageComponents() const { return m_format; }

  QString getLevelsAsString() const;

signals:

public slots:

private:
  /*! \brief Deletes stuff and does not re-initialize, so use it carefully. All pointers will be null. */
  void internalDelete();

  /*! \brief Clear message categories, routings, formats, and delete the regular expression.  */
  void internalClear();

  void readInternals(QXmlStreamReader& reader, const QString& version);

  /*! \brief Associate message categories to a logging level. */
  QMap<MessageCategory, int>* m_levels;

  /*! \brief Associate message routing to an enable/diable bit. */
  QMap<MessageRouting, bool>* m_routing;

  //**************************************************************************
  /*! \brief If not null, a message is only printed if the message source (filename and line number) must the regular expression.
   * This is case-insensitive.
   ***************************************************************************/
  QRegExp* m_locationRegExp;

  QRegExp* m_messageRegExp;

  //**************************************************************************
  /*! \brief How is the regular expression handled.
   *
   * - QRegExp::RegExp	A rich Perl-like pattern matching syntax.
   * - QRegExp::RegExp2	Like RegExp, but with greedy quantifiers. (Introduced in Qt 4.2.)
   * - QRegExp::Wildcard	This provides a simple pattern matching syntax similar to that used by shells (command interpreters) for "file globbing". See QRegExp wildcard matching.
   * - QRegExp::WildcardUnix	This is similar to Wildcard but with the behavior of a Unix shell. The wildcard characters can be escaped with the character "\".
   * - QRegExp::FixedString	The pattern is a fixed string. This is equivalent to using the RegExp pattern on a string in which all metacharacters are escaped using escape().
   * - QRegExp::W3CXmlSchema11	The pattern is a regular expression as defined by the W3C XML Schema 1.1 specification.
   ***************************************************************************/
  QRegExp::PatternSyntax m_locationRegExpPatternSyntax;
  QRegExp::PatternSyntax m_messageRegExpPatternSyntax;


  Qt::CaseSensitivity m_locRegExpCaseSensitivity;
  Qt::CaseSensitivity m_messageRegExpCaseSensitivity;

  /*! \brief Message components are printed in the order that they appear here. A non-empty date/time component causes the date/time to be formatted based on the included format string. */
  QList< QPair<MessageComponent, QString> > m_format;

  /*! \brief Fast way to disable an object. */
  bool m_enabled;

  /*! \brief Give the object a name that may be meaningful to a user while editing these things. */
  QString m_name;

  /*! \brief This object is used in the static methods to obtain QT meta-data to convert between strings and enums. */
  static SimpleLoggerRoutingInfo privateObjectForMetaData;
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
