#ifndef CRITERIAFORFILEMATCHTABLEMODEL_H
#define CRITERIAFORFILEMATCHTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "criteriaforfilematch.h"


class CriteriaForFileMatchTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit CriteriaForFileMatchTableModel(QObject *parent = 0);

  int rowCount( const QModelIndex &parent = QModelIndex() ) const;
  int columnCount( const QModelIndex &parent = QModelIndex() ) const;

  Qt::ItemFlags flags( const QModelIndex &index ) const;
  QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
  bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

  QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

  const QList<CriteriaForFileMatch>& getCriteria() const;
  void setCriteria(const QList<CriteriaForFileMatch>& criteria);

  void copyCriteria(int row);
  void insertCriteria(int row, const CriteriaForFileMatch& criteria);
  void clear();
  void removeCriteria(int row);
  void moveCriteriaUp(int row);
  void moveCriteriaDown(int row);
  int criteriaCount() const;

signals:

public slots:
private:
  QList<CriteriaForFileMatch> m_criteria;

};

#endif // CRITERIAFORFILEMATCHTABLEMODEL_H
