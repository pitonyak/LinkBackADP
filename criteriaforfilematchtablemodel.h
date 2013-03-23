#ifndef CRITERIAFORFILEMATCHTABLEMODEL_H
#define CRITERIAFORFILEMATCHTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "criteriaforfilematch.h"


//**************************************************************************
//! Implements interface for models that represent their data as a two-dimensional array of items. It is not used directly, but must be subclassed.
/*!
 * The QTableView class provides a default model/view implementation of a table view.
 * A QTableView requires a QAbstractTableModel, which contains the data to display.
 * This object represents the Model for Criteria data.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

class CriteriaForFileMatchTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  /*! \brief Main Constructor. */
  explicit CriteriaForFileMatchTableModel(QObject *parent = 0);

  //**************************************************************************
  //! Returns the number of rows under the given parent. When the parent is valid it means that rowCount is returning the number of children of parent.
  /*!
   * For a table based model, rowCount() returnd 0 when the parent is valid.
   * \param parent is used to locate data in a data model; contains row, column, and parent.
   * \returns 0 when the parent is valid, and number of rows in the table otherwise.
   *
   ***************************************************************************/
  int rowCount( const QModelIndex &parent = QModelIndex() ) const;

  //**************************************************************************
  //! Returns the number of columns for the children of the given parent; usually constant.
  /*!
   * \param parent is used to locate data in a data model; contains row, column, and parent.
   * \returns Number of columns in the data model.
   *
   ***************************************************************************/
  int columnCount( const QModelIndex &parent = QModelIndex() ) const;

  //**************************************************************************
  //! Returns the flags for the item referred to by the index.
  /*!
   * Determine if a specific "Item" is selectable, editable, drag/drop enabled, user checkable, enabled, or tristate.
   * \param index is used to locate data in a data model; contains row, column, and parent.
   * \returns
   *
   ***************************************************************************/
  Qt::ItemFlags flags( const QModelIndex &index ) const;

  //**************************************************************************
  //! Returns the data for the given role for the item referred to by the index.
  /*!
   * \param index is used to locate data in a data model; contains row, column, and parent.
   * \param role indicates purpose for the data request; for exapmle, display, editing, tooltip, size hint, etc.
   * \returns Data for the given role for the item referred to by the index.
   *
   ***************************************************************************/
  QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

  //**************************************************************************
  //! Sets the role data for the item at index to value.
  /*!
   * \param index is used to locate data in a data model; contains row, column, and parent.
   * \param value is the data to set.
   * \param role indicates purpose for the data request; for exapmle, display, editing, tooltip, size hint, etc.
   * \returns Returns true if successful; otherwise returns false.
   *
   ***************************************************************************/
  bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

  //**************************************************************************
  //! Returns the data for the given role and section in the header with the specified orientation.
  /*!
   * \param section For horizontal headers, the section number corresponds to the column number / name. Similarly, for vertical headers, the section number corresponds to the row number / name.
   * \param orientation indicates if the row (vertical) or column (horizontal) header is desired.
   * \param role indicates purpose for the data request; for exapmle, display, editing, tooltip, size hint, etc.
   * \returns Row or column header as specified by section and orientation.
   *
   ***************************************************************************/
  QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

  //**************************************************************************
  //! Returns an ordered list of criteria (as arranged in the table).
  /*!
   * \returns an ordered list of criteria (as arranged in the table).
   *
   ***************************************************************************/
  const QList<CriteriaForFileMatch>& getCriteria() const;

  //**************************************************************************
  //! Sets the criteria list as it will be shown in the table.
  /*!
   * \param criteria is the list of criteria to display in the table.
   *
   ***************************************************************************/
  void setCriteria(const QList<CriteriaForFileMatch>& criteria);

  /*! \brief Duplicate the currently selected Criteria in place (so there are two copies). */
  void copyCriteria(int row);

  /*! \brief Insert a new "default" critiria, pusshing the currently selected criteria down one. */
  void insertCriteria(int row, const CriteriaForFileMatch& criteria);

  /*! \brief Delete all criteria. */
  void clear();

  /*! \brief Delete the currently selected criteria. */
  void removeCriteria(int row);

  /*! \brief Move the currently selected criteria up one row. */
  void moveCriteriaUp(int row);

  /*! \brief Move the currently selected criteria down one row. */
  void moveCriteriaDown(int row);

  /*! \brief Number of Criteria data elements in the list. */
  int criteriaCount() const;

signals:

public slots:
private:
  /*! \brief List of Criteria objects. */
  QList<CriteriaForFileMatch> m_criteria;

};

#endif // CRITERIAFORFILEMATCHTABLEMODEL_H
