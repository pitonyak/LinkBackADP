#ifndef DBFILEENTRIES_H
#define DBFILEENTRIES_H

#include "dbfileentry.h"
#include <QList>
#include <QMultiHash>

class CriteriaForFileMatch;

//**************************************************************************
//! Collection of file entries. This may represent a previous backup set or a new backup set as it is created.
/*!
 * This object is able to test to see if it already contains a matching object based on matching criteria,
 * so that sometimes a file is copied, and sometimes a  file is linked.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

class DBFileEntries
{
public:
    /*! Constructor */
    DBFileEntries();

    /*! Desctructor, clears all structures and deletes each file entry. */
    virtual ~DBFileEntries();

    /*! Add a new file entry object. This object takes ownership of the file entry. All related datastructures are updated. */
    void addEntry(DBFileEntry *entry);

    /*! Clear all entries, deleting each file entry. */
    void clear();

    /*! \brief Find an entry that matches as specified by the criteria.
     *
     *  A match is not done exactly as might be expected, a few simple "cheat" rules are used.
     *  If the criteria allows for the full path, then the entry for the full path is tried first.
     *  The entry is immediately rejected if the size is wrong.
     *  \param [in] criteria Specifies how to match a file entry.
     *  \param [in, out] entry File entry to match.
     *  \param [in] matchInitialPath When prepended to entry, this yields the full path to the file on disk.
     *  \return Pointer to the file entry, or null if no match is found.
     */
    const DBFileEntry* findEntry(const CriteriaForFileMatch& criteria, DBFileEntry* entry, const QString& matchInitialPath) const;

    /*! \brief Find an entry that matches at least one of the criteria.
     *
     *  Criteria are searched in list order. The first to match is returned.
     *  \param [in] criteria Specifies how to match a file entry.
     *  \param [in, out] entry File entry to match.
     *  \param [in] matchInitialPath When prepended to entryToMatch, this yields the full path to the file on disk.
     *  \return Pointer to the file entry, or null if no match is found.
     */
    const DBFileEntry* findEntry(const QList<CriteriaForFileMatch>& criteria, DBFileEntry* entry, const QString& matchInitialPath) const;

    /*! \brief Get an entry by index; useful to get all entries.
     *
     *  \param [in] index.
     *  \return Pointer to the file entry, or null if the index is out of range.
     */
    const DBFileEntry* value(const int index) const;

    /*! \brief Determine if an external entry matches an internal entry based on the provided criteria.
     *
     *  \param [in] criteria Specifies how to match a file entry.
     *  \param [in] myEntry Internal entry that may match the external entry.
     *  \param [in, out] entryToMatch External entry, we want to find an entry that matches this one.The Hash will be calculated if it is needed.
     *  \param [in] matchInitialPath When prepended to entry, this yields the full path to the file on disk.
     *  \return Pointer to the file entry, or null if no match is found.
     */
    static bool entriesMatch(const CriteriaForFileMatch& criteria, const DBFileEntry* myEntry, DBFileEntry* entryToMatch, const QString& matchInitialPath);

    /*! \brief Read entry file from the path specified. The file name is not part of the path.
     *
     *  \param [in] path Full path to the directory containing the db entry file.
     *  \return New class containing the read data, and null if not cannot read.
     */
    static DBFileEntries* read(const QString& path);

    /*! \brief Read entry file from the text stream reader.
     *
     *  \param [in,out] reader Text stream already opened to the file of interest.
     *  \return New class containing the read data, and null if not cannot read.
     */
    static DBFileEntries* read(QTextStream& reader);

    /*! \brief Write entry file to the path specified. The file name is not part of the path.
     *
     *  \param [in] path Full path to the directory containing the db entry file.
     *  \return True if the file is successfully read.
     */
    bool write(const QString& path) const;

    /*! \brief Write this object to a text stream.
     *
     *  \param [in,out] writer Text stream already opened to the file of interest.
     *  \return True if the file is successfully read.
     */
    bool write(QTextStream& writer) const;

    /*! \brief Number of entries in the list. */
    int count() const;

private:
    /*! List of files with file size, time, full path, hash value, and the link type on disk in the backup location. Contained entries are owned by this object. */
    QList<DBFileEntry *> m_entries;

    /*! Use a files hash and size to find the file's index in the m_entries variable. This is a multi-hash, so there can be multiple files with the same hash and size. */
    QHash<QString, QMultiHash<quint64, int>* > m_hashToSizeToEntry;

    /*! Use the full path to find the file's index in the m_entries variable. */
    QHash<QString, int> m_pathToEntry;
};

inline int DBFileEntries::count() const
{
  return m_entries.count();
}

inline const DBFileEntry* DBFileEntries::value(const int index) const
{
    return m_entries.value(index, nullptr);
}


#endif // DBFILEENTRIES_H
