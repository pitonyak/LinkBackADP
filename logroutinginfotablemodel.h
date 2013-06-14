#ifndef LOGROUTINGINFOTABLEMODEL_H
#define LOGROUTINGINFOTABLEMODEL_H

#include <QAbstractTableModel>
#include "simpleloggerroutinginfo.h"

class LogRoutingInfoTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit LogRoutingInfoTableModel(QObject *parent = 0);

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
   *  This implementation returns the number of MessageComponents when the parent is not valid.
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
  /*! \brief Append the MessageComponents to the internal MessageComponent list.
   *  \param [in] routing Routings to add to the current list.
   ***************************************************************************/
  void appendRoutings(const QList<SimpleLoggerRoutingInfo>& routing);

  //**************************************************************************
  /*! \brief Clear the list then add these MessageComponents to the list.
   *  \param [in] routing Routings that become the current list.
   ***************************************************************************/
  void setRoutings(const QList<SimpleLoggerRoutingInfo> &routing);

  //**************************************************************************
  /*! \brief Get the current MessageComponent lits.
   *  \return Current list of MessageComponents.
   ***************************************************************************/
  const QList<SimpleLoggerRoutingInfo>& getRoutings() const;

  void updateRouting(int row, const SimpleLoggerRoutingInfo& routing);
  void insertRouting(int row, const SimpleLoggerRoutingInfo& routing);
  void copyRouting(int row);
  void removeRouting(int row);
  void clear();
  void moveRoutingUp(int row);
  void moveRoutingDown(int row);

  int count() const;

private:
  //**************************************************************************
  /*! \brief List of routing objects.
   ***************************************************************************/
  QList<SimpleLoggerRoutingInfo> m_routings;

public:
  static const int numColumns;
  static const int nameColumn;
  static const int enabledColumn;
  static const int locationRegExpColumn;
  static const int messageRegExpColumn;
  static const int levelsColumn;
  static const int componentColumn;
  static const int routFileColumn;
  static const int routScreenColumn;
  static const int routDebugColumn;

signals:
  
public slots:
  
};

#endif // LOGROUTINGINFOTABLEMODEL_H
