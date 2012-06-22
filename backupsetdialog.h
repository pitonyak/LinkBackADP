#ifndef BACKUPSETDIALOG_H
#define BACKUPSETDIALOG_H

#include <QDialog>

#include "backupset.h"
#include "filtertablemodel.h"
#include "criteriaforfilematchtablemodel.h"

namespace Ui {
class BackupSetDialog;
}

//**************************************************************************
//! Provide a list view for a list of Filters.
/*!
 *  \author Andrew Pitonyak
 *  This routine is Copyright (c) by Andrew Pitonyak, and may be used
 *  without restrictions or renumerations. I make no statement of correctness.
 ***************************************************************************/

class BackupSetDialog : public QDialog
{
  Q_OBJECT

public:
  explicit BackupSetDialog(QWidget *parent = 0);
  explicit BackupSetDialog(const BackupSet& backupSet, QWidget *parent = 0);

  ~BackupSetDialog();

  /*! \brief Get a backup set object representing the current state of the dialog.
   *
   *  A new backup set object is created and returned.
   *  \return Object representing the current state of the dialog.
   */
  BackupSet getBackupSet() const;

  /*! \brief Configure the dialog to use the specified backup set.
   *
   *  Each field in the dialog is set based on the backup set parameter.
   *  \param [in] backupSet object used to configure the dialog.
   *  \return Flags that apply to the item specified by the index.
   */
  void setBackupSet(const BackupSet& backupSet);

  QString getConfigFilePath() const;
  void setConfigFilePath(const QString& path);

private:
  /*! \brief Set list columns, delegates, and initial values. */
  void initialize();

  int getSelectedFilterRow() const;

  /*! \brief Enable/disable buttons based on dialog values. */
  void enableButtons();

  bool isFilterSelected() const;
  bool isCriteriaSelected() const;

public slots:
  void copyFilter();
  void insertFilter();
  void delFilter();
  void upFilter();
  void downFilter();

  void copyCriteria();
  void insertCriteria();
  void delCriteria();
  void upCriteria();
  void downCriteria();

  void selectFromRoot();
  void selectToRoot();
  void selectHashMethod(const QString& method);
  void saveBackupSet();

  /*! \brief Open a dialog and ask from where to load a backup set.
     *
     *  The user selects the file to load. The backup set object is used to read
     *  the file. A call to setBackupSet is used to set the values in the dialog.
     */
  void loadBackupSet();

  void currentFilterRowChanged ( const QModelIndex & current, const QModelIndex &    previous );
  void currentCriteriaRowChanged ( const QModelIndex & current, const QModelIndex &    previous );


private:
  Ui::BackupSetDialog *ui;
  FilterTableModel m_filterTableModel;
  CriteriaForFileMatchTableModel m_criteriaForFileMatchTableModel;

};

#endif // BACKUPSETDIALOG_H
