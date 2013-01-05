#ifndef COPYLINKUTIL_H
#define COPYLINKUTIL_H

#include <QString>

class QCryptographicHash;
class QElapsedTimer;


//**************************************************************************
//! Class to copy files, link files, and generate a hash value.
/*!
 * This class also accumulates statistics on how much data moved how quickly.
 **************************************************************************/
class CopyLinkUtil
{
public:
  /*! \brief Constructor. You must still set the hash type and buffer size. */
  CopyLinkUtil();

  /*! \brief Constructor. You must still set the buffer size before use.
   *
   *  \param [in] hashType Set the hash type for future hash operations.
   */
  CopyLinkUtil(const QString& hashType);

  /*! \brief Constructor.
   *
   *  \param [in] hashType Set the hash type for future hash operations.
   *  \param [in] bufferSize Set the buffer size for copying files and calculating hash values.
   */
  CopyLinkUtil(const QString& hashType, qint64 bufferSize);

  /*! \brief Constructor. You must still set the hash type before calculating a hash.
   *
   *  \param [in] bufferSize Set the buffer size for copying files and calculating hash values.
   */
  CopyLinkUtil(qint64 bufferSize);

  /*! \brief Destructor to delete things such as the internal buffer. */
  ~CopyLinkUtil();

  qint64 getBytesCopied() const;
  qint64 getBytesLinked() const;
  qint64 getBytesHashed() const;
  qint64 getBytesCopiedHashed() const;

  qint64 getMillisCopied() const;
  qint64 getMillisLinked() const;
  qint64 getMillisHashed() const;
  qint64 getMillisCopiedHashed() const;

  /*! \brief Create a read buffer.
   *
   *  \param [in] bufferSize Size of the read buffer.
   *  \return True if the command succeeds.
   */
  bool setBufferSize(qint64 bufferSize);
  bool setHashType(const QString& hashType);
  bool copyFile(const QString& copyFromPath, const QString& copyToPath);
  bool generateHash(const QString& copyFromPath);
  bool copyFileGenerateHash(const QString& copyFromPath, const QString& copyToPath);
  bool linkFile(const QString& linkToThisFile, const QString& placeLinkHere);

  QString getLastHash() const;

  bool isCancelRequested() const;
  void setCancelRequested(bool cancelRequested);

  /*! \brief Get a copy of the copy statistics as a string suitable for display to the user. */
  QString getStats() const;
  QString getBPS(qint64 bytesCopied, qint64 millis) const;

  /*! \brief Reset the stats for a new backup. */
  void resetStats();

  bool isUseHardLink() const;
  bool isUseSymbolicLink() const;

  void setUseHardLink();
  void setUseSymbolicLink();

private:

    bool internalCopyFile(const QString& copyFromPath, const QString& copyToPath, bool doHash);

    qint64 m_bytesCopied;
    qint64 m_bytesLinked;
    qint64 m_bytesHashed;
    qint64 m_bytesCopiedHashed;

    qint64 m_millisCopied;
    qint64 m_millisLinked;
    qint64 m_millisHashed;
    qint64 m_millisCopiedHashed;

    char* m_buffer;
    qint64 m_bufferSize;

    QCryptographicHash* m_hashGenerator;
    QElapsedTimer * m_timer;
    bool m_cancelRequested;
    bool m_useHardLink;
};

inline bool CopyLinkUtil::isCancelRequested() const
{
  return m_cancelRequested;
}

inline void CopyLinkUtil::setCancelRequested(bool cancelRequested)
{
  m_cancelRequested = cancelRequested;
}

inline bool CopyLinkUtil::isUseHardLink() const
{
  return m_useHardLink;
}

inline bool CopyLinkUtil::isUseSymbolicLink() const
{
  return !m_useHardLink;
}

inline void CopyLinkUtil::setUseHardLink()
{
  m_useHardLink = true;
}

inline void CopyLinkUtil::setUseSymbolicLink()
{
  m_useHardLink = false;
}


#endif // COPYLINKUTIL_H
