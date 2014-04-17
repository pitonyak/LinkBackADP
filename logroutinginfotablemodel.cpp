#include "logroutinginfotablemodel.h"


const int LogRoutingInfoTableModel::numColumns = 9;
const int LogRoutingInfoTableModel::enabledColumn = 0;
const int LogRoutingInfoTableModel::nameColumn = 1;
const int LogRoutingInfoTableModel::locationRegExpColumn = 2;
const int LogRoutingInfoTableModel::messageRegExpColumn = 3;
const int LogRoutingInfoTableModel::routFileColumn = 4;
const int LogRoutingInfoTableModel::routScreenColumn = 5;
const int LogRoutingInfoTableModel::routDebugColumn = 6;
const int LogRoutingInfoTableModel::levelsColumn = 7;
const int LogRoutingInfoTableModel::componentColumn = 8;


LogRoutingInfoTableModel::LogRoutingInfoTableModel(QObject *parent) :
  QAbstractTableModel(parent)
{
}


void LogRoutingInfoTableModel::clear()
{
  if (m_routings.count() > 0)
  {
    beginRemoveRows(QModelIndex(), 0, m_routings.count() - 1);
    m_routings.clear();
    endRemoveRows();
  }
}

void LogRoutingInfoTableModel::appendRoutings(const QList<SimpleLoggerRoutingInfo> &routing)
{
    clear();
    if (routing.count() > 0)
    {
        beginInsertRows(QModelIndex(), m_routings.count(), m_routings.count() + routing.count() - 1);
        m_routings.append(routing);
        endInsertRows();
    }
}

void LogRoutingInfoTableModel::setRoutings(const QList<SimpleLoggerRoutingInfo>& routing)
{
    clear();
    if (routing.count() > 0)
    {
        beginInsertRows(QModelIndex(), 0, routing.count() - 1);
        m_routings.append(routing);
        endInsertRows();
    }
}

const QList<SimpleLoggerRoutingInfo> & LogRoutingInfoTableModel::getRoutings() const
{
    return m_routings;
}

int LogRoutingInfoTableModel::rowCount( const QModelIndex & parent) const
{
    return parent.isValid() ? 0 : m_routings.count();
}

int LogRoutingInfoTableModel::columnCount( const QModelIndex &) const
{
  return numColumns;
}

#include <QFile>
#include <QTextStream>

bool LogRoutingInfoTableModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
  if (role == Qt::EditRole)
  {
    SimpleLoggerRoutingInfo& info = m_routings[index.row()];
    QString s;
    switch (index.column())
    {
    case enabledColumn:
      info.setEnabled(value.toBool());
      break;
    case nameColumn:
      info.setName(value.toString());
      break;
    case locationRegExpColumn:
      if (!info.setLocationRegExp(value.toString()))
      {
        // TODO: Print an error!
        return false;
      }
      break;
    case messageRegExpColumn:
      if (!info.setMessageRegExp(value.toString()))
      {
        // TODO: Print an error!
        return false;
      }
      break;
    case routFileColumn:
      info.setRouting(SimpleLoggerRoutingInfo::RouteFile, value.toBool());
      break;
    case routScreenColumn:
      info.setRouting(SimpleLoggerRoutingInfo::RouteEmit, value.toBool());
      break;
    case routDebugColumn:
      info.setRouting(SimpleLoggerRoutingInfo::RouteQDebug, value.toBool());
      break;
    default:
      // No other column is setable here!
      return false;
      break;
    }
    emit dataChanged(index, index);
    return true;
  }
  return false;
}


QVariant LogRoutingInfoTableModel::data( const QModelIndex &index, int role ) const
{
  const SimpleLoggerRoutingInfo& info = m_routings[index.row()];
  switch( role )
  {
  case Qt::DisplayRole:
    switch (index.column())
    {
    case enabledColumn:
      return info.isEnabled();
      break;
    case nameColumn:
      return info.getName();
      break;
    case locationRegExpColumn:
      return info.getLocationRegExpString();
      break;
    case messageRegExpColumn:
      return info.getMessageRegExpString();
      break;
    case routFileColumn:
      return info.isRoutingOn(SimpleLoggerRoutingInfo::RouteFile);
      break;
    case routScreenColumn:
      return info.isRoutingOn(SimpleLoggerRoutingInfo::RouteEmit);
      break;
    case routDebugColumn:
      return info.isRoutingOn(SimpleLoggerRoutingInfo::RouteQDebug);
      break;
    case componentColumn:
      return info.formatMessage(tr("message"), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), SimpleLoggerRoutingInfo::ErrorMessage, 1);
      break;
    case levelsColumn:
      return info.getLevelsAsString();
      break;
    default:
      // No other column is valid
      break;
    }
    return (index.row()+1) * (index.column()+1);

  case Qt::EditRole:
    switch (index.column())
    {
    case enabledColumn:
      return info.isEnabled();
      break;
    case nameColumn:
      return info.getName();
      break;
    case locationRegExpColumn:
      return info.getLocationRegExpString();
      break;
    case messageRegExpColumn:
      return info.getMessageRegExpString();
      break;
    case routFileColumn:
      return info.isRoutingOn(SimpleLoggerRoutingInfo::RouteFile);
      break;
    case routScreenColumn:
      return info.isRoutingOn(SimpleLoggerRoutingInfo::RouteEmit);
      break;
    case routDebugColumn:
      return info.isRoutingOn(SimpleLoggerRoutingInfo::RouteQDebug);
      break;
    default:
      break;
    }

    return (index.row()+1) * (index.column()+1);
  case Qt::ToolTipRole:

    switch (index.column())
    {
    case enabledColumn:
      return tr("Enable or disable this routing.");
      break;
    case nameColumn:
      return tr("User recognizable name used only in this dialog.");
      break;
    case locationRegExpColumn:
      return tr("Regular expression pattern used to match the message source location.");
      break;
    case messageRegExpColumn:
      return tr("Regular expression pattern used to match the unformatted message.");
      break;
    case componentColumn:
      return tr("Components printed as part of the message.");
      break;
    case levelsColumn:
      return tr("Only messages with level less than or equal to the level are passed.");
      break;
      break;
    case routFileColumn:
      return tr("Send log messages to the log file.");
      break;
    case routScreenColumn:
      return tr("Send log messages to all log listeners such as the screen.");
      break;
    case routDebugColumn:
      return tr("Use qDebug to send log messages to the console.");
      break;
    default:
      return QVariant();
      break;
    }
  default:
    break;
  }
  return QVariant();
}

QVariant LogRoutingInfoTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( role != Qt::DisplayRole )
  {
    return QVariant();
  }
  const char* headers[] = {"Enabled", "Name", "Loc Reg Ex", "Msg Reg Ex", "To File", "To Screen", "To Console", "Levels", "Message"};
  if (orientation == Qt::Vertical)
  {
    return section + 1;
  }

  return (section < (int)(sizeof(headers)/sizeof(*headers))) ? QString(tr(headers[section])) : QVariant();
}

Qt::ItemFlags LogRoutingInfoTableModel::flags( const QModelIndex &index ) const
{
  if(!index.isValid())
    return Qt::ItemIsEnabled;

  // TODO: Based on column
  switch (index.column())
  {
  case enabledColumn:
  case nameColumn:
  case locationRegExpColumn:
  case messageRegExpColumn:
  case routFileColumn:
  case routScreenColumn:
  case routDebugColumn:
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    break;
  default:
    break;
  }

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void LogRoutingInfoTableModel::copyRouting(int row)
{
  if (row < 0)
  {
    return;
  }
  insertRouting(row, m_routings.at(row));
}

void LogRoutingInfoTableModel::insertRouting(int row, const SimpleLoggerRoutingInfo& routing)
{
  if (row < 0)
  {
    row = 0;
  } else if (row > m_routings.count())
  {
    row = m_routings.count();
  }

  beginInsertRows(QModelIndex(), row, row);
  m_routings.insert(row, routing);
  endInsertRows();
}

void LogRoutingInfoTableModel::updateRouting(int row, const SimpleLoggerRoutingInfo& routing)
{
  if (0 <= row && row < m_routings.count())
  {
    m_routings[row] = routing;
  }
}

void LogRoutingInfoTableModel::removeRouting(int row)
{
  if (0 <= row && row < m_routings.count())
  {
    beginRemoveRows(QModelIndex(), row, row);
    m_routings.removeAt(row);
    endRemoveRows();
  }
}

void LogRoutingInfoTableModel::moveRoutingUp(int row)
{
  if (0 < row && row < m_routings.count())
  {
    if (beginMoveRows(QModelIndex(), row, row, QModelIndex(), row- 1))
    {
      m_routings.move(row, row - 1);
      endMoveRows();
    }
  }
}

void LogRoutingInfoTableModel::moveRoutingDown(int row)
{
  if (0 <= row && row < m_routings.count() - 1)
  {
    // Yes, I really do need to use row+2, odd as it is!
    if (beginMoveRows(QModelIndex(), row, row, QModelIndex(), row + 2))
    {
      m_routings.move(row, row + 1);
      endMoveRows();
    }
  }
}

int LogRoutingInfoTableModel::count() const
{
  return m_routings.count();
}
