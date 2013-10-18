#ifndef MESSAGECATEGORYTABLEMODEL_H
#define MESSAGECATEGORYTABLEMODEL_H

#include <QAbstractTableModel>
#include "simpleloggerroutinginfo.h"

class MessageComponentTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  //**************************************************************************
  /*! Simple constructor.
   *  \param [in] parent Objects owner.
   ***************************************************************************/
  explicit MessageComponentTableModel(QObject *parent = 0);
  
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
   *  \param [in] MessageComponents MessageComponents to add to the current list.
   ***************************************************************************/
  void appendMessageComponents(const QList< QPair<SimpleLoggerRoutingInfo::MessageComponent, QString> >& components);

  //**************************************************************************
  /*! \brief Clear the list then add these MessageComponents to the list.
   *  \param [in] components MessageComponents that become the current list.
   ***************************************************************************/
  void setMessageComponents(const QList< QPair<SimpleLoggerRoutingInfo::MessageComponent, QString> >& components);

  //**************************************************************************
  /*! \brief Get the current MessageComponent lits.
   *  \return Current list of MessageComponents.
   ***************************************************************************/
  const QList< QPair<SimpleLoggerRoutingInfo::MessageComponent, QString> >& getMessageComponents() const;

  void insertMessageComponent(int row, const QPair<SimpleLoggerRoutingInfo::MessageComponent, QString>& component);
  void clear();
  void removeMessageComponent(int row);
  void moveMessageComponentUp(int row);
  void moveMessageComponentDown(int row);
  void copyMessageComponent(int row);

  int messageComponentCount() const;

private:
  /*! \brief Message components are printed in the order that they appear here. A non-empty date/time component causes the date/time to be formatted based on the included format string. */
  QList< QPair<SimpleLoggerRoutingInfo::MessageComponent, QString> > m_components;

public:
  static const int numMessageComponentTableModelColumns;
  static const int fieldColumn;
  static const int stringColumn;

signals:
  
public slots:
  
};

#endif // MESSAGECATEGORYTABLEMODEL_H
