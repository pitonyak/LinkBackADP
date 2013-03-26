#ifndef GLOBALCOPYLINEUTIL_H
#define GLOBALCOPYLINEUTIL_H

#include "copylinkutil.h"
#include "simpleloggeradp.h"
#include <QDateTime>

//**************************************************************************
/*! \brief Get the single global instance of the copy link file utility class.
 * There is one instance because only one backup is run at a time.
 * TODO: Move this into the thread so that multiple backups can run at the same time.
 *
 * \returns The single global instance of the copy link file utility class.
 ***************************************************************************/
CopyLinkUtil& getCopyLinkUtil();

//**************************************************************************
/*! \brief Get the single global instance of the logger.
 *
 * \returns The single global instance of the logger.
 ***************************************************************************/
SimpleLoggerADP &getLogger();

#define ERROR_MSG(msg, level) errorMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
#define WARN_MSG( msg, level) warnMessage((msg),  QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
#define INFO_MSG( msg, level) infoMessage((msg),  QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
#define TRACE_MSG(msg, level) traceMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
#define DEBUG_MSG(msg, level) debugMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
#define USER_MSG( msg, level) userMessage((msg),  QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));


void errorMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);
void warnMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);
void infoMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);
void traceMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);
void debugMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);
void userMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);

#endif // GLOBALCOPYLINEUTIL_H
