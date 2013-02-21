#include "backupsetdialog.h"
#include "ui_backupsetdialog.h"
#include "linkbackfilterdelegate.h"
#include "criteriaforfilematch.h"
#include "linkbackupglobals.h"
#include "checkboxonlydelegate.h"

#include <QMessageBox>
#include <QFileDialog>

BackupSetDialog::BackupSetDialog(QWidget *parent) : QDialog(parent), ui(new Ui::BackupSetDialog)
{
  initialize();
}

BackupSetDialog::BackupSetDialog(const BackupSet& backupSet, QWidget *parent): QDialog(parent), ui(new Ui::BackupSetDialog)
{
  initialize();
  setBackupSet(backupSet);
}

QString BackupSetDialog::getConfigFilePath() const
{
  return ui->configFileLineEdit->text();
}

void BackupSetDialog::setConfigFilePath(const QString& path)
{
  ui->configFileLineEdit->setText(path);
}


BackupSet BackupSetDialog::getBackupSet() const
{
    BackupSet backupSet;
    backupSet.setCriteria(m_criteriaForFileMatchTableModel.getCriteria());
    backupSet.setFilters(m_filterTableModel.getFilters());
    backupSet.setToPath(ui->toRootLineEdit->text());
    backupSet.setFromPath(ui->fromRootLineEdit->text());
    backupSet.setHashMethod(ui->hashComboBox->currentText());
    return backupSet;
}

void BackupSetDialog::setBackupSet(const BackupSet& backupSet)
{
    TRACE_MSG("Setting filters", 10);
    m_filterTableModel.setFilters(backupSet.getFilters());
    TRACE_MSG("Setting Criteria", 10);
    m_criteriaForFileMatchTableModel.setCriteria(backupSet.getCriteria());
    TRACE_MSG("Setting line edits", 10);
    ui->toRootLineEdit->setText(backupSet.getToPath());
    ui->fromRootLineEdit->setText(backupSet.getFromPath());
    QString hashMethod = backupSet.getHashMethod();
    TRACE_MSG("Setting hash combo box", 10);
    for (int i=0; i<ui->hashComboBox->count(); ++i) {
        if (QString::compare(hashMethod, ui->hashComboBox->itemText(i), Qt::CaseInsensitive) == 0) {
            ui->hashComboBox->setCurrentIndex(i);
            break;
        }
    }
    TRACE_MSG("Leaving setBackupSet", 10);
}

void BackupSetDialog::initialize()
{
  ui->setupUi(this);
  ui->filtersTableView->setModel(&(m_filterTableModel));
  LinkBackFilterDelegate* delegate = new LinkBackFilterDelegate(ui->filtersTableView);
  ui->filtersTableView->setItemDelegate(delegate);
  CheckBoxOnlyDelegate * cboDelegate = new CheckBoxOnlyDelegate(ui->filtersTableView);
  ui->filtersTableView->setItemDelegateForColumn(m_filterTableModel.invertColumn, cboDelegate);
  ui->filtersTableView->setItemDelegateForColumn(m_filterTableModel.caseColumn, cboDelegate);
  ui->filtersTableView->setItemDelegateForColumn(m_filterTableModel.filesColumn, cboDelegate);
  ui->filtersTableView->setItemDelegateForColumn(m_filterTableModel.dirsColumn, cboDelegate);
  ui->filtersTableView->setItemDelegateForColumn(m_filterTableModel.multiColumn, cboDelegate);

  ui->filtersTableView->setColumnWidth(m_filterTableModel.fieldColumn, 120);
  ui->filtersTableView->setColumnWidth(m_filterTableModel.methodColumn, 140);
  ui->filtersTableView->setColumnWidth(m_filterTableModel.invertColumn, 100);
  ui->filtersTableView->setColumnWidth(m_filterTableModel.acceptColumn, 110);
  ui->filtersTableView->setColumnWidth(m_filterTableModel.caseColumn, 125);
  ui->filtersTableView->setColumnWidth(m_filterTableModel.valueColumn, 325);
  ui->filtersTableView->setColumnWidth(m_filterTableModel.filesColumn, 70);
  ui->filtersTableView->setColumnWidth(m_filterTableModel.dirsColumn, 70);
  ui->filtersTableView->setColumnWidth(m_filterTableModel.multiColumn, 70);

  ui->criteriaTableView->setItemDelegate(new CheckBoxOnlyDelegate(ui->criteriaTableView));
  ui->criteriaTableView->setModel(&(m_criteriaForFileMatchTableModel));

  ui->hashComboBox->addItem("SHA1");
  ui->hashComboBox->addItem("SHA512");
  ui->hashComboBox->addItem("SHA256");
  ui->hashComboBox->addItem("SHA384");
  ui->hashComboBox->addItem("SHA224");

  ui->hashComboBox->addItem("MD4");
  ui->hashComboBox->addItem("MD5");

  // To enable and disable buttons on selection change.
  connect(ui->filtersTableView->selectionModel(), SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)), this, SLOT(currentFilterRowChanged ( const QModelIndex &, const QModelIndex &)));
  connect(ui->criteriaTableView->selectionModel(), SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)), this, SLOT(currentCriteriaRowChanged ( const QModelIndex &, const QModelIndex &)));
  enableButtons();
}

bool BackupSetDialog::isFilterSelected() const
{
  return m_filterTableModel.filterCount() > 0 && ui->filtersTableView->currentIndex().row() >= 0;
}

bool BackupSetDialog::isCriteriaSelected() const
{
  return m_criteriaForFileMatchTableModel.criteriaCount() > 0 && ui->criteriaTableView->currentIndex().row() >= 0;
}


void BackupSetDialog::enableButtons()
{
  bool b = isFilterSelected();
  ui->copyFilterButton->setEnabled(b);
  ui->delFilterButton->setEnabled(b);

  b = isCriteriaSelected();
  ui->copyCriteriaButton->setEnabled(b);
  ui->deleteCriteriaButton->setEnabled(b);
}

BackupSetDialog::~BackupSetDialog()
{
    delete ui;
    ui = 0;
}

int BackupSetDialog::getSelectedFilterRow() const
{
  return ui->filtersTableView->currentIndex().row();
}


void BackupSetDialog::copyFilter()
{
  m_filterTableModel.copyFilter(getSelectedFilterRow());
}

void BackupSetDialog::insertFilter()
{
  LinkBackFilter newFilter;
  m_filterTableModel.insertFilter(getSelectedFilterRow(), newFilter);
}

void BackupSetDialog::delFilter()
{
  m_filterTableModel.removeFilter(getSelectedFilterRow());
}

void BackupSetDialog::upFilter()
{
  m_filterTableModel.moveFilterUp(getSelectedFilterRow());
}

void BackupSetDialog::downFilter()
{
  m_filterTableModel.moveFilterDown(getSelectedFilterRow());
}

void setLocalValues(const LinkBackFilter& )
{
  // TODO: Fix this. This should create a standard set of values.
}

void BackupSetDialog::copyCriteria()
{
  m_criteriaForFileMatchTableModel.copyCriteria(ui->criteriaTableView->currentIndex().row());
}

void BackupSetDialog::insertCriteria()
{
  TRACE_MSG("Enter BackupSetDialog::insertCriteria", 10);
  CriteriaForFileMatch newCriteria;
  m_criteriaForFileMatchTableModel.insertCriteria(ui->criteriaTableView->currentIndex().row(), newCriteria);
  TRACE_MSG("Exit BackupSetDialog::insertCriteria", 10);
}

void BackupSetDialog::delCriteria()
{
  m_criteriaForFileMatchTableModel.removeCriteria(ui->criteriaTableView->currentIndex().row());
}

void BackupSetDialog::upCriteria()
{
  m_criteriaForFileMatchTableModel.moveCriteriaUp(ui->criteriaTableView->currentIndex().row());
}

void BackupSetDialog::downCriteria()
{
  m_criteriaForFileMatchTableModel.moveCriteriaDown(ui->criteriaTableView->currentIndex().row());
}

void BackupSetDialog::selectFromRoot()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Backup From Directory"), ui->fromRootLineEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (dir.length() > 0)
  {
    ui->fromRootLineEdit->setText(dir);
  }
}

void BackupSetDialog::selectToRoot()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Backup To Directory"), ui->toRootLineEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (dir.length() > 0)
  {
    ui->toRootLineEdit->setText(dir);
  }
}

void BackupSetDialog::selectHashMethod(const QString&)
{
}

void BackupSetDialog::saveBackupSet()
{
    QString defaultExtension = tr("XML files (*.xml)");
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), ui->configFileLineEdit->text(), tr("Text files (*.txt);;XML files (*.xml)"), &defaultExtension);
    if (!filePath.isEmpty())
    {
        ui->configFileLineEdit->setText(filePath);
        BackupSet backupSet = getBackupSet();
        if (backupSet.writeFile(filePath))
        {
            //QMessageBox::information(this, tr("File Saved"), QString(tr("Saved file to %1").arg(filePath)));
        }
        else
        {
            QMessageBox::warning(this, tr("File NOT Saved"), QString(tr("Failed to save file %1").arg(filePath)));
        }
    }
}

void BackupSetDialog::loadBackupSet()
{
    TRACE_MSG("Enter BackupSetDialog::loadBackupSet", 10);
    QString defaultExtension = tr("XML files (*.xml)");
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", ui->configFileLineEdit->text(), tr("Text files (*.txt);;XML files (*.xml)"), &defaultExtension);
    if (!filePath.isEmpty())
    {
        ui->configFileLineEdit->setText(filePath);
        BackupSet backupSet;
        TRACE_MSG("Read backup set file", 10);
        if (backupSet.readFile(filePath))
        {
            TRACE_MSG("Done reading backup set file", 10);
            setBackupSet(backupSet);
            TRACE_MSG("Backup set configured", 10);
            //QMessageBox::information(this, tr("File Read"), QString(tr("Read file from, %1").arg(filePath)));
        }
        else
        {
            QMessageBox::warning(this, tr("File NOT Read"), QString(tr("Failed to read from %1").arg(filePath)));
        }
    }
    TRACE_MSG("Leaving loadBackupSet", 10);
}

void BackupSetDialog::currentFilterRowChanged ( const QModelIndex & current, const QModelIndex &)
{
  bool b = (current.row() >= 0);
  ui->copyFilterButton->setEnabled(b);
  ui->delFilterButton->setEnabled(b);
}

void BackupSetDialog::currentCriteriaRowChanged ( const QModelIndex & current, const QModelIndex &)
{
  bool b = (current.row() >= 0);
  ui->copyCriteriaButton->setEnabled(b);
  ui->deleteCriteriaButton->setEnabled(b);

}

