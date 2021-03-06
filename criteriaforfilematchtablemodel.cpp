#include "criteriaforfilematchtablemodel.h"
#include "linkbackupglobals.h"
#include <cmath>

CriteriaForFileMatchTableModel::CriteriaForFileMatchTableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int CriteriaForFileMatchTableModel::rowCount( const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_criteria.count();
}

int CriteriaForFileMatchTableModel::columnCount( const QModelIndex &) const
{
  return 5;
}

const QList<CriteriaForFileMatch>& CriteriaForFileMatchTableModel::getCriteria() const
{
    return m_criteria;
}

void CriteriaForFileMatchTableModel::setCriteria(const QList<CriteriaForFileMatch>& criteria)
{
    clear();
    if (criteria.count() > 0)
    {
        beginInsertRows(QModelIndex(), 0, criteria.count() - 1);
        m_criteria.append(criteria);
        endInsertRows();
    }
}

Qt::ItemFlags CriteriaForFileMatchTableModel::flags( const QModelIndex &index ) const
{
  if(!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QVariant CriteriaForFileMatchTableModel::data( const QModelIndex &index, int role) const
{
  const CriteriaForFileMatch& criteria = m_criteria[index.row()];

  switch( role )
  {
  case Qt::DisplayRole:
  case Qt::EditRole:
    return criteria.getField(static_cast<CriteriaForFileMatch::CriteriaField>((int)pow(2, index.column())));
    break;
  case Qt::ToolTipRole:
    switch (index.column())
    {
    case CriteriaForFileMatch::FileName:
      return "Filename must match.";
      break;
    case CriteriaForFileMatch::FullPath:
      return "Full path to the file must match.";
      break;
    case CriteriaForFileMatch::Time:
      return "File last modified date and time must match.";
      break;
    case CriteriaForFileMatch::Size:
      return "Filesize must match. I expect that this should ALWAYS be checked.";
      break;
    case CriteriaForFileMatch::Hash:
      return "File hash functions must match. File size and hash are pretty good indicators.";
      break;
    default:
      return "Unkonwn!";
      break;
    }
    break;
  default:
    break;
  }
  return QVariant();
}

bool CriteriaForFileMatchTableModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
  if (role == Qt::EditRole)
  {
    // According to http://qt-project.org/doc/qt-5.0/qtcore/qabstractitemmodel.html#setData
    // The dataChanged() signal should be emitted if the data was successfully set.
    // Must I do this specifically?
    //
    // Only set if the data has changed?
    //

    bool newValue = value.toBool();

    // Field of interest is a binary flag, the column is a zero indexed number from zero.
    // Convert 0->1, 1->2, 2->4, 3->8, etc.
    CriteriaForFileMatch::CriteriaField fieldOfInterest = static_cast<CriteriaForFileMatch::CriteriaField>(pow(2, index.column()));
    CriteriaForFileMatch& criteria = m_criteria[index.row()];

    if (criteria.getField(fieldOfInterest) != newValue) {
      criteria.setField(fieldOfInterest, newValue);
      emit dataChanged(index, index);
    }
    return true;
  }
  return false;
}

QVariant CriteriaForFileMatchTableModel::headerData( int section, Qt::Orientation orientation, int role) const
{
  if( role != Qt::DisplayRole )
  {
    return QVariant();
  }
  const char* headers[] = {"Name", "Full Path", "Date/Time", "Size", "Hash"};
  if (orientation == Qt::Vertical)
  {
    return section + 1;
  }

  return (section < (int)(sizeof(headers)/sizeof(*headers))) ? QString(headers[section]) : QVariant();
}

void CriteriaForFileMatchTableModel::copyCriteria(int row)
{
  if (row < 0 || m_criteria.count() <= row)
  {
    return;
  }
  insertCriteria(row, m_criteria.at(row));
}

void CriteriaForFileMatchTableModel::insertCriteria(int row, const CriteriaForFileMatch& criteria)
{
  if (row < 0)
  {
    row = 0;
  } else if (row > m_criteria.count())
  {
    row = m_criteria.count();
  }

  beginInsertRows(QModelIndex(), row, row);
  m_criteria.insert(row, criteria);
  endInsertRows();
}

void CriteriaForFileMatchTableModel::clear()
{
  if (m_criteria.count() > 0)
  {
    beginRemoveRows(QModelIndex(), 0, m_criteria.count() - 1);
    m_criteria.clear();
    endRemoveRows();
  }
}

void CriteriaForFileMatchTableModel::removeCriteria(int row)
{
  if (0 <= row && row < m_criteria.count())
  {
    beginRemoveRows(QModelIndex(), row, row);
    m_criteria.removeAt(row);
    endRemoveRows();
  }
}

void CriteriaForFileMatchTableModel::moveCriteriaUp(int row)
{
  if (0 < row && row < m_criteria.count())
  {
    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row- 1);
    m_criteria.move(row, row - 1);
    endMoveRows();
  }
}

void CriteriaForFileMatchTableModel::moveCriteriaDown(int row)
{
  if (0 <= row && row < m_criteria.count() - 1)
  {
    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row + 1);
    m_criteria.move(row, row + 1);
    endMoveRows();
  }
}

int CriteriaForFileMatchTableModel::criteriaCount() const
{
  return m_criteria.count();
}


