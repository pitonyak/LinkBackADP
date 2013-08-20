#ifndef DBFILEENTRY_H
#define DBFILEENTRY_H

#include <QChar>
#include <QDateTime>

class QTextStream;
class QFileInfo;

//**************************************************************************
//! Encapsulate the data about a single file used to determine if it matches another. This includes file's size, time, full path, hash value, and the link type on disk in the backup location.
/*!
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 **************************************************************************/
class DBFileEntry
{
public:
    //**************************************************************************
    //! Main Constructor.
    /*!
     *
     ***************************************************************************/
    DBFileEntry();

    //**************************************************************************
    //! Destructor.
    /*!
     * Only exists so that I can make this virtual.
     ***************************************************************************/
    virtual ~DBFileEntry();

    //**************************************************************************
    //! Copy Constructor.
    /*!
     *
     ***************************************************************************/
    DBFileEntry(const DBFileEntry& entry);

    //**************************************************************************
    //! Constructor that sets data available from the QFileInfo object, but does not set the hash.
    /*!
     * \param [in] info File information object. Used to set file size and last modified time.
     * \param [in] rootPath The path to the file is assumed to start after this path.
     *
     ***************************************************************************/
    DBFileEntry(const QFileInfo& info, const QString& rootPath);

    //**************************************************************************
    //! Get the file size in bytes.
    /*!
     * \returns File size in bytes.
     *
     ***************************************************************************/
    quint64 getSize() const;

    //**************************************************************************
    //! Set the file size in bytes.
    /*!
     * \param [in] fileSize File size in bytes.
     *
     ***************************************************************************/
    void setSize(const quint64 fileSize);

    //**************************************************************************
    //! Get character representing how the file is copied. C means copy and L means Link.
    /*!
     * \returns Character representing how the file is copied. C means copy and L means Link.
     *
     ***************************************************************************/
    QChar getLinkType() const;

    //**************************************************************************
    //! Set character representing how the file is copied. C means copy and L means Link.
    /*!
     * \param [in] C character representing how the file is copied. C means copy and L means Link.
     *
     ***************************************************************************/
    void setLinkType(const QChar c);

    //**************************************************************************
    //! Shortcut for setting the link type to C.
    /*!
     *
     ***************************************************************************/
    void setLinkTypeCopy();

    //**************************************************************************
    //! Shortcut for setting the link type to L.
    /*!
     *
     ***************************************************************************/
    void setLinkTypeLink();

    //**************************************************************************
    //! Get the file last modified date and time.
    /*!
     * \returns Date and time the file was last modified.
     *
     ***************************************************************************/
    const QDateTime& getTime() const;

    //**************************************************************************
    //! Set the last modified date and time.
    /*!
     * \param [in] dateTime Date and time the file was last modified.
     *
     ***************************************************************************/
    void setTime(const QDateTime& dateTime);

    //**************************************************************************
    //! Get the relative path to the file from the backup start point. Includes the file name.
    /*!
     * \returns Relative path to the file from the backup start point. Includes the file name.
     *
     ***************************************************************************/
    const QString& getPath() const;

    //**************************************************************************
    //! Set the relative path to the file from the backup start point. Includes the file name.
    /*!
     * \param path Relative path to the file from the backup start point. Includes the file name.
     *
     ***************************************************************************/
    void setPath(const QString& path);

    //**************************************************************************
    //! Returns the hash value. If you want a value for it, you must set it.
    /*!
     * \returns Already computed hash value.
     *
     ***************************************************************************/
    const QString& getHash() const;

    //**************************************************************************
    //! Set the hash value.
    /*!
     * \param [in] hash Hash value for this entry.
     *
     ***************************************************************************/
    void setHash(const QString& hash);

    //**************************************************************************
    //! Populate the values in this class from the stream.
    /*!
     * This is tolerant to a field separator in the path because the path is written
     * last. A field separator in other fields (such as the time stamp) is a problem.
     * \param [in,out] stream The entry is filled by reading this stream.
     * \returns True if successful, false otherwise.
     *
     ***************************************************************************/
    bool readLine(QTextStream& stream);

    //**************************************************************************
    //! Persist this object to the stream.
    /*!
     * Path is written last so a "field separator" in the path will not break things.
     * \param [in,out] stream Stream to which this object is written.
     * \returns True if successful, false otherwise.
     *
     ***************************************************************************/
    bool writeLine(QTextStream& stream);

    const DBFileEntry& operator=(const DBFileEntry& entry);


private:
    /*! \brief File size. */
    quint64 m_size;

    /*! \brief Identifies backup type; C if a copy was made, L if a link was made. */
    QChar m_linkType;

    /*! \brief File date/time */
    QDateTime m_time;

    /*! \brief Path to file relative to backup root directory. Includes the file name. */
    QString m_path;

    /*! \brief Hash value. */
    QString m_hash;

    /*! \brief Format for the date/time. */
    static QString dateTimeFormat;

    /*! \brief Character used to separate files in the produced file. This character had better not be written to the file in any field other than the path and filename. Current value is a comma. */
    static QChar fieldSeparator;
};

inline quint64 DBFileEntry::getSize() const
{
  return m_size;
}
inline void DBFileEntry::setSize(const quint64 fileSize)
{
  m_size = fileSize;
}

inline QChar DBFileEntry::getLinkType() const
{
  return m_linkType;
}
inline void DBFileEntry::setLinkType(const QChar c)
{
  m_linkType = c;
}

inline const QDateTime& DBFileEntry::getTime() const
{
  return m_time;
}
inline void DBFileEntry::setTime(const QDateTime& dateTime)
{
  m_time = dateTime;
}

inline const QString& DBFileEntry::getPath() const
{
  return m_path;
}
inline void DBFileEntry::setPath(const QString& path)
{
  m_path = path;
}

inline const QString& DBFileEntry::getHash() const
{
  return m_hash;
}
inline void DBFileEntry::setHash(const QString& hash)
{
  m_hash = hash;
}
inline void DBFileEntry::setLinkTypeCopy()
{
  setLinkType('C');
}

inline void DBFileEntry::setLinkTypeLink()
{
  setLinkType('L');
}

#endif // DBFILEENTRY_H
