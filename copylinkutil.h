#ifndef COPYLINKUTIL_H
#define COPYLINKUTIL_H

#include <QString>
#include "enhancedqcryptographichash.h"

class QElapsedTimer;


//**************************************************************************
//! Class to copy files, link files, and generate a hash value.
/*!
 * This class also accumulates statistics on how much data moved how quickly.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 **************************************************************************/
class CopyLinkUtil
{
public:

    /*! \brief Provide trace output during long reads so that the user knows the software is not locked. This value tells how many bytes to read between reported reads. */
    static qint64 s_readReportBytes;

    /*! \brief Constructor. You must still set the hash type and buffer size. */
    CopyLinkUtil();

    /*! \brief Copy Constructor.*/
    CopyLinkUtil(const CopyLinkUtil& obj);

    //**************************************************************************
    /*! \brief Constructor. You must still set the buffer size before use.
     *
     *  \param [in] hashType Set the hash type for future hash operations.
     ***************************************************************************/
    CopyLinkUtil(const QString& hashType);

    //**************************************************************************
    /*! \brief Constructor.
     *
     *  \param [in] hashType Set the hash type for future hash operations.
     *  \param [in] bufferSize Set the buffer size for copying files and calculating hash values.
     ***************************************************************************/
    CopyLinkUtil(const QString& hashType, qint64 bufferSize);

    //**************************************************************************
    /*! \brief Constructor. You must still set the hash type before calculating a hash.
     *
     *  \param [in] bufferSize Set the buffer size for copying files and calculating hash values.
     ***************************************************************************/
    CopyLinkUtil(qint64 bufferSize);

    /*! \brief Destructor to delete things such as the internal buffer. */
    ~CopyLinkUtil();

    /*! \brief Get number of bytes copied during the backup. */
    qint64 getBytesCopied() const;

    /*! \brief Get number of bytes linked during the backup. */
    qint64 getBytesLinked() const;

    /*! \brief Get number of bytes hashed during the backup. */
    qint64 getBytesHashed() const;

    /*! \brief Get number of bytes copied and hashed (at the same time) during the backup. */
    qint64 getBytesCopiedHashed() const;

    /*! \brief Get number of milliseconds used to copy data. */
    qint64 getMillisCopied() const;

    /*! \brief Get number of milliseconds used to link data. */
    qint64 getMillisLinked() const;

    /*! \brief Get number of milliseconds used to hash data. */
    qint64 getMillisHashed() const;

    /*! \brief Get number of milliseconds used to copy and hash data at the same time. */
    qint64 getMillisCopiedHashed() const;

    //**************************************************************************
    /*! \brief Create a read buffer.
     *
     *  The buffer is resized as needed. Setting to less than 1 bytes leaves no buffer.
     *  \param [in] bufferSize Size of the read buffer.
     *  \return True if the command succeeds.
     ***************************************************************************/
    bool setBufferSize(const qint64 bufferSize);

    //**************************************************************************
    /*! \brief Set the hash type based on a string. Case does not matter.
     *
     *  Sadly, the hash type enumeration is not fully supported by the Qt meta-data stuff,
     *  I guess that the over-head was deemed "not worth it" for the containing class.
     *  The result is that I cannot use meta-data to match the supported hash types.
     *  Because of this, supported hash types are hard-coded and must be changed based
     *  on the Qt version.
     *
     *  \param [in] hashType is a string representation of the desired hash type.
     *  \return True if the command succeeds.
     ***************************************************************************/
    bool setHashType(const QString& hashType);

    //**************************************************************************
    /*! \brief Set the hash type based on the Qt enumerated type.
     *
     *  \param [in] hashType of the desired hash type.
     *  \return True if the command succeeds.
     ***************************************************************************/
    bool setHashType(QCryptographicHash::Algorithm hashType);

    //**************************************************************************
    /*! \brief Copy a file without calculating the hash.
     *
     *  \param [in] copyFromPath Full path to an existing file.
     *  \param [in] copyToPath Full path to where the file will be copied.
     *  \return True on success, false otherwise.
     *  \sa CopyLinkUtil::internalCopyFile()
     ***************************************************************************/
    bool copyFile(const QString& copyFromPath, const QString& copyToPath);

    //**************************************************************************
    /*! \brief Generate the hash value for a file. Call getLastHash() to get the hash value.
     *
     *  \param [in] copyFromPath Full path to an existing file.
     *  \return True on success, false otherwise.
     *  \sa CopyLinkUtil::getLastHash()
     ***************************************************************************/
    bool generateHash(const QString& copyFromPath);

    //**************************************************************************
    /*! \brief Copy a file and calculate the hash at the same time. Call getLastHash() to get the hash value.
     *
     *  \param [in] copyFromPath Full path to an existing file.
     *  \param [in] copyToPath Full path to where the file will be copied.
     *  \return True on success, false otherwise.
     *  \sa CopyLinkUtil::internalCopyFile()
     *  \sa CopyLinkUtil::getLastHash()
     ***************************************************************************/
    bool copyFileGenerateHash(const QString& copyFromPath, const QString& copyToPath);

    //**************************************************************************
    /*! \brief Create a link to an existing file.
     *
     *  The "use hard link" property in this object determines if a hard or soft link is created.
     *  The default is to use a hard link rather than a soft link.
     *
     *  \param [in] linkToThisFile Full path to an existing file. A link will reference this file.
     *  \param [in] placeLinkHere Full path to where the link will be created.
     *  \return True on success, false otherwise.
     ***************************************************************************/
    bool linkFile(const QString& linkToThisFile, const QString& placeLinkHere);

    /*! \brief Get a upper-case representation of the current hash generator. */
    QString getLastHash() const;

    /*! \brief Get the "cancel requested" flag. */
    bool isCancelRequested() const;
    /*! \brief Set the "cancel requested" flag. */
    void setCancelRequested(bool cancelRequested);

    //**************************************************************************
    /*! \brief Get a copy of the copy statistics as a string suitable for display to the user.
     *  Contains Bytes Per Second copied, hashed, and copied/hashed. Also contains total bytes linked.
     *  \return Multi-line String representation of the statistics.
     ***************************************************************************/
    QString getStats() const;

    //**************************************************************************
    /*! \brief Return a formatted string such as "8 MB/sec" representing "Bytes Per Second".
     * Units are selected such that the number is less than 1024.
     *
     *  \param [in] numBytes is the number of bytes.
     *  \param [in] millis is the number of elapsed milliseconds.
     *  \return Formatted string representing "Bytes Per Second".
     ***************************************************************************/
    static QString getBPS(const qint64 numBytes, const qint64 millis);

    /*! \brief Reset the stats for a new backup. */
    void resetStats();

    /*! \brief Returns True if a hardlink is used to link to duplicate files, and false if a symbolic link is used. Should always be a hard link. */
    bool isUseHardLink() const;

    /*! \brief Returns True if a softlink is used to link to duplicate files, and false if a hard link is used. Should always be a hard link. */
    bool isUseSymbolicLink() const;

    /*! \brief Set to use a hardlink between duplicate files. */
    void setUseHardLink();

    /*! \brief Set to use a symbolic link etween duplicate files. */
    void setUseSymbolicLink();

private:

    //**************************************************************************
    /*! \brief Copy a file and calculate the hash (if requested) at the same time. Call getLastHash() to get the hash value.
     *
     *  \param [in] copyFromPath Full path to an existing file.
     *  \param [in] copyToPath Full path to where the file will be copied.
     *  \param [in] doHash determines if the file's hash is calculated while the file is copied.
     *  \return True on success, false otherwise.
     *  \sa CopyLinkUtil::getLastHash()
     */
    bool internalCopyFile(const QString& copyFromPath, const QString& copyToPath, const bool doHash);

    /*! \brief Total number of bytes copied (without generating a hash at the same time) since the stats were reset by resetStats(). */
    qint64 m_bytesCopied;
    /*! \brief Total number of bytes linked since the stats were reset by resetStats(). */
    qint64 m_bytesLinked;
    /*! \brief Total number of bytes hashed (ignoring files hashed while copying at the same time) since the stats were reset by resetStats(). */
    qint64 m_bytesHashed;
    /*! \brief Total number of bytes copied and hashed at the same time since the stats were reset by resetStats(). */
    qint64 m_bytesCopiedHashed;

    /*! \brief Total number of milliseconds while files were copied (without generating a hash at the same time) since the stats were reset by resetStats(). */
    qint64 m_millisCopied;
    /*! \brief Total number of milliseconds while files were linked since the stats were reset by resetStats(). */
    qint64 m_millisLinked;
    /*! \brief Total number of milliseconds while files were hashed (ignoring files hashed while copying at the same time) since the stats were reset by resetStats(). */
    qint64 m_millisHashed;
    /*! \brief Total number of milliseconds while files were copied and hashed at the same time since the stats were reset by resetStats(). */
    qint64 m_millisCopiedHashed;

    //**************************************************************************
    /*! \brief Internal buffer used while reading files for copying and hashing.
     *  \sa CopyLinkUtil::setBufferSize()
     ***************************************************************************/
    char* m_buffer;

    //**************************************************************************
    /*! \brief Size of the internal buffer used while reading files for copying and hashing.
     *  \sa CopyLinkUtil::setBufferSize()
     ***************************************************************************/
    qint64 m_bufferSize;

<<<<<<< HEAD
    EnhancedQCryptographicHash* m_hashGenerator;
=======
    //**************************************************************************
    /*! \brief Hash generator to use to generate a hash.
     *  \sa CopyLinkUtil::setHashType()
     ***************************************************************************/
    QCryptographicHash* m_hashGenerator;

    /*! \brief Used to time operations such as copy, hash, and link. */
>>>>>>> 180eb6890230f7ba88357bacc54e94baeae7e68f
    QElapsedTimer * m_timer;

    /*! \brief If true, then a cancel has been requested and copying / hashing is aborted; this is why I use my own copy routines, there is no way to cancel a built-in copy. */
    bool m_cancelRequested;

    //**************************************************************************
    /*! \brief If set to true, hard links are used to reference "duplicate" files. This is expected to always be true.
     *  \sa CopyLinkUtil::setUseHardLink()
     *  \sa CopyLinkUtil::setUseSymbolicLink()
     ***************************************************************************/
    bool m_useHardLink;

    //**************************************************************************
    /*! \brief Hash generator type to use to generate a hash.
     *  \sa CopyLinkUtil::setHashType()
     ***************************************************************************/
    QCryptographicHash::Algorithm m_hashMethod;
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
