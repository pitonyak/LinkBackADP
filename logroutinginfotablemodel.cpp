#include "logroutinginfotablemodel.h"


const int LogRoutingInfoTableModel::numColumns = 5;
const int LogRoutingInfoTableModel::enabledColumn = 0;
const int LogRoutingInfoTableModel::nameColumn = 1;
const int LogRoutingInfoTableModel::componentColumn = 2;
const int LogRoutingInfoTableModel::levelsColumn = 3;
const int LogRoutingInfoTableModel::regExpColumn = 4;

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

void LogRoutingInfoTableModel::appendMessageComponents(const QList<SimpleLoggerRoutingInfo> routing)
{
    clear();
    if (routing.count() > 0)
    {
        beginInsertRows(QModelIndex(), m_routings.count(), m_routings.count() + routing.count() - 1);
        m_routings.append(routing);
        endInsertRows();
    }
}

void LogRoutingInfoTableModel::setMessageComponents(const QList<SimpleLoggerRoutingInfo> routing)
{
    clear();
    if (routing.count() > 0)
    {
        beginInsertRows(QModelIndex(), 0, routing.count() - 1);
        m_routings.append(routing);
        endInsertRows();
    }
}

const QList<SimpleLoggerRoutingInfo> &LogRoutingInfoTableModel::getRoutings() const
{
    return m_routings;
}


int LogRoutingInfoTableModel::rowCount( const QModelIndex & parent) const
{
    return parent.isValid() ? 0 : m_routings.count();
}

int LogRoutingInfoTableModel::columnCount( const QModelIndex &) const
{
  return numMessageComponentTableModelColumns;
}

#include <QFile>
#include <QTextStream>

bool LogRoutingInfoTableModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
  if (role == Qt::EditRole)
  {
    QPair<SimpleLoggerRoutingInfo::MessageComponent, QString>& component = m_routings[index.row()];
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


QVariant LogRoutingInfoTableModel::data( const QModelIndex &index, int role ) const
{
  const QPair<SimpleLoggerRoutingInfo::MessageComponent, QString>& component = m_routings[index.row()];
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
      switch (component.first)
      {
      case SimpleLoggerRoutingInfo::DateTimeComponent:
        return tr("Date and time the message is logged. Format string is used to format log message if ISO date format is not desired.");
        break;
      case SimpleLoggerRoutingInfo::MessageLocationComponent:
        return tr("Filename and line number where log message originated. Format string is ignored.");
        break;
      case SimpleLoggerRoutingInfo::MessageTextComponent:
        return tr("The actual log message that was requested to be displayed to the user. Format string is ignored.");
        break;
      case SimpleLoggerRoutingInfo::MessageTypeComponent:
        return tr("Include the message type (such as Error or Info) in the log message. Message type is truncated to the length of the format text.");
        break;
      case SimpleLoggerRoutingInfo::MessageText:
        return tr("The format field text string is included verbatim in the log message.");
        break;
      default:
        return tr("Message Component that is printed.");
        break;
      }
      break;
    case stringColumn:
      switch (component.first)
      {
      case SimpleLoggerRoutingInfo::DateTimeComponent:
        return tr("Used to format date/time. If empty use ISO format. (y)ear, (M)onth, (d)ay, (h)our, (m)inute, (s)econd, (z)millisecond, (AP)or(ap) is AM/PM or am/pm.");
        break;
      case SimpleLoggerRoutingInfo::MessageLocationComponent:
        return tr("Format string is ignored for file location and line number.");
        break;
      case SimpleLoggerRoutingInfo::MessageTextComponent:
        return tr("Format string is ignored for text of the log message.");
        break;
      case SimpleLoggerRoutingInfo::MessageTypeComponent:
        return tr("Message type is truncated to the length of the format text, so X displays only the first letter. No text displays full type name such as Error.");
        break;
      case SimpleLoggerRoutingInfo::MessageText:
        return tr("This text is logged as entered.");
        break;
      default:
        return tr("Determines how the field is formatted.");
        break;
      }
    default:
      return tr("Help text requested for an unknown column!");
      break;
    }
    break;
  default:
    return QVariant();
  }
  return QVariant();
}

QVariant LogRoutingInfoTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
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

Qt::ItemFlags LogRoutingInfoTableModel::flags( const QModelIndex &index ) const
{
  if(!index.isValid())
    return Qt::ItemIsEnabled;

  // TODO: Based on column
  switch (index.column())
  {
  case enabledColumn:
  case nameColumn:
  case regExpColumn:
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    break;
  default:
    break;
  }

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void LogRoutingInfoTableModel::copyMessageComponent(int row)
{
  if (row < 0)
  {
    return;
  }
  insertMessageComponent(row, m_routings.at(row));
}

void LogRoutingInfoTableModel::insertMessageComponent(int row, const QPair<SimpleLoggerRoutingInfo::MessageComponent, QString>& component)
{
  if (row < 0)
  {
    row = 0;
  } else if (row > m_routings.count())
  {
    row = m_routings.count();
  }

  beginInsertRows(QModelIndex(), row, row);
  m_routings.insert(row, component);
  endInsertRows();
}

void LogRoutingInfoTableModel::removeMessageComponent(int row)
{
  if (0 <= row && row < m_routings.count())
  {
    beginRemoveRows(QModelIndex(), row, row);
    m_routings.removeAt(row);
    endRemoveRows();
  }
}

void LogRoutingInfoTableModel::moveMessageComponentUp(int row)
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

void LogRoutingInfoTableModel::moveMessageComponentDown(int row)
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

int LogRoutingInfoTableModel::messageComponentCount() const
{
  return m_routings.count();
}

