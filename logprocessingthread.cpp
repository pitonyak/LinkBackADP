#include "logprocessingthread.h"

LogProcessingThread::LogProcessingThread(QObject *parent) : QThread(parent), m_cancelRequested(false)
{
}

void LogProcessingThread::run()
{
}


void LogProcessingThread::requestCancel()
{
  m_cancelRequested = true;
}
