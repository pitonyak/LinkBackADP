#include "linkbackfilterdelegate.h"
#include <QLineEdit>
#include <QComboBox>
#include <typeinfo>
#include <QCheckBox>

LinkBackFilterDelegate::LinkBackFilterDelegate(QObject *parent) :
  QStyledItemDelegate(parent)
{
}

#include <QFile>
#include <QTextStream>
#include <QCheckBox>

QWidget *LinkBackFilterDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                                              const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return nullptr;
  }
  QVariant qvar = index.model()->data(index, Qt::EditRole);
  if (qvar.typeId() == QMetaType::QStringList)
  {
    return new QComboBox(parent);
  }
  else if (qvar.typeId() == QMetaType::Bool)
  {
    QCheckBox* checkBox = new QCheckBox(parent);
    checkBox->setTristate(false);
    return checkBox;
  }

  //widget = new QLineEdit(parent);
  // Returns an expanding line editor.
  QWidget* widget = QStyledItemDelegate::createEditor(parent, option, index);

  // If I do not set this to true, then the "view" data shows through the text while editing.
  // Also, the expanding editor grows over existing controls, and you can see those through
  // the edit display as well (distracting).
  widget->setAutoFillBackground(true);
  return widget;
}

void LinkBackFilterDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return;
  }
  QVariant qvar = index.model()->data(index, Qt::EditRole);
  if (qvar.typeId() == QMetaType::QStringList)
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
  else if (qvar.typeId() == QMetaType::Bool)
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
  if (!index.isValid())
  {
    return;
  }
  QVariant currentModelValue = index.model()->data(index, Qt::DisplayRole);
  QString s;
  if (dynamic_cast<QComboBox*>(editor) != nullptr)
  {
    QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
    s = comboBox->currentText();
  }
  else if (dynamic_cast<QLineEdit*>(editor) != nullptr)
  {
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
    s = lineEdit->displayText();
  }
  else if (dynamic_cast<QCheckBox*>(editor) != nullptr)
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
    model->setData(index, s, Qt::EditRole);
  }
}
