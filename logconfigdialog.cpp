#include "logconfigdialog.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

LogConfigDialog::LogConfigDialog(QWidget *parent) :
  QDialog(parent), m_configFilePath(nullptr), m_logFilePath(nullptr)
{
  buildDialog();
}

LogConfigDialog::~LogConfigDialog()
{
  QSettings settings;
  settings.setValue("LogConfigDialogGeometry", saveGeometry());
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
  vLayout->addWidget(button);
  button = new QPushButton(tr("Select"));
  vLayout->addWidget(button);
  hLayout->addLayout(vLayout);

  vLayout = new QVBoxLayout();
  button = new QPushButton(tr("Save"));
  vLayout->addWidget(button);
  vLayout->addStretch();
  hLayout->addLayout(vLayout);

  fLayout->addRow(hLayout);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  fLayout->addRow(buttonBox);

  setLayout(fLayout);

  QSettings settings;
  restoreGeometry(settings.value("LogConfigDialogGeometry").toByteArray());
}

void LogConfigDialog::configureDialog(const SimpleLoggerADP& logger)
{

}

void LogConfigDialog::configureLogger(SimpleLoggerADP& logger)
{
}

void LogConfigDialog::editSelectedRouting()
{

}

void LogConfigDialog::loadLogConfiguration()
{

}

void LogConfigDialog::saveLogConfiguration()
{

}

void LogConfigDialog::selectLogFile()
{

}
