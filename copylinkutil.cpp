#include "copylinkutil.h"
#include "enhancedqcryptographichash.h"

#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <unistd.h>  // Contains the "link" method.

// Report every 2GB of data.
qint64 CopyLinkUtil::s_readReportBytes = 2L * 1024L * 1024L * 1024L;

CopyLinkUtil::CopyLinkUtil() : m_bytesCopied(0), m_bytesLinked(0), m_bytesHashed(0), m_bytesCopiedHashed(0), m_millisCopied(0), m_millisLinked(0), m_millisHashed(0), m_millisCopiedHashed(0), m_buffer(nullptr), m_bufferSize(0), m_hashGenerator(nullptr), m_timer(nullptr), m_cancelRequested(false), m_useHardLink(true), m_hashMethod(EnhancedQCryptographicHash::getDefaultAlgorithm())
{
  m_timer = new QElapsedTimer();
}

// I can only do this because I am using C++11, otherwise I could not use the default constructor.
CopyLinkUtil::CopyLinkUtil(const CopyLinkUtil& obj) : m_bytesCopied(obj.m_bytesCopied), m_bytesLinked(obj.m_bytesLinked), m_bytesHashed(obj.m_bytesHashed), m_bytesCopiedHashed(obj.m_bytesCopiedHashed), m_millisCopied(obj.m_millisCopied), m_millisLinked(obj.m_millisLinked), m_millisHashed(obj.m_millisHashed), m_millisCopiedHashed(obj.m_millisCopiedHashed), m_buffer(nullptr), m_bufferSize(0), m_hashGenerator(nullptr), m_timer(nullptr), m_cancelRequested(false), m_useHardLink(true), m_hashMethod(obj.m_hashMethod)
{
  m_timer = new QElapsedTimer();
  if (obj.m_hashGenerator != nullptr)
  {
    setHashType(m_hashMethod);
  }
  setBufferSize(obj.m_bufferSize);
}


CopyLinkUtil::CopyLinkUtil(const QString& hashType) : CopyLinkUtil()
{
  setHashType(hashType);
}

CopyLinkUtil::CopyLinkUtil(const QString& hashType, qint64 bufferSize) : CopyLinkUtil()
{
  setHashType(hashType);
  setBufferSize(bufferSize);
}

CopyLinkUtil::CopyLinkUtil(qint64 bufferSize) : CopyLinkUtil()
{
  setBufferSize(bufferSize);
}

CopyLinkUtil::~CopyLinkUtil()
{
  if (m_buffer != nullptr)
  {
    delete[] m_buffer;
    m_buffer = nullptr;
  }
  if (m_timer != nullptr)
  {
    delete m_timer;
    m_timer = nullptr;
  }
  if (m_hashGenerator != nullptr)
  {
    delete m_hashGenerator;
    m_hashGenerator = nullptr;
  }
}

void CopyLinkUtil::resetStats()
{
    m_bytesCopied = 0;
    m_bytesLinked = 0;
    m_bytesHashed = 0;
    m_bytesCopiedHashed =0;
    m_millisCopied = 0;
    m_millisLinked = 0;
    m_millisHashed = 0;
    m_millisCopiedHashed = 0;
    if (m_timer != nullptr)
    {
      delete m_timer;
      m_timer = nullptr;
    }
    m_timer = new QElapsedTimer();
    m_cancelRequested = false;
}

qint64 CopyLinkUtil::getBytesCopied() const
{
  return m_bytesCopied;
}

qint64 CopyLinkUtil::getBytesLinked() const
{
  return m_bytesLinked;
}

qint64 CopyLinkUtil::getBytesHashed() const
{
  return m_bytesHashed;
}

qint64 CopyLinkUtil::getBytesCopiedHashed() const
{
  return m_bytesCopiedHashed;
}

qint64 CopyLinkUtil::getMillisCopied() const
{
  return m_millisCopied;
}

qint64 CopyLinkUtil::getMillisLinked() const
{
  return m_millisLinked;
}

qint64 CopyLinkUtil::getMillisHashed() const
{
  return m_millisHashed;
}

qint64 CopyLinkUtil::getMillisCopiedHashed() const
{
  return m_millisCopiedHashed;
}

bool CopyLinkUtil::setBufferSize(const qint64 bufferSize)
{
  if (m_buffer != nullptr)
  {
    delete[] m_buffer;
    m_buffer = nullptr;
    m_bufferSize = 0;
  }
  if (bufferSize > 0)
  {
    m_buffer = new char[bufferSize];
    if (m_buffer != nullptr)
    {
      m_bufferSize = bufferSize;
    }
    else
    {
      return false;
    }
  }
  return true;
}

bool CopyLinkUtil::setHashType(const QString& hashType)
{
  bool ok;
  QCryptographicHash::Algorithm algorithm = EnhancedQCryptographicHash::toAlgorithm(hashType, &ok);
  if (ok)
  {
    setHashType(algorithm);
  }
  else
  {
    delete m_hashGenerator;
    m_hashGenerator = nullptr;
    qDebug(qPrintable(QString("Unsupported hash type %1").arg(hashType)));
    return false;
  }
  return true;
}


bool CopyLinkUtil::setHashType(QCryptographicHash::Algorithm hashType)
{
  if (m_hashGenerator != nullptr)
  {
    delete m_hashGenerator;
    m_hashGenerator = nullptr;
  }
  m_hashGenerator = new EnhancedQCryptographicHash(hashType);
  return true;
}




bool CopyLinkUtil::linkFile(const QString& linkToThisFile, const QString& placeLinkHere)
{
  bool noError = true;
  QFile existingFile(linkToThisFile);
  m_timer->restart();
  if (isUseHardLink())
  {
    noError = link(qPrintable(linkToThisFile), qPrintable(placeLinkHere)) == 0;
  }
  else
  {
    noError = existingFile.link(placeLinkHere);
  }
  if (noError)
  {
    m_millisLinked += m_timer->elapsed();
    m_bytesLinked += existingFile.size();
  }
  return noError;
}

bool CopyLinkUtil::copyFile(const QString& copyFromPath, const QString& copyToPath)
{
  return internalCopyFile(copyFromPath, copyToPath, false);
}

bool CopyLinkUtil::generateHash(const QString& copyFromPath)
{
  QFile fileToRead(copyFromPath);

  QFileInfo fileInfo(copyFromPath);
  qint64 lastReportByteCount = 0;

  if (isCancelRequested())
  {
    return false;
  }

  if (!fileToRead.exists() || !fileToRead.open(QIODevice::ReadOnly))
  {
    qDebug(qPrintable(QString("Failed to open file to read: %1").arg(copyFromPath)));
    return false;
  }

  m_timer->restart();
  m_hashGenerator->reset();
  qint64 totalRead = 0;
  qint64 numRead = fileToRead.read(m_buffer, m_bufferSize);
  while (numRead > 0 && fileToRead.error() == QFile::NoError && !isCancelRequested())
  {
    totalRead += numRead;
    m_hashGenerator->addData(m_buffer, numRead);
    if (totalRead - lastReportByteCount > s_readReportBytes)
    {
      lastReportByteCount =  totalRead;
      // TODO: Log message here
      qDebug(qPrintable(QString("Read %1/%2 from %3").arg(getBPS(totalRead, 0)).arg(fileToRead.size()).arg(fileInfo.fileName())));
    }
    numRead = fileToRead.read(m_buffer, m_bufferSize);
  }
  if (fileToRead.error() != QFile::NoError || isCancelRequested())
  {
    fileToRead.close();
  }
  else
  {
    m_millisHashed += m_timer->elapsed();
    m_bytesHashed += totalRead;
    fileToRead.close();
    return true;
  }
  return false;
}

bool CopyLinkUtil::copyFileGenerateHash(const QString& copyFromPath, const QString& copyToPath)
{
  return internalCopyFile(copyFromPath, copyToPath, true);
}

bool CopyLinkUtil::internalCopyFile(const QString& copyFromPath, const QString& copyToPath, const bool doHash)
{
  //qDebug(qPrintable(QString("Ready to read from : %1").arg(copyFromPath)));
  //qDebug(qPrintable(QString("Ready to write to  : %1").arg(copyToPath)));
  QFile fileToRead(copyFromPath);

  QFileInfo fileInfo(copyFromPath);
  qint64 lastReportByteCount = 0;

  QFile fileToWrite(copyToPath);
  QFileInfo fileInfoFileToWrite(copyToPath);
  QString pathToFileToWrite = fileInfoFileToWrite.absolutePath();

  QDir dirFileToWrite(pathToFileToWrite);
  if (!dirFileToWrite.exists(pathToFileToWrite) && !dirFileToWrite.mkpath(pathToFileToWrite))
  {
    // Failed to create the path to the file.
    // Generate error and get out.
    qDebug(qPrintable(QString("Failed to create backup directory %1").arg(pathToFileToWrite)));
    return false;
  }

  if (doHash)
  {
    m_hashGenerator->reset();
  }

  if (fileToWrite.exists()) {
    qDebug(qPrintable(QString("File to write does exist: %1").arg(copyToPath)));
    return false;
  }

  if (isCancelRequested())
  {
    qDebug("Cancel Requested, ending copy.");
    return false;
  }

  if (!fileToWrite.open(QIODevice::WriteOnly))
  {
    qDebug(qPrintable(QString("Failed to open File to write : %1").arg(copyToPath)));
    return false;
  }

  if (!fileToRead.exists() || !fileToRead.open(QIODevice::ReadOnly))
  {
    qDebug(qPrintable(QString("Failed to open file to read: %1").arg(copyFromPath)));
    fileToWrite.close();
    fileToWrite.remove();
    return false;
  }

  m_timer->restart();
  qint64 totalRead = 0;
  qint64 numRead = fileToRead.read(m_buffer, m_bufferSize);
  while (numRead > 0 && fileToRead.error() == QFile::NoError && fileToWrite.error() == QFile::NoError && !isCancelRequested())
  {
    totalRead += numRead;
    fileToWrite.write(m_buffer, numRead);
    if (doHash)
    {
      m_hashGenerator->addData(m_buffer, numRead);
    }
    if (totalRead - lastReportByteCount > s_readReportBytes)
    {
      lastReportByteCount =  totalRead;
      // TODO: Log message here
      qDebug(qPrintable(QString("Copied %1/%2 from %3").arg(getBPS(totalRead, 0)).arg(fileToRead.size()).arg(fileInfo.fileName())));
    }
    numRead = fileToRead.read(m_buffer, m_bufferSize);

  }
  if (fileToRead.error() != QFile::NoError || fileToWrite.error() != QFile::NoError || isCancelRequested())
  {
    qDebug(qPrintable(QString("Removing file because error encountered : %1").arg(copyToPath)));
    fileToWrite.close();
    fileToRead.close();
    fileToWrite.remove();
  }
  else
  {
    if (doHash)
    {
      m_millisCopiedHashed += m_timer->elapsed();
      m_bytesCopiedHashed += totalRead;
    }
    else
    {
      m_millisCopied += m_timer->elapsed();
      m_bytesCopied += totalRead;
    }
    fileToWrite.close();
    fileToRead.close();
    fileToWrite.setPermissions(fileToRead.permissions());
    return true;
  }
  return false;
}

QString CopyLinkUtil::getLastHash() const
{
  return m_hashGenerator->result().toHex().toUpper();
}

QString CopyLinkUtil::getStats() const
{
  QStringList sList;
  if (getBytesCopied() > 0)
  {
    sList.append(QString("%1 Copied at %2").arg(getBPS(getBytesCopied(), 0), getBPS(getBytesCopied(), getMillisCopied())));
  }
  if (getBytesHashed() > 0)
  {
    sList.append(QString("%1 Hashed at %2").arg(getBPS(getBytesHashed(), 0), getBPS(getBytesHashed(), getMillisHashed())));
  }
  if (getBytesCopiedHashed() > 0)
  {
    sList.append(QString("%1 Copied and Hashed simultaneously at %2").arg(getBPS(getBytesCopiedHashed(), 0), getBPS(getBytesCopiedHashed(), getMillisCopiedHashed())));
  }
  if (getBytesLinked() > 0)
  {
    sList.append(QString("%1 Linked in %2 seconds").arg(getBPS(getBytesLinked(), 0), QString::number(getMillisLinked() / 1000)));
  }
  sList.append(QString("%1 total copied and %2 total read (copied and hashed)").arg(getBPS(getBytesCopiedHashed() + getBytesCopied(), 0), getBPS(getBytesCopiedHashed() + getBytesCopied() + getBytesHashed(), 0)));
  QString s;
  for (int i=0; i<sList.count(); ++i)
  {
    if (i == 0)
    {
      s = sList[i];
    }
    else
    {
      s = QString("%1\n%2").arg(s, sList[i]);
    }
  }
  return s;
}

QString CopyLinkUtil::getBPS(const qint64 numBytes, const qint64 millis)
{
  if (numBytes == 0)
  {
    return QString("0 Bytes");
  }

  double dBytesPerSecond = (millis != 0) ? 1000.0 * numBytes / millis : numBytes;

  int i=0;
  const char* terms[] = {"B", "KB", "MB", "GB", "TB", "Petabytes", "Exabytes", "Zettabyte", "Yottabyte", "Brontobyte", "Geopbyte", nullptr};
  while (terms[i+1] != nullptr && dBytesPerSecond > 1024)
  {
    ++i;
    dBytesPerSecond /= 1024;
  }
  if (millis == 0)
  {
    return QString("%1 %2").arg(QString::number((qint64)dBytesPerSecond), terms[i]);
  }
  else
  {
    return QString("%1 %2/sec").arg(QString::number((qint64)dBytesPerSecond), terms[i]);
  }
}


