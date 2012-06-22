#ifndef GLOBALCOPYLINEUTIL_H
#define GLOBALCOPYLINEUTIL_H

#include "copylinkutil.h"
#include <QDateTime>

CopyLinkUtil& getCopyLinkUtil();

#define ERROR_MSG(msg, level) errorMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__ , QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
#define WARN_MSG(msg, level) warnMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
#define INFO_MSG(msg, level) informationMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
#define TRACE_MSG(msg, level) traceMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
#define DEBUG_MSG(msg, level) debugMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));


void errorMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);
void warnMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);
void informationMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);
void traceMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);
void debugMessage(const QString& message, const QString& location, const QDateTime& dateTime, int level=1);

#endif // GLOBALCOPYLINEUTIL_H
