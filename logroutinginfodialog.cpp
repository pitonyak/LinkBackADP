#include "logroutinginfodialog.h"
#include "ui_logroutinginfodialog.h"

LogRoutingInfoDialog::LogRoutingInfoDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LogRoutingInfoDialog)
{
  initialize();
}

LogRoutingInfoDialog::~LogRoutingInfoDialog()
{
  delete ui;
}

SimpleLoggerRoutingInfo LogRoutingInfoDialog::getRoutingInfo() const
{
  SimpleLoggerRoutingInfo info;
  info.setName(ui->nameEdit->text());
  info.setEnabled(ui->enabledCheckBox->checkState() == Qt::Checked ? true : false);
  info.setRouting(SimpleLoggerRoutingInfo::RouteEmit, ui->enableScreenCheckBox->checkState() == Qt::Checked ? true : false);
  info.setRouting(SimpleLoggerRoutingInfo::RouteQDebug, ui->enableQDebugBox->checkState() == Qt::Checked ? true : false);
  info.setRouting(SimpleLoggerRoutingInfo::RouteFile, ui->enableFileCheckBox->checkState() == Qt::Checked ? true : false);
  // TODO:
  return info;
}

void LogRoutingInfoDialog::setRoutingInfo(const SimpleLoggerRoutingInfo &routingInfo)
{
  // TODO:
  ui->nameEdit->setText(routingInfo.getName());
  ui->regExpEdit->setText(routingInfo.getRegExpString());
  ui->enabledCheckBox->setCheckState(routingInfo.isEnabled() ? Qt::Checked : Qt::Unchecked);
  ui->enableScreenCheckBox->setCheckState(routingInfo.isRoutingOn(SimpleLoggerRoutingInfo::RouteEmit) ? Qt::Checked : Qt::Unchecked);
  ui->enableQDebugBox->setCheckState(routingInfo.isRoutingOn(SimpleLoggerRoutingInfo::RouteQDebug) ? Qt::Checked : Qt::Unchecked);
  ui->enableFileCheckBox->setCheckState(routingInfo.isRoutingOn(SimpleLoggerRoutingInfo::RouteFile) ? Qt::Checked : Qt::Unchecked);

  ui->traceSpinBox->setValue(routingInfo.getCategoryLevel(SimpleLoggerRoutingInfo::TraceMessage));
  ui->debugSpinBox->setValue(routingInfo.getCategoryLevel(SimpleLoggerRoutingInfo::DebugMessage));
  ui->infoSpinBox->setValue(routingInfo.getCategoryLevel(SimpleLoggerRoutingInfo::InformationMessage));
  ui->warnSpinBox->setValue(routingInfo.getCategoryLevel(SimpleLoggerRoutingInfo::WarningMessage));
  ui->errorSpinBox->setValue(routingInfo.getCategoryLevel(SimpleLoggerRoutingInfo::ErrorMessage));
  ui->userSpinBox->setValue(routingInfo.getCategoryLevel(SimpleLoggerRoutingInfo::UserMessage));

}

void LogRoutingInfoDialog::initialize()
{
  ui->setupUi(this);
  /**
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

  for (const QCryptographicHash::Algorithm algorithm : EnhancedQCryptographicHash::getAlgorithmList())
  {
      ui->hashComboBox->addItem(EnhancedQCryptographicHash::toAlgorithmString(algorithm));
  }

  ui->priorityComboBox->addItems(BackupSet::getAllPriorities());

  // To enable and disable buttons on selection change.
  connect(ui->filtersTableView->selectionModel(), SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)), this, SLOT(currentFilterRowChanged ( const QModelIndex &, const QModelIndex &)));
  connect(ui->criteriaTableView->selectionModel(), SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)), this, SLOT(currentCriteriaRowChanged ( const QModelIndex &, const QModelIndex &)));
  enableButtons();
  **/
}

int LogRoutingInfoDialog::getSelectedCatRow() const
{
  // TODO:
  return -1;
}

void LogRoutingInfoDialog::enableButtons()
{
  // TODO:
}

bool LogRoutingInfoDialog::isCatSelected() const
{
  // TODO:
  return false;
}

void LogRoutingInfoDialog::copyMessageCategory()
{
  // TODO:
}

void LogRoutingInfoDialog::insertMessageCategory()
{
  // TODO:
}

void LogRoutingInfoDialog::delMessageCategory()
{
  // TODO:
}

void LogRoutingInfoDialog::upMessageCategory()
{
  // TODO:
}

void LogRoutingInfoDialog::downMessageCategory()
{
  // TODO:
}

void LogRoutingInfoDialog::currentMessageCategoryRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
  // TODO:
}

void LogRoutingInfoDialog::currentCriteriaRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
  // TODO:
}
