#ifndef DBFILEENTRIESTREEMODEL_H
#define DBFILEENTRIESTREEMODEL_H

#include <QAbstractItemModel>

class DBFileEntryTreeItem;
class DBFileEntries;

class DBFileEntriesTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DBFileEntriesTreeModel(QObject *parent = 0);
    ~DBFileEntriesTreeModel();
    
    QVariant data(const QModelIndex &index, const int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(const int section, Qt::Orientation orientation, const int role = Qt::DisplayRole) const;
    QModelIndex index(const int row, const int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void setupModelData(const QStringList &lines, DBFileEntryTreeItem *parent);
    void setupModelData(const QStringList &lines, const DBFileEntries *entries);

signals:
    
public slots:

private:
   DBFileEntryTreeItem *m_rootItem;
    
};

#endif // DBFILEENTRIESTREEMODEL_H
