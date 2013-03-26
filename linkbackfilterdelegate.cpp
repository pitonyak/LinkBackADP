#include "linkbackfilterdelegate.h"
#include <QLineEdit>
#include <QComboBox>
#include <typeinfo>
#include <QCheckBox>

LinkBackFilterDelegate::LinkBackFilterDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

#include <QFile>
#include <QTextStream>
#include <QCheckBox>

QWidget *LinkBackFilterDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,
                      const QModelIndex &index) const
{
  QVariant qvar = index.model()->data(index, Qt::EditRole);
  if (qvar.type() == QVariant::StringList)
  {
    return new QComboBox(parent);
  }
  else if (qvar.type() == QVariant::Bool)
  {
    QCheckBox* checkBox = new QCheckBox(parent);
    checkBox->setTristate(false);
    return checkBox;
  }
  return new QLineEdit(parent);
}

void LinkBackFilterDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QVariant qvar = index.model()->data(index, Qt::EditRole);
  if (qvar.type() == QVariant::StringList)
  {
    QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
    QStringList qsl = qvar.toStringList();
    if (qsl.count() == 1)
    {
      comboBox->addItems(qsl);
      comboBox->setCurrentIndex(0);
    }
    else if (qsl.count() > 1)
    {
      QString desiredItemText = qsl.at(0);
      qsl.removeAt(0);
      comboBox->addItems(qsl);
      for (int i=0; i<comboBox->count(); ++i)
      {
        if (QString::compare(comboBox->itemText(i), desiredItemText) == 0)
        {
          comboBox->setCurrentIndex(i);
          return;
        }
      }
      comboBox->setCurrentIndex(0);
    }
  }
  else if (qvar.type() == QVariant::Bool)
  {
    QCheckBox* checkBox = dynamic_cast<QCheckBox*>(editor);
    checkBox->setChecked(qvar.toBool());
  }
  else
  {
    QString s = qvar.toString();
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
    lineEdit->setText(s);
  }
}

void LinkBackFilterDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
  QVariant currentModelValue = index.model()->data(index, Qt::DisplayRole);
  QString s;
  if (typeid(*editor) ==  typeid(QComboBox))
  {
    QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
    s = comboBox->currentText();
  }
  else if (typeid(*editor) ==  typeid(QLineEdit))
  {
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
    s = lineEdit->displayText();
  }
  else if (typeid(*editor) ==  typeid(QCheckBox))
  {
    QCheckBox* checkBox = dynamic_cast<QCheckBox*>(editor);
    if (currentModelValue.toBool() != checkBox->isChecked())
    {
      model->setData(index, checkBox->isChecked(), Qt::EditRole);
    }
    return;
  }
  if (QString::compare(s, currentModelValue.toString(), Qt::CaseSensitive) != 0)
  {
    //qDebug(qPrintable(QString("Setting data (%1) current model (%2) index %3").arg(s, currentModelValue.toString(), QString::number(index.column()))));
    model->setData(index, s, Qt::EditRole);
  }
}

void LinkBackFilterDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &) const
{
  editor->setGeometry(option.rect);
}

void LinkBackFilterDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  // This feels wrong, I should not need to do this, but, it works.
  // If I do not do this, then the underlying text box shows through to the
  // Editing text box.
  if ((option.state & (QStyle::State_Active | QStyle::State_HasFocus | QStyle::State_Selected)) == QStyle::State_Selected) {
    QItemDelegate::drawBackground( painter, option, index );
   } else {
    QItemDelegate::paint( painter, option, index );
  }
}
