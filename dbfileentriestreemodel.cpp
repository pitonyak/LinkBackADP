#include "dbfileentriestreemodel.h"
#include "dbfileentries.h"
#include "dbfileentry.h"
#include "dbfileentrytreeitem.h"

#include <QStringList>

DBFileEntriesTreeModel::DBFileEntriesTreeModel(QObject *parent) :
    QAbstractItemModel(parent), m_rootItem(nullptr)
{
    m_rootItem = new DBFileEntryTreeItem();
}

DBFileEntriesTreeModel::~DBFileEntriesTreeModel()
{
    delete m_rootItem;
}

QVariant DBFileEntriesTreeModel::data(const QModelIndex &index, const int role) const
{
    if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();

     DBFileEntryTreeItem *item = static_cast<DBFileEntryTreeItem*>(index.internalPointer());

     return item->data(index.column());
}

Qt::ItemFlags DBFileEntriesTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DBFileEntriesTreeModel::headerData(const int section, Qt::Orientation orientation, const int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            return "Name";
          break;
        case 1:
            return "Size";
          break;
        case 2:
            return "Time";
          break;
        case 3:
            return "L/C";
          break;
        case 4:
            return "Hash";
          break;
        default:
          break;
        }
    }

     return QVariant();
}

QModelIndex DBFileEntriesTreeModel::index(const int row, const int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
         return QModelIndex();

     DBFileEntryTreeItem *parentItem;

     if (!parent.isValid())
         parentItem = m_rootItem;
     else
         parentItem = static_cast<DBFileEntryTreeItem*>(parent.internalPointer());

     DBFileEntryTreeItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
}

QModelIndex DBFileEntriesTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DBFileEntryTreeItem *childItem = static_cast<DBFileEntryTreeItem*>(index.internalPointer());
    DBFileEntryTreeItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DBFileEntriesTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    DBFileEntryTreeItem *parentItem = parent.isValid() ? static_cast<DBFileEntryTreeItem*>(parent.internalPointer()) : m_rootItem;

    return parentItem->childCount();
}

int DBFileEntriesTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
         return static_cast<DBFileEntryTreeItem*>(parent.internalPointer())->columnCount();
     else
         return m_rootItem->columnCount();
}

void DBFileEntriesTreeModel::setupModelData(const QStringList &lines, DBFileEntryTreeItem *parent)
{
    if (m_rootItem != nullptr)
    {
        delete m_rootItem;
    }
    m_rootItem = parent;
}

void DBFileEntriesTreeModel::setupModelData(const QStringList &lines, const DBFileEntries *entries)
{
    if (entries == nullptr || entries->count() < 1)
    {
        QStringList lst;
        setupModelData(lst, new DBFileEntryTreeItem());
        return;
    }
    QString shortestPath = "";
    for (int i=0; i<entries->count(); ++i)
    {
        const DBFileEntry* entry = entries->value(i);
        // This should never be null, but, I hate taking chances.
        if (entry != nullptr)
        {
            QString path = entry->getPath();
            // While path is not in my map of paths to DBFileEntryTreeItem things,
            //   build the tree.
            // Add this entry to the tree

        }
    }
    // Sort the list by name.
}

