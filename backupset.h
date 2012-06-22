#ifndef BACKUPSET_H
#define BACKUPSET_H

#include "linkbackfilter.h"
#include "criteriaforfilematch.h"

#include <QList>
#include <QXmlStreamWriter>

//**************************************************************************
//! Hold the settings for a single backup set.
/*!
 *  This routine is Copyright (c) by Andrew Pitonyak, and may be used
 *  without restrictions or renumerations. I make no statement of correctness.
 ***************************************************************************/

class BackupSet
{
public:
    /*! \brief Default constructor. */
    BackupSet();

    /*! \brief Copy constructor.
     *
     *  \param [in] backupSet Class to copy.
     */
    BackupSet(const BackupSet& backupSet);

    /*! \brief Virtual destructor. */
    virtual ~BackupSet();

    /*! \brief Copy a class into this object.
     *
     *  \param [in] backupSet Object to copy into this object.
     *  \return Reference to this object.
     */
    const BackupSet& operator=(const BackupSet& backupSet);

    /*! \brief Empty all lists and set all strings to be empty. */
    void clear();

    /*! \brief Returns the flags used to describe the item identified by the index.
     *
     *  Flags determine whether the item can be checked, edited, and selected.
     *  \param [in] index Identifies the item of interest based on a row and column.
     *  \return Flags that apply to the item specified by the index.
     */
    const QString& getFromPath() const;

    /*! \brief Set the path that is backed-up.
     *
     *  \param [in] fromPath Path that is backed-up.
     */
    void setFromPath(const QString& fromPath);

    /*! \brief Get the path that is backed-up.
     *
     *  \return Path that is backed-up.
     */
    const QString& getToPath() const;

    /*! \brief Set the path where the copies are stored.
     *
     * This directory contains other directories named with the date and time the backup was created.
     * The date / time directory contains the backup summary file and another directory with the files.
     *  \param [in] toPath Path where the copies are stored.
     */
    void setToPath(const QString& toPath);

    /*! \brief Get the Hash method used to compare files.
     *
     *  \return Hash method used to compare files.
     */
    const QString& getHashMethod() const;

    /*! \brief Set the Hash method used to compare files.
     *
     *  \param [in] hashMethod Hash method used to compare files.
     */
    void setHashMethod(const QString& hashMethod);

    /*! \brief Write the data to the stream in a manner suitable for saving.
     *
     *  \param [in] writer XML stream writer to which the data is written.
     *  \return Reference to the XML stream writer.
     */
    QXmlStreamWriter& operator<<(QXmlStreamWriter& writer) const;

    /*! \brief Read data from the stream.
     *
     *  \param [in] reader XML stream reader from which the data is read.
     *  \return Reference to the XML stream writer.
     */
    QXmlStreamReader& operator>>(QXmlStreamReader& reader);

    /*! \brief Read data from the stream.
     *
     *  \param [in] reader XML stream reader from which the data is read.
     *  \return True if no errors are encountered.
     */
    bool readFile(const QString& fullPath);

    /*! \brief Write the data to the stream in a manner suitable for saving.
     *
     *  \param [in] writer XML stream writer to which the data is written.
     *  \return True if successful.
     */
    bool writeFile(const QString& fullPath);

    /*! \brief Set all values to default. Currently, this is the same as calling clear(). */
    void setAllDefault();

    /*! \brief Get a reference to the collection of filters used to determine if a file or directory is used in the backup.
     *
     *  \return Refernece to the filters.
     */
    const QList<LinkBackFilter>& getFilters() const;

    /*! \brief Set the collection of filters used to determine if a file or directory is used in the backup.
     *  \param [in] filters Collection of filters used to determine if a file or directory is used in the backup.
     */
    void setFilters(const QList<LinkBackFilter>& filters);

    const QList<CriteriaForFileMatch>& getCriteria() const;
    void setCriteria(const QList<CriteriaForFileMatch>& criteria);

    /*! \brief Determine if a file or directory is processed.
     *
     *  This is where the real magic happens. The file or directory is
     *  compared against every filter. The first filter that matches determines
     *  if the file is accepted or rejected.
     *
     *  If a file or directory does not match a filter, the default behavior
     *  is to reject.
     *
     *  \param [in] info QFileInfo object referencing the file to be checked.
     *  \return True if the file or directory passes the filters, false otherwise.
     */
    bool passes(const QFileInfo& info) const;

private:
    QXmlStreamReader& readFilter(QXmlStreamReader& reader);
    void readInternals(QXmlStreamReader& reader);
    void readFilters(QXmlStreamReader& reader);
    void readCriteria(QXmlStreamReader& reader);


private:
    QString m_fromPath;
    QString m_toPath;
    QString m_hashMethod;
    QList<LinkBackFilter> m_filters;
    QList<CriteriaForFileMatch> m_criteria;
};


inline QXmlStreamWriter& operator<<(QXmlStreamWriter& reader, const BackupSet& backupSet)
{
    return backupSet.operator<<(reader);
}

inline QXmlStreamReader& operator>>(QXmlStreamReader& writer, BackupSet& backupSet)
{
    return backupSet.operator>>(writer);
}

inline const QString& BackupSet::getFromPath() const
{
  return m_fromPath;
}

inline void BackupSet::setFromPath(const QString& fromPath)
{
  m_fromPath = fromPath;
}

inline const QString& BackupSet::getToPath() const
{
  return m_toPath;
}

inline void BackupSet::setToPath(const QString& toPath)
{
  m_toPath = toPath;
}

inline const QString& BackupSet::getHashMethod() const
{
  return m_hashMethod;
}

inline void BackupSet::setHashMethod(const QString& hashMethod)
{
  m_hashMethod = hashMethod;
}

inline void BackupSet::setAllDefault() {
    clear();
}

#endif // BACKUPSET_H
