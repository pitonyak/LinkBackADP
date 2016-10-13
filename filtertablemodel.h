#ifndef FILTERTABLEMODEL_H
#define FILTERTABLEMODEL_H

#include <QAbstractTableModel>
#include "linkbackfilter.h"

/**************************************************************************
 * \class FilterTableModel
 * \brief Provide a list view for a list of Filters.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

class FilterTableModel : public QAbstractTableModel

{
public:
  //**************************************************************************
  /*! Simple constructor.
   *  \param [in] parent Objects owner.
   ***************************************************************************/
  explicit FilterTableModel(QObject *parent = nullptr );

  //**************************************************************************
  /*! \brief Returns the flags used to describe the item identified by the index.
   *
   *  Flags determine whether the item can be checked, edited, and selected.
   *  \param [in] index Identifies the item of interest based on a row and column.
   *  \return Flags that apply to the item specified by the index.
   ***************************************************************************/
  Qt::ItemFlags flags( const QModelIndex &index ) const;

  //**************************************************************************
  /*! \brief Returns the data for the the item identified by the index.
   *  \param [in] index Identifies the item of interest based on a row and column.
   *  \param [in] role Identifies the role for which the data should be returned. Data used for display may differ from that used for editing, for example.
   *  \return Returns the data stored under the given role for the item referred to by the index.
   ***************************************************************************/
  QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

  //**************************************************************************
  /*! \brief Sets the role data for the item at index to value.
   *  \param [in] index Identifies the item of interest based on a row and column.
   *  \param [in] value Data to set.
   *  \param [in] role Identifies the role for which the data should be set.
   *  \return Returns true if successful; otherwise returns false.
   ***************************************************************************/
  bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );


  //**************************************************************************
  /*! \brief Returns the data for the given role and section in the header with the specified orientation.
   *  \param [in] section For horizontal headers, the section number corresponds to the column number. Similarly, for vertical headers, the section number corresponds to the row number.
   *  \param [in] orientation Specifies if the header is horizontal or vertical.
   *  \param [in] role Identifies the role for which the data should be set.
   *  \return Returns the data for the given role and section in the header with the specified orientation.
   ***************************************************************************/
  QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

  //**************************************************************************
  /*! \brief Returns the number of rows under the given parent. When the parent is valid it means that rowCount is returning the number of children of parent.
   *
   *  When implementing a table based model, rowCount() should return 0 when the parent is valid.
   *  This implementation returns the number of filters when the parent is not valid.
   *  \param [in] parent Parent item when using a tree type model.
   *  \return Returns the number of rows under the given parent. When the parent is valid it means that rowCount is returning the number of children of parent.
   ***************************************************************************/
  int rowCount( const QModelIndex &parent = QModelIndex() ) const;

  //**************************************************************************
  /*! \brief Returns the number of columns for the children of the given parent.
   *
   *  \param [in] parent Parent item when using a tree type model.
   *  \return Returns the number of columns for the children of the given parent.
   ***************************************************************************/
  int columnCount( const QModelIndex &parent = QModelIndex() ) const;

  //**************************************************************************
  /*! \brief Append the filters to the internal filter list.
   *  \param [in] filters Filters to add to the current list.
   ***************************************************************************/
  void appendFilters(const QList<LinkBackFilter>& filters);

  //**************************************************************************
  /*! \brief Clear the list then add these filters to the list.
   *  \param [in] filters Filters that become the current list.
   ***************************************************************************/
  void setFilters(const QList<LinkBackFilter>& filters);

  //**************************************************************************
  /*! \brief Get the current filter lits.
   *  \return Current list of filters.
   ***************************************************************************/
  const QList<LinkBackFilter>& getFilters() const;

  void insertFilter(int row, const LinkBackFilter& filter);
  void clear();
  void removeFilter(int row);
  void moveFilterUp(int row);
  void moveFilterDown(int row);
  void copyFilter(int row);

  int filterCount() const;

private:
  QList<LinkBackFilter> m_filters;

public:
  static const int numFilterTableModelColumns;
  static const int fieldColumn;
  static const int methodColumn;
  static const int valueColumn;
  static const int acceptColumn;
  static const int caseColumn;
  static const int invertColumn;
  static const int filesColumn;
  static const int dirsColumn;
  static const int multiColumn;
};

#endif // FILTERTABLEMODEL_H
