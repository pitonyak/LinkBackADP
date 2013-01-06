#ifndef SIMPLELOGGERADP_H
#define SIMPLELOGGERADP_H

#include "simpleloggerroutinginfo.h"
#include <QObject>
#include <QDateTime>

class QTextStream;
class QFile;

//**************************************************************************
//! TODO:
/*!
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

class SimpleLoggerADP : public QObject
{
  Q_OBJECT

public:
  explicit SimpleLoggerADP(QObject *parent = 0);
  ~SimpleLoggerADP();

  void clearRouting();
  void addRouting(const SimpleLoggerRoutingInfo& routing);

  void setFileName(const QString& logFileName);
  QString getFileName() const;

  QXmlStreamWriter& write(QXmlStreamWriter& writer) const;
  QXmlStreamReader& read(QXmlStreamReader& reader);

signals:
  void formattedMessage(const QString& formattedMessage, SimpleLoggerRoutingInfo::MessageCategory category);

public slots:
  void receiveMessage(const QString& message, const QString& location, const QDateTime dateTime, SimpleLoggerRoutingInfo::MessageCategory category, int level);

private:
  void readInternals(QXmlStreamReader& reader, const QString& version);
  QFile* m_logFile;
  QTextStream* m_textStream;
  QList<SimpleLoggerRoutingInfo> m_routing;
  QString m_logFileName;
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
