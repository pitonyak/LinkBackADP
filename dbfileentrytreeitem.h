#ifndef DBFILEENTRYTREEITEM_H
#define DBFILEENTRYTREEITEM_H

#include "dbfileentry.h"
#include <QList>
#include <QVariant>

struct DBFileEntryTreeItemPtrLess;

//**************************************************************************
//! Encapsulate a DBFileEntry for display in a QTreeView
/*!
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2012-2013
 **************************************************************************/
class DBFileEntryTreeItem : public DBFileEntry
{
public:

  enum Columns {Name, Size, TimeStamp, LinkType, Hash};

  //**************************************************************************
  //! Default Constructor.
  /*!
   *
   ***************************************************************************/
  DBFileEntryTreeItem();

  //**************************************************************************
  //! Destructor
  /*!
   *
   ***************************************************************************/
  virtual ~DBFileEntryTreeItem();

  //**************************************************************************
  //! Copy Constructor.
  /*!
   * \param [in] entry Object to assign to this one.
   *
   ***************************************************************************/
  DBFileEntryTreeItem(const DBFileEntryTreeItem& entry);

  //**************************************************************************
  //! Copy Constructor.
  /*!
   * \param [in] entry Object to assign to this one. Always assumed to be a file.
   *
   ***************************************************************************/
  DBFileEntryTreeItem(const DBFileEntry& entry);

  const QString& getFileName() const { return m_fileName; }

  //**************************************************************************
  //! Deep copy assignment.
  /*!
   * A deep copy is made of all children, so, this can be very expensive.
   *
   * \param [in] entry Object to assign to this one.
   * \returns Reference to this object.
   *
   ***************************************************************************/
  const DBFileEntryTreeItem& operator=(const DBFileEntryTreeItem& entry);

  //**************************************************************************
  //! Add a child to the list of children.
  /*!
   * This object takes ownership of the child and later deletes it. nullptr is not added.
   * Parent is automatically set on this child when it is added.
   *
   * \param [in] child Added to the end of the list.
   *
   ***************************************************************************/
  void appendChild(DBFileEntryTreeItem *child);

  //**************************************************************************
  //! Get a child object.
  /*!
   * \param [in] row Index of the child to return.
   *
   ***************************************************************************/
  DBFileEntryTreeItem *child(const int row);

  /*! \brief Return the number of children. */
  int childCount() const {return m_children.count();}

  int columnCount() const {return 3;}

  //**************************************************************************
  //! Get the data based on the author's arbitrary decision for order.
  /*!
   * column = 0 is Name
   * column = 1 is Size
   * column = 2 is Time
   * column = 3 is Link Type
   * column = 4 is Hash
   * \param [in] column indicates the column of interst.
   * \returns The index of this object in the parent's child list or 0 if there is no parent.
   *
   ***************************************************************************/
  QVariant data(const int column) const;

  //**************************************************************************
  //! What row is this object in the parent object's child list?
  /*!
   * \returns The index of this object in the parent's child list or 0 if there is no parent.
   *
   ***************************************************************************/
  int row() const;

  /*! \brief Get the parent object. */
  DBFileEntryTreeItem *parent() {return m_parent;}

  void sort(Columns column=Name);
  void sort(DBFileEntryTreeItemPtrLess& lessThan);

private:
  /*! Parent item, which is NOT owned by this item */
  DBFileEntryTreeItem* m_parent;

  /*! \brief Quick way to determine if this is a file or a directory. */
  bool m_isFile;

  /*! \brief File name portion on which this field is sorted (so it has a value for a directory as well). */
  QString m_fileName;

  /*! \brief List of child items that are owned by this object.. */
  QList<DBFileEntryTreeItem*> m_children;

};

struct DBFileEntryTreeItemPtrLess
{
  DBFileEntryTreeItemPtrLess() : m_column(DBFileEntryTreeItem::Name) {}
  DBFileEntryTreeItemPtrLess(DBFileEntryTreeItem::Columns column) : m_column(column) {}

  DBFileEntryTreeItem::Columns m_column;

  bool operator()(const DBFileEntryTreeItem* a, const DBFileEntryTreeItem* b) const;
};


#endif // DBFILEENTRYTREEITEM_H
