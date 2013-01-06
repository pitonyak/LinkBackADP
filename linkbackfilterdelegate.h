#ifndef LINKBACKFILTERDELEGATE_H
#define LINKBACKFILTERDELEGATE_H

#include <QItemDelegate>

//**************************************************************************
//! Delegate for the dialog for editing filters.
/*!
 *  As the data grid is edited, this object is called.
 *  This delegate is associated to the filters table view in the BackupSetDialog.
 *  The data is stored in a FilterTableModel.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/
class LinkBackFilterDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit LinkBackFilterDelegate(QObject *parent = 0);

  /*! \brief Returns the widget used to edit the item specified by index for editing.
   *  \param [in, out] parent Used to control how the editor widget appears.
   *  \param [in] option Used to control how the editor widget appears.
   *  \param [in] index Indicates which column is of interest.
   *  \return Widget used to edit the specified data (so the editor is specific to the column).
   */
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    /*! \brief Set the data into the editor for the location based on the mode.
     *  The data is obtained from the model.
     */
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // LINKBACKFILTERDELEGATE_H
