#include "logconfigdialog.h"
#include "logroutinginfotablemodel.h"
#include "linkbackfilterdelegate.h"
#include "checkboxonlydelegate.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableView>

LogConfigDialog::LogConfigDialog(QWidget *parent) :
  QDialog(parent), m_configFilePath(nullptr), m_logFilePath(nullptr), m_routingTableView(nullptr)
{
  buildDialog();
}

LogConfigDialog::~LogConfigDialog()
{
  QSettings settings;
  settings.setValue("LogConfigDialogGeometry", saveGeometry());
  settings.setValue("LogConfigDialogLastConfigPath", getConfigFilePath());
  //m_routingTableView
  // m_tableModel
  if (m_tableModel != nullptr && m_routingTableView != nullptr)
  {
    QString s;
    for (int i=0; i<LogRoutingInfoTableModel::numColumns; ++i)
    {
      if (s.length() > 0)
      {
        s = s + QString(",%1").arg(m_routingTableView->columnWidth(i));
      }
      else
      {
        s = QString("%1").arg(m_routingTableView->columnWidth(i));
      }
    }
    settings.setValue("LogConfigDialogRoutingColumnWidths", s);
  }
}

void LogConfigDialog::buildDialog()
{
  QPushButton* button;
  QVBoxLayout *vLayout;
  QHBoxLayout *hLayout;
  QFormLayout *fLayout = new QFormLayout;

  m_configFilePath = new QLineEdit();
  m_logFilePath = new QLineEdit();

  hLayout = new QHBoxLayout();

  vLayout = new QVBoxLayout();
  vLayout->addWidget(new QLabel(tr("Configuration File: ")));
  vLayout->addWidget(new QLabel(tr("Log File: ")));
  hLayout->addLayout(vLayout);

  vLayout = new QVBoxLayout();
  vLayout->addWidget(m_configFilePath);
  vLayout->addWidget(m_logFilePath);
  hLayout->addLayout(vLayout);

  vLayout = new QVBoxLayout();
  button = new QPushButton(tr("Load"));
  connect(button, SIGNAL(clicked()), this, SLOT(loadLogConfiguration()));
  vLayout->addWidget(button);
  button = new QPushButton(tr("Select"));
  connect(button, SIGNAL(clicked()), this, SLOT(selectLogFile()));
  vLayout->addWidget(button);
  hLayout->addLayout(vLayout);

  vLayout = new QVBoxLayout();
  button = new QPushButton(tr("Save"));
  connect(button, SIGNAL(clicked()), this, SLOT(saveLogConfiguration()));
  vLayout->addWidget(button);
  vLayout->addStretch();
  hLayout->addLayout(vLayout);

  fLayout->addRow(hLayout);

  hLayout = new QHBoxLayout();
  vLayout = new QVBoxLayout();
  vLayout->addWidget(new QLabel(tr("Routings:")));
  button = new QPushButton(tr("Edit"));
  connect(button, SIGNAL(clicked()), this, SLOT(editSelectedRouting()));
  vLayout->addWidget(button);
  button = new QPushButton(tr("Add"));
  connect(button, SIGNAL(clicked()), this, SLOT(addRouting()));
  vLayout->addWidget(button);
  button = new QPushButton(tr("Delete"));
  connect(button, SIGNAL(clicked()), this, SLOT(delSelectedRouting()));
  vLayout->addWidget(button);
  vLayout->addStretch();
  hLayout->addLayout(vLayout);

  m_routingTableView = new QTableView();
  m_tableModel = new LogRoutingInfoTableModel();
  m_routingTableView->setModel(m_tableModel);

  LinkBackFilterDelegate* delegate = new LinkBackFilterDelegate(m_routingTableView);
  m_routingTableView->setItemDelegate(delegate);

  CheckBoxOnlyDelegate * cboDelegate = new CheckBoxOnlyDelegate(m_routingTableView);
  m_routingTableView->setItemDelegateForColumn(LogRoutingInfoTableModel::enabledColumn, cboDelegate);
  m_routingTableView->setItemDelegateForColumn(LogRoutingInfoTableModel::routFileColumn, cboDelegate);
  m_routingTableView->setItemDelegateForColumn(LogRoutingInfoTableModel::routScreenColumn, cboDelegate);
  m_routingTableView->setItemDelegateForColumn(LogRoutingInfoTableModel::routDebugColumn, cboDelegate);

  hLayout->addWidget(m_routingTableView);
  fLayout->addRow(hLayout);


  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  fLayout->addRow(buttonBox);

  setLayout(fLayout);

  QSettings settings;
  restoreGeometry(settings.value("LogConfigDialogGeometry").toByteArray());
  setConfigFilePath(settings.value("LogConfigDialogLastConfigPath").toString());
  QString s = settings.value("LogConfigDialogRoutingColumnWidths").toString();
  if (s.length() > 0)
  {
    QStringList list = s.split(',');
    bool ok = true;
    for (int i=0; i<list.count() && i<LogRoutingInfoTableModel::numColumns; ++i)
    {
      int width = list[i].toInt(&ok);
      if (ok && width > 0)
      {
        m_routingTableView->setColumnWidth(i, width);
      }
    }
  }
}

void LogConfigDialog::configureDialog(const SimpleLoggerADP& logger)
{
  if (m_logFilePath != nullptr)
  {
    m_logFilePath->setText(logger.getFileName());
    if (m_tableModel != nullptr)
    {
      m_tableModel->setRoutings(logger.getRouting());
    }
  }
}

void LogConfigDialog::setConfigFilePath(const QString& path)
{
  if (m_configFilePath != nullptr)
  {
    m_configFilePath->setText(path);
  }
}

QString LogConfigDialog::getConfigFilePath() const
{
  return (m_configFilePath != nullptr) ? m_configFilePath->text() : "";
}

void LogConfigDialog::configureLogger(SimpleLoggerADP& logger)
{
  setConfigFilePath(logger.getFileName());
  // TODO:
}

void LogConfigDialog::editSelectedRouting()
{
  // TODO:
}

void LogConfigDialog::delSelectedRouting()
{

}

void LogConfigDialog::addRouting()
{
  // TODO:
}


void LogConfigDialog::loadLogConfiguration()
{
  SimpleLoggerADP logger;
  // TODO: read the logger
  configureDialog(logger);
}

void LogConfigDialog::saveLogConfiguration()
{
  SimpleLoggerADP logger;
  configureLogger(logger);
  // TODO: Save the logger
}

void LogConfigDialog::selectLogFile()
{
  // TODO: Select log file for logging to it.
}
