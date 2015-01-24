#ifndef SIMPLELOGGERADP_H
#define SIMPLELOGGERADP_H

#include "simpleloggerroutinginfo.h"
#include "logmessagequeue.h"
#include <QObject>
#include <QDateTime>
#include <QMutex>

class QTextStream;
class QFile;

//**************************************************************************
/*! \class SimpleLoggerADP
 * \brief Log object that handles all of the logging.
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 *
 * \code{.cpp}

   SimpleLoggerADP logger;

   // Configure using an XML file.
   QFile file("/path/to/log/config/file/logger.xml");
   if (file.open(QIODevice::ReadOnly)) {
     QXmlStreamReader reader(&file);
     reader >> logger;
     file.close();
   }

   // Manually configure the logger

   // Step 1: How is the message formatted?
   SimpleLoggerRoutingInfo routing;
   routing.addMessageFormat(SimpleLoggerRoutingInfo::DateTimeComponent, "");
   routing.addMessageFormat(SimpleLoggerRoutingInfo::MessageText, " ");
   routing.addMessageFormat(SimpleLoggerRoutingInfo::MessageTypeComponent, "");
   routing.addMessageFormat(SimpleLoggerRoutingInfo::MessageText, " ");
   routing.addMessageFormat(SimpleLoggerRoutingInfo::MessageLocationComponent, "");
   routing.addMessageFormat(SimpleLoggerRoutingInfo::MessageText, " | ");
   routing.addMessageFormat(SimpleLoggerRoutingInfo::MessageTextComponent, "");

   // Step 2: What messages pass?
   int logLevel = 1;

   routing.setCategoryLevel(SimpleLoggerRoutingInfo::TraceMessage, 0);
   routing.setCategoryLevel(SimpleLoggerRoutingInfo::DebugMessage, 0);
   routing.setCategoryLevel(SimpleLoggerRoutingInfo::InformationMessage, logLevel);
   routing.setCategoryLevel(SimpleLoggerRoutingInfo::WarningMessage, logLevel);
   routing.setCategoryLevel(SimpleLoggerRoutingInfo::ErrorMessage, logLevel);
   routing.setCategoryLevel(SimpleLoggerRoutingInfo::UserMessage, logLevel);

   // Step 3: Where are messages sent?
   routing.setRoutingOn(SimpleLoggerRoutingInfo::RouteFile | SimpleLoggerRoutingInfo::RouteQDebug);

   // Step 4: Add the routing to the logger.
   logger.addRouting(routing);
 * \endcode
 *
 * By default, messages are parsed as they come in. For high volumn situations, you probably want to
 * queue the messages and then parse them in bulk.
 * code{.cpp}

    // Connect the timer to the processing timer every two seconds.
    QTimer* logProcessingTimer = new QTimer();
    connect(logProcessingTimer, SIGNAL(timeout()), &getLogger(), SLOT(processQueuedMessages()));
    logProcessingTimer->start(2000);
    getLogger().enableMessageQueue();

 * \endcode
 *
 ***************************************************************************/

class SimpleLoggerADP : public QObject
{
  Q_OBJECT

public:
  /*! \brief Default constructor */
  explicit SimpleLoggerADP(QObject *parent = 0);

  /*! \brief Destructor */
  ~SimpleLoggerADP();

  /*! \brief Clear all routing, so no log messages will be printed. This removes all loggers / appenders. */
  void clearRouting();

  //**************************************************************************
  /*! \brief Add a logger / appender that actually outputs messages.
   *
   *  \param [in] routing The logger / appender to add.
   ***************************************************************************/
  void addRouting(const SimpleLoggerRoutingInfo& routing);
  void addRouting(const QList<SimpleLoggerRoutingInfo>& routings);

  QList<SimpleLoggerRoutingInfo> & getRouting() {return m_routing;}
  const QList<SimpleLoggerRoutingInfo> & getRouting() const {return m_routing;}

  //**************************************************************************
  /*! \brief Set the log file name,
   *
   *  \param [in] logFileName Filename to use if writing to a logfile.
   ***************************************************************************/
  void setFileName(const QString& logFileName);

  //**************************************************************************
  /*! \brief Get the log filename, if any.
   *
   *  \return File name for the log file that will be used if a routing object specifies to send output to a file.
   ***************************************************************************/
  QString getFileName() const;

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
  /*! \brief Process the single message, which means send it to appropriate log outputs (file, qDebug, and emit).
   *
   *  \param [in] message is the message to log.
   ***************************************************************************/
  void processOneMessage(const LogMessageContainer& message);

  //**************************************************************************
  /*! \brief Create a message queue (if one does not already exist).
   ***************************************************************************/
  void enableMessageQueue();

  //**************************************************************************
  /*! \brief Get the number of error messages logged in a non thread safe way.
   ***************************************************************************/
  long errorCount() const { return m_numErrors; }

  //**************************************************************************
  /*! \brief Clear the number of errors logged in a non thread safe way.
   ***************************************************************************/
  void clearErrorCount() { m_numErrors = 0; }

signals:
  void formattedMessage(const QString& formattedMessage, SimpleLoggerRoutingInfo::MessageCategory category);

public slots:
  //**************************************************************************
  /*! \brief Receive a message and log it based on the routings.
   *
   *  Messages are cached and processed in another thread rather than being written immediately
   *  when they are received.
   *
   *  \param [in] message Primary message to log.
   *  \param [in] location Location provided compiler macros; usually file name and line number.
   *  \param [in] dateTime Date and time the message was created.
   *  \param [in] category Message category flags such as error, warning, informational, etc.
   *  \param [in] level Severity level. Lower is more sever, so level 100 is less likely to be logged than 3.
   ***************************************************************************/
  void receiveMessage(const QString& message, const QString& location, const QDateTime dateTime, const SimpleLoggerRoutingInfo::MessageCategory category, const int level);

  //**************************************************************************
  /*! \brief Log all of the messages currently in the message queue.
   ***************************************************************************/
  void processQueuedMessages();

private:

  //**************************************************************************
  /*! \brief Check to see if the logger is currently processing messages.
   ***************************************************************************/
  bool isProcessing();

  void readInternals(QXmlStreamReader& reader, const QString& version);

  //**************************************************************************
  /*! \brief Used to track the number of error messages received.
   *  This is used to attempt to track a large number of errors while creating a backup.
   ***************************************************************************/
  long m_numErrors;

  //**************************************************************************
  /*! \brief Allow incoming messages to be queued so that the text box is not flooded.
   ***************************************************************************/
  LogMessageQueue* m_messageQueue;

  //**************************************************************************
  /*! \brief Logs written to a file are written here!
   ***************************************************************************/
  QFile* m_logFile;

  //**************************************************************************
  /*! \brief Stream used to write to a log file.
   ***************************************************************************/
  QTextStream* m_textStream;

  //**************************************************************************
  /*! \brief List of routing objects.
   ***************************************************************************/
  QList<SimpleLoggerRoutingInfo> m_routing;

  //**************************************************************************
  /*! \brief When writing logs to a file, this is where the files are written.
   ***************************************************************************/
  QString m_logFileName;

  //**************************************************************************
  /*! \brief This is locked while processing messages
   * \sa processQueuedMessages
   ***************************************************************************/
  mutable QMutex m_processingMutex;
};

inline QXmlStreamWriter& operator<<(QXmlStreamWriter& writer, SimpleLoggerADP& logger)
{
  return logger.write(writer);
}

inline QXmlStreamReader& operator>>(QXmlStreamReader& reader, SimpleLoggerADP& logger)
{
  return logger.read(reader);
}


#endif // SIMPLELOGGERADP_H
