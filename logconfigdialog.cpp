#include "logconfigdialog.h"

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
  QDialog(parent), m_configFilePath(nullptr), m_logFilePath(nullptr), m_Components(nullptr)
{
  buildDialog();
}

LogConfigDialog::~LogConfigDialog()
{
  QSettings settings;
  settings.setValue("LogConfigDialogGeometry", saveGeometry());
  settings.setValue("LogConfigDialogLastConfigPath", getConfigFilePath());
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

  m_Components = new QTableView();
  hLayout->addWidget(m_Components);
  fLayout->addRow(hLayout);


  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  fLayout->addRow(buttonBox);

  setLayout(fLayout);

  QSettings settings;
  restoreGeometry(settings.value("LogConfigDialogGeometry").toByteArray());
  setConfigFilePath(settings.value("LogConfigDialogLastConfigPath").toString());
}

void LogConfigDialog::configureDialog(const SimpleLoggerADP& logger)
{
  if (m_logFilePath != nullptr)
  {
    m_logFilePath->setText(logger.getFileName());
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
}

void LogConfigDialog::editSelectedRouting()
{

}

void LogConfigDialog::delSelectedRouting()
{

}

void LogConfigDialog::addRouting()
{

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
