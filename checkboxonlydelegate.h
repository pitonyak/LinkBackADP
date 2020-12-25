#ifndef CHECKBOXONLYDELEGATE_H
#define CHECKBOXONLYDELEGATE_H

#include <QStyledItemDelegate>

//**************************************************************************
/*! \class CheckBoxOnlyDelegate
 * \brief Provides display and editing capabilitye interpreting booling data to be viewed and edited as a checkbox.
 * A normal check box includes text to the right of the checkbox.
 * This delegate displays only the checkbox without the text.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2020
 ***************************************************************************/

class CheckBoxOnlyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    //**************************************************************************
    //! Default Constructor
    /*!
     * \param [in] parent This is a QObject, so you can set a parent that will control the lifetime of this object.
     ***************************************************************************/
    explicit CheckBoxOnlyDelegate(QObject *parent = 0);

    //**************************************************************************
    //! Renders the checkbox using the given painter and style option for the item specified by index.
    /*!
     * \param [in,out] painter object to which the box is rendered.
     * \param [in] option Dictates how the check box is drawn (enabled, disabled, etc).
     * \param [in] index Identifies the data in the model (think row and/or column)
     ***************************************************************************/
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //**************************************************************************
    //! Called when editing begins. Handles mouse and keyboard, updating the data if it changes.
    /*!
     * \param [in] event Event that occurred; for example, mouse event or keyboard event.
     * \param [in,out] model Contains the data model. This is toggled between true and false as the check box is checked and unchecked.
     * \param [in] option Indicates state of the checkbox.
     * \param [in] index Identifies the data in the model (think row and/or column)
     *
     * \returns True if the event was handled, false otherwise.
     ***************************************************************************/
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
private:
    //**************************************************************************
    //! Get the rectangle that surrounds the checkbox (for drawing purposes).
    /*!
     * \param [in] view_item_style_options Indicates state of the checkbox.
     *
     * \returns Rectangle that surrounds the check box.
     ***************************************************************************/
    QRect CheckBoxRect(const QStyleOptionViewItem &view_item_style_options) const;

signals:

public slots:

};

#endif // CHECKBOXONLYDELEGATE_H
