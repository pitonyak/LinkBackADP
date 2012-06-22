#ifndef DBFILEENTRIES_H
#define DBFILEENTRIES_H

#include "dbfileentry.h"
#include <QList>
#include <QMultiHash>

class CriteriaForFileMatch;

class DBFileEntries
{
public:
    DBFileEntries();
    virtual ~DBFileEntries();

    void addEntry(DBFileEntry *entry);
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

    /*! \brief Determine if an external entry matches an internal entry based on the provided criteria.
     *
     *  \param [in] criteria Specifies how to match a file entry.
     *  \param [in] myEntry Internal entry that may match the external entry.
     *  \param [in, out] entryToMatch External entry, we want to find an entry that matches this one.The Hash will be calculated if it is needed.
     *  \param [in] matchInitialPath When prepended to entry, this yields the full path to the file on disk.
     *  \return Pointer to the file entry, or null if no match is found.
     */
    bool entriesMatch(const CriteriaForFileMatch& criteria, const DBFileEntry* myEntry, DBFileEntry* entryToMatch, const QString& matchInitialPath) const;

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

    /*! \brief Number of entries in the list.
     *
     */
    int count() const;

private:
    QList<DBFileEntry *> m_entries;
    QHash<QString, QMultiHash<quint64, int>* > m_hashToSizeToEntry;
    QHash<QString, int> m_pathToEntry;
};

inline int DBFileEntries::count() const
{
  return m_entries.count();
}


#endif // DBFILEENTRIES_H
