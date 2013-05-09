#include "messagecomponenttablemodel.h"


const int MessageComponentTableModel::numMessageComponentTableModelColumns = 2;
const int MessageComponentTableModel::fieldColumn = 0;
const int MessageComponentTableModel::stringColumn = 1;

MessageComponentTableModel::MessageComponentTableModel(QObject *parent) :
  QAbstractTableModel(parent)
{
}

void MessageComponentTableModel::clear()
{
  if (m_components.count() > 0)
  {
    beginRemoveRows(QModelIndex(), 0, m_components.count() - 1);
    m_components.clear();
    endRemoveRows();
  }
}

void MessageComponentTableModel::appendMessageComponents(const QList< QPair<SimpleLoggerRoutingInfo::MessageComponent, QString> >& components)
{
    clear();
    if (components.count() > 0)
    {
        beginInsertRows(QModelIndex(), m_components.count(), m_components.count() + components.count() - 1);
        m_components.append(components);
        endInsertRows();
    }
}

void MessageComponentTableModel::setMessageComponents(const QList< QPair<SimpleLoggerRoutingInfo::MessageComponent, QString> >& components)
{
    clear();
    if (components.count() > 0)
    {
        beginInsertRows(QModelIndex(), 0, components.count() - 1);
        m_components.append(components);
        endInsertRows();
    }
}

const QList< QPair<SimpleLoggerRoutingInfo::MessageComponent, QString> >& MessageComponentTableModel::getMessageComponents() const
{
    return m_components;
}


int MessageComponentTableModel::rowCount( const QModelIndex & parent) const
{
    return parent.isValid() ? 0 : m_components.count();
}

int MessageComponentTableModel::columnCount( const QModelIndex &) const
{
  return numMessageComponentTableModelColumns;
}

#include <QFile>
#include <QTextStream>

bool MessageComponentTableModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
  if (role == Qt::EditRole)
  {
    QPair<SimpleLoggerRoutingInfo::MessageComponent, QString>& component = m_components[index.row()];
    QString s;
    switch (index.column())
    {
    case fieldColumn:
      component.first = SimpleLoggerRoutingInfo::stringToComponent(value.toString());
      break;
    case stringColumn:
      // TODO: Validation?
      component.second = value.toString();
      break;
    default:
      return false;
      break;
    }
    emit dataChanged(index, index);
    return true;
  }
  return false;
}


QVariant MessageComponentTableModel::data( const QModelIndex &index, int role ) const
{
  const QPair<SimpleLoggerRoutingInfo::MessageComponent, QString>& component = m_components[index.row()];
  switch( role )
  {
  case Qt::DisplayRole:
    switch (index.column())
    {
    case fieldColumn:
      return SimpleLoggerRoutingInfo::componentToString(component.first);
      break;
    case stringColumn:
      return component.second;
      break;
    default:
      break;
    }
    return (index.row()+1) * (index.column()+1);

  case Qt::EditRole:
    switch (index.column())
    {
    case fieldColumn:
      {
        QStringList qsl;
        qsl << SimpleLoggerRoutingInfo::componentToString(component.first);
        qsl << SimpleLoggerRoutingInfo::getMessageRoutingStrings();
        return QVariant(qsl);
      }
      break;
    case stringColumn:
      {
        return component.second;
      }
      break;
    default:
      break;
    }

    return (index.row()+1) * (index.column()+1);
  case Qt::ToolTipRole:

    switch (index.column())
    {
    case fieldColumn:
      return tr("Message Component that is printed.");
      break;
    case stringColumn:
      return tr("Determines how the field is formatted.");
    default:
      return tr("Random unknown tip!");
      break;
    }
    break;
  default:
    return QVariant();
  }
  return QVariant();
}

QVariant MessageComponentTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( role != Qt::DisplayRole )
  {
    return QVariant();
  }
  const char* headers[] = {"Component", "Format"};
  if (orientation == Qt::Vertical)
  {
    return section + 1;
  }

  return (section < (int)(sizeof(headers)/sizeof(*headers))) ? QString(tr(headers[section])) : QVariant();
}

Qt::ItemFlags MessageComponentTableModel::flags( const QModelIndex &index ) const
{
  if(!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void MessageComponentTableModel::copyMessageComponent(int row)
{
  if (row < 0)
  {
    return;
  }
  insertMessageComponent(row, m_components.at(row));
}

void MessageComponentTableModel::insertMessageComponent(int row, const QPair<SimpleLoggerRoutingInfo::MessageComponent, QString>& component)
{
  if (row < 0)
  {
    row = 0;
  } else if (row > m_components.count())
  {
    row = m_components.count();
  }

  beginInsertRows(QModelIndex(), row, row);
  m_components.insert(row, component);
  endInsertRows();
}

void MessageComponentTableModel::removeMessageComponent(int row)
{
  if (0 <= row && row < m_components.count())
  {
    beginRemoveRows(QModelIndex(), row, row);
    m_components.removeAt(row);
    endRemoveRows();
  }
}

void MessageComponentTableModel::moveMessageComponentUp(int row)
{
  if (0 < row && row < m_components.count())
  {
    if (beginMoveRows(QModelIndex(), row, row, QModelIndex(), row- 1))
    {
      m_components.move(row, row - 1);
      endMoveRows();
    }
  }
}

void MessageComponentTableModel::moveMessageComponentDown(int row)
{
  if (0 <= row && row < m_components.count() - 1)
  {
    // Yes, I really do need to use row+2, odd as it is!
    if (beginMoveRows(QModelIndex(), row, row, QModelIndex(), row + 2))
    {
      m_components.move(row, row + 1);
      endMoveRows();
    }
  }
}

int MessageComponentTableModel::messageComponentCount() const
{
  return m_components.count();
}


