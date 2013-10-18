#include "filtertablemodel.h"

#include <QMetaObject>
#include <QMetaEnum>
#include <QStringList>
#include <QVariant>

const int FilterTableModel::numFilterTableModelColumns = 9;
const int FilterTableModel::fieldColumn = 0;
const int FilterTableModel::methodColumn = 1;
const int FilterTableModel::valueColumn = 2;
const int FilterTableModel::acceptColumn = 3;
const int FilterTableModel::caseColumn = 4;
const int FilterTableModel::invertColumn = 5;
const int FilterTableModel::filesColumn = 6;
const int FilterTableModel::dirsColumn = 7;
const int FilterTableModel::multiColumn = 8;


FilterTableModel::FilterTableModel(QObject *parent ) :
  QAbstractTableModel( parent )
{
}

void FilterTableModel::clear()
{
  if (m_filters.count() > 0)
  {
    beginRemoveRows(QModelIndex(), 0, m_filters.count() - 1);
    m_filters.clear();
    endRemoveRows();
  }
}

void FilterTableModel::appendFilters(const QList<LinkBackFilter>& filters)
{
    clear();
    if (filters.count() > 0)
    {
        beginInsertRows(QModelIndex(), m_filters.count(), m_filters.count() + filters.count() - 1);
        m_filters.append(filters);
        endInsertRows();
    }
}

void FilterTableModel::setFilters(const QList<LinkBackFilter>& filters)
{
    clear();
    if (filters.count() > 0)
    {
        beginInsertRows(QModelIndex(), 0, filters.count() - 1);
        m_filters.append(filters);
        endInsertRows();
    }
}

const QList<LinkBackFilter>& FilterTableModel::getFilters() const
{
    return m_filters;
}


int FilterTableModel::rowCount( const QModelIndex & parent) const
{
    return parent.isValid() ? 0 : m_filters.count();
}

int FilterTableModel::columnCount( const QModelIndex &) const
{
  return numFilterTableModelColumns;
}

#include <QFile>
#include <QTextStream>

bool FilterTableModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
  if (role == Qt::EditRole)
  {
    LinkBackFilter& filter = m_filters[index.row()];
    const QMetaObject* metaObj = filter.metaObject();
    QString s;
    switch (index.column())
    {
    case fieldColumn:
      {
        int i = metaObj->enumerator(metaObj->indexOfEnumerator("CompareField")).keyToValue(qPrintable(value.toString()));
        filter.setCompareField(static_cast<LinkBackFilter::CompareField>(i));
      }
      break;
    case methodColumn:
      s = value.toString();
      if (QString::compare(s, "<") == 0)
      {
        filter.setCompareType(LinkBackFilter::Less);
      }
      else if (QString::compare(s, "<=") == 0)
      {
        filter.setCompareType(LinkBackFilter::LessEqual);
      }
      else if (QString::compare(s, "==") == 0)
      {
        filter.setCompareType(LinkBackFilter::Equal);
      }
      else if (QString::compare(s, ">=") == 0)
      {
        filter.setCompareType(LinkBackFilter::GreaterEqual);
      }
      else if (QString::compare(s, ">") == 0)
      {
        filter.setCompareType(LinkBackFilter::Greater);
      }
      else if (QString::compare(s, "!=") == 0)
      {
        filter.setCompareType(LinkBackFilter::NotEqual);
      }
      else if (QString::compare(s, "RegularExpression") == 0)
      {
        filter.setCompareType(LinkBackFilter::RegularExpression);
      }
      else if (QString::compare(s, "FileSpec") == 0)
      {
        filter.setCompareType(LinkBackFilter::FileSpec);
      }
      else if (QString::compare(s, "Contains") == 0)
      {
        filter.setCompareType(LinkBackFilter::Contains);
      }
      else
      {
        return false;
      }
      break;
    case valueColumn:
      filter.setValue(value.toString());
      break;
    case acceptColumn:
      filter.setFilterMeansAccept(QString::compare("accept", value.toString()) == 0);
      break;
    case caseColumn:
      filter.setCaseSensitivity(value.toBool() ? Qt::CaseSensitive : Qt::CaseInsensitive);
      break;
    case invertColumn:
      filter.setInvertFilterResult(QString::compare(value.toString(), "true") == 0);
      break;
    case filesColumn:
      filter.setFilterFiles(value.toBool());
      break;
    case dirsColumn:
      filter.setFilterDirs(value.toBool());
      break;
    case multiColumn:
      filter.setMultiValued(value.toBool());
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


QVariant FilterTableModel::data( const QModelIndex &index, int role ) const
{
  const LinkBackFilter& filter = m_filters[index.row()];
  const QMetaObject* metaObj = filter.metaObject();
  switch( role )
  {
  case Qt::DisplayRole:
    switch (index.column())
    {
    case fieldColumn:
      return QLatin1String(metaObj->enumerator(metaObj->indexOfEnumerator("CompareField")).valueToKey(filter.getCompareField()));
      break;
    case methodColumn:
      switch(filter.getCompareType())
      {
      case LinkBackFilter::Less:
        return "<";
        break;
      case LinkBackFilter::LessEqual:
        return "<=";
        break;
      case LinkBackFilter::Equal:
        return "==";
        break;
      case LinkBackFilter::GreaterEqual:
        return ">=";
        break;
      case LinkBackFilter::Greater:
        return ">";
        break;
      case LinkBackFilter::NotEqual:
        return "!=";
        break;
      case LinkBackFilter::RegularExpression:
        return "RegularExpression";
        break;
      case LinkBackFilter::FileSpec:
        return "FileSpec";
        break;
      case LinkBackFilter::Contains:
        return "Contains";
        break;
      default:
        break;
      }

      return QLatin1String(metaObj->enumerator(metaObj->indexOfEnumerator("CompareType")).valueToKey(filter.getCompareType()));
      break;
    case valueColumn:
      return filter.getMainValueAsString();
      break;
    case acceptColumn:
      return filter.isFilterMeansAccept() ? "accept" : "reject";
      break;
    case caseColumn:
      return (filter.getCaseSensitivity() == Qt::CaseSensitive);
      break;
    case invertColumn:
      return filter.isInvertFilterResult();
      break;
    case filesColumn:
      return filter.isFilterFiles();
      break;
    case dirsColumn:
      return filter.isFilterDirs();
      break;
    case multiColumn:
      return filter.isMultiValued();
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
        QMetaEnum metaEnum = metaObj->enumerator(metaObj->indexOfEnumerator("CompareField"));
        QStringList qsl;
        qsl << metaEnum.valueToKey(filter.getCompareField());
        for (int i=0; i<metaEnum.keyCount(); ++i)
        {
          qsl << metaEnum.valueToKey(i);
        }
        return QVariant(qsl);
      }
      break;
    case methodColumn:
      {
        QStringList qsl;
        qsl << data(index, Qt::DisplayRole).toString();
        qsl << "<" << "<=" << "==" << ">" << ">=" << "!=";
        // If I do not want to support all comparison types for all fields,
        // then change things here.
        qsl << "RegularExpression" << "FileSpec" << "Contains";
        return QVariant(qsl);
      }
      break;
    case valueColumn:
      return filter.getMainValueAsString();
      break;
    case acceptColumn:
      {
        QStringList qsl;
        qsl << data(index, Qt::DisplayRole).toString();
        qsl << "accept" << "reject";
        return qsl;
      }
      break;
    case caseColumn:
      {
        return (filter.getCaseSensitivity() == Qt::CaseSensitive);
      }
      break;
    case invertColumn:
      return filter.isInvertFilterResult();
      break;
    case filesColumn:
      return filter.isFilterFiles();
      break;
    case dirsColumn:
      return filter.isFilterDirs();
      break;
    case multiColumn:
      return filter.isMultiValued();
      break;
    default:
      break;
    }

    return (index.row()+1) * (index.column()+1);
  case Qt::ToolTipRole:

    switch (index.column())
    {
    case fieldColumn:
      return tr("The filter will act on this field for the file or directory in question.");
      break;
    case methodColumn:
      return tr("How the file propery is compared to the filter value.");
    case valueColumn:
      return tr("Value compared against the file property.");
      break;
    case acceptColumn:
      return tr("Determines if passing this filter cause the file to be accepted or rejected.");
      break;
    case caseColumn:
      return tr("Determines if the filter value is compared in a CaseSensitive way.");
      break;
    case invertColumn:
      return tr("If TRUE, the ineverse of the filter value is used. So, size=10 inverted is the same as size!=10.");
      break;
    case filesColumn:
      return tr("If TRUE, the filter applies to files.");
      break;
    case dirsColumn:
      return tr("If TRUE, the filter applies to directories.");
      break;
    case multiColumn:
      return tr("If TRUE, text input is split by commas and the filter is applied once for each value. This is a shortcut to entering multiple filters that differ only in the value.");
      break;
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

QVariant FilterTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( role != Qt::DisplayRole )
  {
    return QVariant();
  }
  const char* headers[] = {"Field", "Method", "Value", "Accept/Reject", "Case Sensitive", "Invert Filter", "Files", "Dirs", "Multi"};
  if (orientation == Qt::Vertical)
  {
    return section + 1;
  }

  return (section < (int)(sizeof(headers)/sizeof(*headers))) ? QString(tr(headers[section])) : QVariant();
}

Qt::ItemFlags FilterTableModel::flags( const QModelIndex &index ) const
{
  if(!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void FilterTableModel::copyFilter(int row)
{
  if (row < 0)
  {
    return;
  }
  insertFilter(row, m_filters.at(row));
}

void FilterTableModel::insertFilter(int row, const LinkBackFilter& filter)
{
  if (row < 0)
  {
    row = 0;
  } else if (row > m_filters.count())
  {
    row = m_filters.count();
  }

  beginInsertRows(QModelIndex(), row, row);
  m_filters.insert(row, filter);
  endInsertRows();
}

void FilterTableModel::removeFilter(int row)
{
  if (0 <= row && row < m_filters.count())
  {
    beginRemoveRows(QModelIndex(), row, row);
    m_filters.removeAt(row);
    endRemoveRows();
  }
}

void FilterTableModel::moveFilterUp(int row)
{
  if (0 < row && row < m_filters.count())
  {
    if (beginMoveRows(QModelIndex(), row, row, QModelIndex(), row- 1))
    {
      m_filters.move(row, row - 1);
      endMoveRows();
    }
  }
}

void FilterTableModel::moveFilterDown(int row)
{
  if (0 <= row && row < m_filters.count() - 1)
  {
    // Yes, I really do need to use row+2, odd as it is!
    if (beginMoveRows(QModelIndex(), row, row, QModelIndex(), row + 2))
    {
      m_filters.move(row, row + 1);
      endMoveRows();
    }
  }
}

int FilterTableModel::filterCount() const
{
  return m_filters.count();
}


