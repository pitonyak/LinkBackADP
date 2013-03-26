#ifndef LINKBACKFILTERDELEGATE_H
#define LINKBACKFILTERDELEGATE_H

#include <QStyledItemDelegate>

//**************************************************************************
/*! \class LinkBackFilterDelegate
 *  \brief Delegate for the dialog for editing filters.
 *
 *  As the data grid is edited, this object is called.
 *  This delegate is associated to the filters table view in the BackupSetDialog.
 *  The data is stored in a FilterTableModel.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/
class LinkBackFilterDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit LinkBackFilterDelegate(QObject *parent = 0);

  //**************************************************************************
  /*! \brief Returns the widget used to edit the item specified by index for editing.
   *  \param [in, out] parent Used to control how the editor widget appears.
   *  \param [in] option Used to control how the editor widget appears.
   *  \param [in] index Indicates which column is of interest.
   *  \return Widget used to edit the specified data (so the editor is specific to the column).
   ***************************************************************************/
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    //**************************************************************************
    /*! \brief Sets the data to be displayed and edited by the editor from the data model item specified by the model index.
     *
     *  \param [in, out] editor will contain the data on exit.
     *  \param [in] index is the row and column of interest.
     ***************************************************************************/
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    //**************************************************************************
    /*! \brief Gets data from the editor widget and stores it in the specified model at the item index.
     *
     *  \param [in, out] editor from which the data is taken.
     *  \param [in] model contains the data that is displayed.
     *  \param [in] index is the row and column of interest.
     ***************************************************************************/
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

signals:

public slots:

};

#endif // LINKBACKFILTERDELEGATE_H
