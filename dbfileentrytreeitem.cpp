#include "dbfileentrytreeitem.h"

DBFileEntryTreeItem::DBFileEntryTreeItem() : DBFileEntry(), m_parent(nullptr), m_isFile(true)
{
}

DBFileEntryTreeItem::DBFileEntryTreeItem(const DBFileEntryTreeItem& entry) : DBFileEntryTreeItem()
{
  operator=(entry);
}

DBFileEntryTreeItem::DBFileEntryTreeItem(const DBFileEntry& entry) : DBFileEntry(entry), m_parent(nullptr), m_isFile(true)
{
  // This is for a file, so extract the file name from the end.
  int pos = getPath().lastIndexOf('/');
  if (pos < 0)
  {
    m_fileName = getPath();
  }
  else if (pos == getPath().length() - 1)
  {
    // Last character so just leave the name empty!
    m_isFile = false;
  }
  else
  {
    // It exists and is NOT the last character.
    m_fileName = getPath().left(getPath().length() - pos - 1);
  }
}

DBFileEntryTreeItem& DBFileEntryTreeItem::operator=(const DBFileEntryTreeItem& entry)
{
  if (&entry != this)
  {
    DBFileEntry::operator=(entry);

    // This may be wrong, so be careful.
    m_parent = entry.m_parent;

    m_isFile = entry.m_isFile;
    m_fileName = entry.m_fileName;

    qDeleteAll(m_children);
    m_children.clear();

    if (!entry.m_children.isEmpty())
    {
      setSize(0);
      QListIterator<DBFileEntryTreeItem*> i(entry.m_children);
      while (i.hasNext())
      {
        // It is assumed that NO children are nullptr.
        appendChild(new DBFileEntryTreeItem(*i.next()));
      }
    }
  }
  return *this;
}

DBFileEntryTreeItem::~DBFileEntryTreeItem()
{
  qDeleteAll(m_children);
}

void DBFileEntryTreeItem::appendChild(DBFileEntryTreeItem *child)
{
  if (child != nullptr)
  {
    m_children.append(child);
    child->m_parent = this;
    if (getTime() < child->getTime())
    {
      setTime(child->getTime());
    }
    setSize(getSize() + child->getSize());
  }
}

DBFileEntryTreeItem *DBFileEntryTreeItem::child(const int row)
{
  return m_children.value(row);
}

QVariant DBFileEntryTreeItem::data(const int column) const
{
  switch (column)
  {
  case 0:
    return getFileName();
    break;
  case 1:
    // TODO: Convert to pretty human readable text.
    return getSize();
    break;
  case 2:
    return getTime();
    break;
  case 3:
    return getLinkType();
    break;
  case 4:
    return getHash();
    break;
  default:
    break;
  }
  return QVariant();
}

int DBFileEntryTreeItem::row() const
{
  return (m_parent != nullptr) ? m_parent->m_children.indexOf(const_cast<DBFileEntryTreeItem*>(this)) : 0;
}

void DBFileEntryTreeItem::sort(Columns column)
{
  DBFileEntryTreeItemPtrLess lessThan(column);
  sort(lessThan);
}

void DBFileEntryTreeItem::sort(DBFileEntryTreeItemPtrLess& lessThan)
{
  qSort(m_children.begin(), m_children.end(), lessThan);
  QListIterator<DBFileEntryTreeItem*> i(m_children);
  while (i.hasNext())
  {
    // It is assumed that NO children are nullptr.
    i.next()->sort(lessThan);
  }
}

bool DBFileEntryTreeItemPtrLess::operator ()(const DBFileEntryTreeItem* a, const DBFileEntryTreeItem* b) const
{
  if (a == nullptr)
  {
    return (b != nullptr) ? true : false;
  }
  else if (b != nullptr)
  {
    switch(m_column)
    {
    case DBFileEntryTreeItem::Name :
      return a->getFileName().compare(b->getFileName(), Qt::CaseInsensitive) < 0 ? true : false;
      break;
    case DBFileEntryTreeItem::Size :
      return a->getSize() < b->getSize();
      break;
    case DBFileEntryTreeItem::TimeStamp :
      return a->getTime() < b->getTime();
      break;
    case DBFileEntryTreeItem::LinkType :
      return a->getLinkType() < b->getLinkType();
      break;
    case DBFileEntryTreeItem::Hash :
      return a->getHash() < b->getHash();
      break;
    default:
      // TODO: This is an error
      break;
    }
  }
  return false;
}

