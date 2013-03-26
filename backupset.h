#ifndef BACKUPSET_H
#define BACKUPSET_H

#include "linkbackfilter.h"
#include "criteriaforfilematch.h"

#include <QList>
#include <QXmlStreamWriter>
#include <QThread>

//**************************************************************************
/*! \class BackupSet
 * \brief Hold the settings for a single backup set.
 *
 * Primarily, this includes the criteria for two files to be considered the same,
 * The criteria for ignoring or backing up files and directories, directory from which
 * to copy the files, the directory to which the files will be written, and the hash method.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 *
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
    BackupSet& operator=(const BackupSet& backupSet);

    /*! \brief Empty all lists and set all strings to be empty. */
    void clear();

    /*! \brief Returns the "FromPath", which is the source of the backup.
     *
     *  \return The path which is the source of the backup.
     */
    const QString& getFromPath() const;

    /*! \brief Set the path that is backed-up.
     *
     *  \param [in] fromPath Path that is backed-up.
     */
    void setFromPath(const QString& fromPath);

    /*! \brief Get the path to which the back-up is stored.
     *
     *  \return Path to which the back-up is stored.
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

    /*! \brief Get the thread priority at which the backup runs.
     *
     *  \return Thread priority at which the backup runs.
     */
    const QString& getPriority() const;

    /*! \brief Set the thread priority at which the backup runs.
     *
     *  Default value is usually InheritPriority.
     *
     *  \param [in] priority is the thread priority at which the backup runs.
     */
    void setPriority(const QString& priority);

    /*! \brief Write the data to the stream in a manner suitable for saving.
     *
     *  \param [in,out] writer XML stream writer to which the data is written.
     *  \return Reference to the XML stream writer.
     */
    QXmlStreamWriter& operator<<(QXmlStreamWriter& writer) const;

    /*! \brief Read data from the stream.
     *
     *  \param [in,out] reader XML stream reader from which the data is read.
     *  \return Reference to the XML stream writer.
     */
    QXmlStreamReader& operator>>(QXmlStreamReader& reader);

    /*! \brief Read XML configuration file to configure this object.
     *
     *  \param [in] Full path to the XML file to read.
     *  \return True if no errors are encountered.
     */
    bool readFile(const QString& fullPath);

    /*! \brief Write configuration data to the file.
     *
     *  \param [in] Full path to the XML file to write.
     *  \return True if successful.
     */
    bool writeFile(const QString& fullPath);

    /*! \brief Set all values to default. Currently, this is the same as calling clear(). */
    void setAllDefault();

    /*! \brief Get a reference to the collection of filters used to determine if a file or directory is used in the backup.
     *
     *  \return Reference to the filters.
     */
    const QList<LinkBackFilter>& getFilters() const;

    /*! \brief Set the collection of filters used to determine if a file or directory is used in the backup.
     *  \param [in] filters Collection of filters used to determine if a file or directory is used in the backup.
     */
    void setFilters(const QList<LinkBackFilter>& filters);

    /*! \brief Get a reference to the criteria used to determine if two files are the same on disk.
     *
     *  \return Reference to the criteria used to determine if two files are the same on disk.
     */
    const QList<CriteriaForFileMatch>& getCriteria() const;

    /*! \brief Set the criteria used to determine if two files are the same on disk.
     *
     *  \param [in] criteria used to determine if two files are the same on disk.
     */
    void setCriteria(const QList<CriteriaForFileMatch>& criteria);

    /*! \brief Determine if a file or directory will be processed.
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

    static QString priorityToString(QThread::Priority priority);

    static QThread::Priority stringToPriority(const QString& priority, QThread::Priority defaultPriority = QThread::InheritPriority);

    static QStringList getAllPriorities();

private:
    /*! \brief Read the BackupSet XML into this object.
     *
     *  \param [in,out] reader XML stream from which the data is read.
     *  \return Reference to the reader so that this can be used as part of streaming operations.
     */
    QXmlStreamReader& readFilter(QXmlStreamReader& reader);

    /*! \brief Assumes the outer "BackupSet" element has been read. Read the rest of the XML into this object.
     *
     *  \param [in,out] reader XML stream from which the data is read.
     */
    void readInternals(QXmlStreamReader& reader);

    /*! \brief Assumes the outer "Filters" element has been read. Read each "Filter" and add it. Stops when the closing "Filters" element is reached.
     *
     *  \param [in,out] reader XML stream from which the data is read.
     */
    void readFilters(QXmlStreamReader& reader);

    /*! \brief Assumes the outer "MatchCriteria" element has been read. Read each "Criteria" and add it. Stops when the closing "MatchCriteria" element is reached.
     *
     *  \param [in,out] reader XML stream from which the data is read.
     */
    void readCriteria(QXmlStreamReader& reader);


private:
    /*! \brief Path that is backed-up */
    QString m_fromPath;

    /*! \brief Path where the backup is written */
    QString m_toPath;

    /*! \brief Hash method to use. */
    QString m_hashMethod;

    /*! \brief Priority at which the backup thread runs. */
    QString m_backupPriority;

    /*! \brief Filters used to determine what is backed-up and what is not. */
    QList<LinkBackFilter> m_filters;

    /*! \brief Criteria used to determine if two files are the same. */
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

inline const QString& BackupSet::getPriority() const
{
    return m_backupPriority;
}

inline void BackupSet::setPriority(const QString& priority)
{
    m_backupPriority = priority;
}

inline void BackupSet::setAllDefault() {
    clear();
}

#endif // BACKUPSET_H
