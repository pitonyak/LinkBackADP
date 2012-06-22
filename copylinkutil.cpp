#include "copylinkutil.h"

#include <QCryptographicHash>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QDir>

CopyLinkUtil::CopyLinkUtil() : m_bytesCopied(0), m_bytesLinked(0), m_bytesHashed(0), m_bytesCopiedHashed(0), m_millisCopied(0), m_millisLinked(0), m_millisHashed(0), m_millisCopiedHashed(0), m_buffer(nullptr), m_bufferSize(0), m_hashGenerator(nullptr), m_timer(nullptr), m_cancelRequested(false), m_useHardLink(true)
{
  m_timer = new QElapsedTimer();
}

CopyLinkUtil::CopyLinkUtil(const QString& hashType) : m_bytesCopied(0), m_bytesLinked(0), m_bytesHashed(0), m_bytesCopiedHashed(0), m_millisCopied(0), m_millisLinked(0), m_millisHashed(0), m_millisCopiedHashed(0), m_buffer(nullptr), m_bufferSize(0), m_hashGenerator(nullptr), m_timer(nullptr), m_cancelRequested(false), m_useHardLink(true)
{
  m_timer = new QElapsedTimer();
  setHashType(hashType);
}

CopyLinkUtil::CopyLinkUtil(const QString& hashType, qint64 bufferSize) : m_bytesCopied(0), m_bytesLinked(0), m_bytesHashed(0), m_bytesCopiedHashed(0), m_millisCopied(0), m_millisLinked(0), m_millisHashed(0), m_millisCopiedHashed(0), m_buffer(nullptr), m_bufferSize(0), m_hashGenerator(nullptr), m_timer(nullptr), m_cancelRequested(false), m_useHardLink(true)
{
  m_timer = new QElapsedTimer();
  setHashType(hashType);
  setBufferSize(bufferSize);
}

CopyLinkUtil::CopyLinkUtil(qint64 bufferSize) : m_bytesCopied(0), m_bytesLinked(0), m_bytesHashed(0), m_bytesCopiedHashed(0), m_millisCopied(0), m_millisLinked(0), m_millisHashed(0), m_millisCopiedHashed(0), m_buffer(nullptr), m_bufferSize(0), m_hashGenerator(nullptr), m_timer(nullptr), m_cancelRequested(false), m_useHardLink(true)
{
  m_timer = new QElapsedTimer();
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

bool CopyLinkUtil::setBufferSize(qint64 bufferSize)
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
  if (m_hashGenerator != nullptr)
  {
    delete m_hashGenerator;
    m_hashGenerator = nullptr;
  }
  if (hashType.compare("SHA1", Qt::CaseInsensitive) == 0)
  {
    m_hashGenerator = new QCryptographicHash(QCryptographicHash::Sha1);
  }
  else if (hashType.compare("MD5", Qt::CaseInsensitive) == 0)
  {
    m_hashGenerator = new QCryptographicHash(QCryptographicHash::Md5);
  }
  else if (hashType.compare("MD4", Qt::CaseInsensitive) == 0)
  {
    m_hashGenerator = new QCryptographicHash(QCryptographicHash::Md4);
  }
  else
  {
    qDebug(qPrintable(QString("Unsupported hash type %1").arg(hashType)));
    return false;
  }
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

bool CopyLinkUtil::internalCopyFile(const QString& copyFromPath, const QString& copyToPath, bool doHash)
{
  QFile fileToRead(copyFromPath);

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
    numRead = fileToRead.read(m_buffer, m_bufferSize);
  }
  if (fileToRead.error() != QFile::NoError || fileToWrite.error() != QFile::NoError || isCancelRequested())
  {
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
  if (m_bytesCopied > 0)
  {
    sList.append(QString("%1 Copied at %2").arg(getBPS(m_bytesCopied, 0), getBPS(m_bytesCopied, m_millisCopied)));
  }
  if (m_bytesHashed > 0)
  {
    sList.append(QString("%1 Hashed at %2").arg(getBPS(m_bytesHashed, 0), getBPS(m_bytesHashed, m_millisHashed)));
  }
  if (m_bytesCopiedHashed > 0)
  {
    sList.append(QString("%1 Copied and Hashed simultaneously at %2").arg(getBPS(m_bytesCopiedHashed, 0), getBPS(m_bytesCopiedHashed, m_millisCopiedHashed)));
  }
  if (m_bytesLinked > 0)
  {
    sList.append(QString("%1 Linked in %2 seconds").arg(getBPS(m_bytesLinked, 0), QString::number(m_millisLinked / 1000)));
  }
  sList.append(QString("%1 total copied and %2 total read").arg(getBPS(m_bytesCopiedHashed + m_bytesCopied, 0), getBPS(m_bytesCopiedHashed + m_bytesCopied + m_bytesHashed, 0)));
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

QString CopyLinkUtil::getBPS(qint64 bytesCopied, qint64 millis) const
{
  if (bytesCopied == 0)
  {
    return QString("0 Bytes").arg(QString::number(bytesCopied));
  }

  double dBytesPerSecond = (millis != 0) ? 1000.0 * bytesCopied / millis : bytesCopied;

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


