#include <QApplication>
#include "linkbackupadp.h"
#include "linkbackupglobals.h"
#include "simpleloggeradp.h"
#include <QFile>
#include <QTimer>
#include <QLoggingCategory>
#include <iostream>

static CopyLinkUtil globalCopyLinkUtil;

SimpleLoggerADP logger;
QtEnumMapper enumMapper;

void configureTheLogger();

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Ignore the qt events but allow my events to still be printed.
    if (type == QtDebugMsg && context.category != nullptr) {
      QString catStr(context.category);
      if (catStr.startsWith("qt.")) {
        return;
      }
    }

    // consider these:
    //const char *file = context.file ? context.file : "";
    //const char *function = context.function ? context.function : "";

    // Write the date of recording
    std::cout << qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz "));
    // By type determine to what level belongs message
    switch (type)
    {
    case QtInfoMsg:     std::cout << "INF "; break;
    case QtDebugMsg:    std::cout << "DBG "; break;
    case QtWarningMsg:  std::cout << "WRN "; break;
    case QtCriticalMsg: std::cout << "CRT "; break;
    case QtFatalMsg:    std::cout << "FTL "; break;
    }
    // Write to the output category of the message and the message itself
    std::cout << context.category << ": " << qPrintable(msg) << std::endl;
}

int main(int argc, char *argv[])
{
  // Use a 24MB buffer
  globalCopyLinkUtil.setBufferSize(1024*1024*24);

  QApplication a(argc, argv);
  // qInstallMessageHandler(messageHandler);

  // Fedora disables qDebug output at the moment
  // New configuration thing, so, re-enable it as follows (if you want).
  // But, you do NOT want all those mouse movement messages
  QLoggingCategory::setFilterRules("*.debug=true\n"
                                   "qt.gui.*=false\n"
                                   "qt.qpa.*=false\n"
                                   "qt.pointer*.debug=false\n"
                                   "qt.accessibility*.debug=false\n"
                                   "qt.text.layout*.debug=false\n"
                                   "qt.text*.debug=false\n"
                                   "qt.svg.timing=false\n"
                                   "qt.widgets.gestures*.debug=false");



  QCoreApplication::setOrganizationDomain("pitonyak.org");
  QCoreApplication::setOrganizationName("Pitonyak");
  QCoreApplication::setApplicationName("Link Backup ADP");
  QCoreApplication::setApplicationVersion("1.0.2");

  LinkBackupADP w;
  qRegisterMetaType<SimpleLoggerRoutingInfo::MessageCategory>( "SimpleLoggerRoutingInfo::MessageCategory" );

  configureTheLogger();

  // Connect the logger emit messages to the link backup software, which will show the log on screen.
  QObject::connect(&logger, SIGNAL(formattedMessage(const QString&, SimpleLoggerRoutingInfo::MessageCategory)), &w, SLOT(formattedMessage(const QString&, SimpleLoggerRoutingInfo::MessageCategory)));

  w.show();
  return a.exec();
}

void configureTheLogger()
{
    QFile file("/andrew0/home/andy/logger.xml");
    if (file.open(QIODevice::ReadOnly)) {
      QXmlStreamReader reader(&file);
      reader >> logger;
      file.close();
    }

    int logLevel = 1;

    // Add routing specific to the display. Keep it smaller so that it does not fill up.
    SimpleLoggerRoutingInfo routing_01;
    //routing_01.addMessageFormat(SimpleLoggerRoutingInfo::DateTimeComponent, "");
    //routing_01.addMessageFormat(SimpleLoggerRoutingInfo::StraightText, " ");

    // A string of length 1 forces just a single letter to be printed rather than the entire string.
    routing_01.addMessageFormat(SimpleLoggerRoutingInfo::MessageType, "X");
    routing_01.addMessageFormat(SimpleLoggerRoutingInfo::ConstantText, " ");
    routing_01.addMessageFormat(SimpleLoggerRoutingInfo::MessageLocation, "");
    routing_01.addMessageFormat(SimpleLoggerRoutingInfo::ConstantText, " | ");
    routing_01.addMessageFormat(SimpleLoggerRoutingInfo::MessageText, "");

    //routing_01.setCategoryLevel(SimpleLoggerRoutingInfo::TraceMessage, logLevel);
    routing_01.setCategoryLevel(SimpleLoggerRoutingInfo::DebugMessage, logLevel);
    //routing_01.setCategoryLevel(SimpleLoggerRoutingInfo::InformationMessage, logLevel);
    routing_01.setCategoryLevel(SimpleLoggerRoutingInfo::UserMessage, logLevel);
    routing_01.setCategoryLevel(SimpleLoggerRoutingInfo::WarningMessage, logLevel);
    routing_01.setCategoryLevel(SimpleLoggerRoutingInfo::ErrorMessage, logLevel);

    routing_01.setRoutingOn(SimpleLoggerRoutingInfo::RouteEmit);
    logger.addRouting(routing_01);

    // Messages for the log file and also for the console (through QDebug).
    SimpleLoggerRoutingInfo routing_02;
    routing_02.addMessageFormat(SimpleLoggerRoutingInfo::MessageDateTime, "");
    routing_02.addMessageFormat(SimpleLoggerRoutingInfo::ConstantText, " ");
    routing_02.addMessageFormat(SimpleLoggerRoutingInfo::MessageType, "X");
    routing_02.addMessageFormat(SimpleLoggerRoutingInfo::ConstantText, " ");
    routing_02.addMessageFormat(SimpleLoggerRoutingInfo::MessageLocation, "");
    routing_02.addMessageFormat(SimpleLoggerRoutingInfo::ConstantText, " | ");
    routing_02.addMessageFormat(SimpleLoggerRoutingInfo::MessageText, "");

    logLevel = 1;

    routing_02.setCategoryLevel(SimpleLoggerRoutingInfo::TraceMessage, logLevel);
    routing_02.setCategoryLevel(SimpleLoggerRoutingInfo::DebugMessage, logLevel);
    routing_02.setCategoryLevel(SimpleLoggerRoutingInfo::InformationMessage, logLevel);
    routing_02.setCategoryLevel(SimpleLoggerRoutingInfo::WarningMessage, logLevel);
    routing_02.setCategoryLevel(SimpleLoggerRoutingInfo::ErrorMessage, logLevel);
    routing_02.setCategoryLevel(SimpleLoggerRoutingInfo::UserMessage, logLevel);
    routing_02.setRoutingOn(SimpleLoggerRoutingInfo::RouteFile | SimpleLoggerRoutingInfo::RouteQDebug);

    logger.addRouting(routing_02);

    // Example creating a log configuration file
    /**
    QFile file3("/home/andy/logger3.xml");
    if (file3.open(QIODevice::WriteOnly)) {
      QXmlStreamWriter writer(&file3);
      writer.setAutoFormatting(true);
      writer << logger;
      file3.close();
    }
    **/
}


//**************************************************************************

CopyLinkUtil& getCopyLinkUtil()
{
  return globalCopyLinkUtil;
}


QtEnumMapper& getEnumMapper()
{
  return enumMapper;
}

//**************************************************************************
//**
//** Logging helpers.
//**
//**************************************************************************
SimpleLoggerADP& getLogger()
{
  return logger;
}

void errorMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level)
{
  logger.receiveMessage(message, location, dateTime, SimpleLoggerRoutingInfo::ErrorMessage, level);
}

void warnMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level)
{
  logger.receiveMessage(message, location, dateTime, SimpleLoggerRoutingInfo::WarningMessage, level);
}

void infoMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level)
{
  logger.receiveMessage(message, location, dateTime, SimpleLoggerRoutingInfo::InformationMessage, level);
}

void traceMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level)
{
  logger.receiveMessage(message, location, dateTime, SimpleLoggerRoutingInfo::TraceMessage, level);
}

void debugMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level)
{
  logger.receiveMessage(message, location, dateTime, SimpleLoggerRoutingInfo::DebugMessage, level);
}

void userMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level)
{
    logger.receiveMessage(message, location, dateTime, SimpleLoggerRoutingInfo::UserMessage, level);
}
