#ifndef CHECKBOXONLYDELEGATE_H
#define CHECKBOXONLYDELEGATE_H

#include <QStyledItemDelegate>

class CheckBoxOnlyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CheckBoxOnlyDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
private:
    QRect CheckBoxRect(const QStyleOptionViewItem &view_item_style_options) const;

signals:

public slots:

};

#endif // CHECKBOXONLYDELEGATE_H
