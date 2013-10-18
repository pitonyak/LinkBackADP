#include "logconfigdialog.h"
#include "logroutinginfotablemodel.h"
#include "linkbackfilterdelegate.h"
#include "checkboxonlydelegate.h"
#include "logroutinginfodialog.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableView>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>

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
  setWindowTitle(tr("Configure Logging"));
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
  button = new QPushButton(tr("Copy"));
  connect(button, SIGNAL(clicked()), this, SLOT(copyRouting()));
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

void LogConfigDialog::setLogFilePath(const QString& path)
{
  if (m_logFilePath != nullptr)
  {
    m_logFilePath->setText(path);
  }
}

QString LogConfigDialog::getLogFilePath() const
{
  return (m_logFilePath != nullptr) ? m_logFilePath->text() : "";
}

bool LogConfigDialog::isRoutingSelected() const
{
  return m_tableModel != nullptr && m_routingTableView != nullptr && m_tableModel->count() > 0 && m_routingTableView->currentIndex().row() >= 0;
}

int LogConfigDialog::getSelectedRouting() const
{
  return m_routingTableView->currentIndex().row();
}

void LogConfigDialog::configureLogger(SimpleLoggerADP& logger)
{
  if (m_logFilePath != nullptr)
  {
    logger.setFileName(m_logFilePath->text());
  }
  else
  {
    logger.setFileName("");
  }
  if (m_tableModel != nullptr)
  {
    logger.clearRouting();
    logger.addRouting(m_tableModel->getRoutings());
  }
}

void LogConfigDialog::editSelectedRouting()
{
  if (isRoutingSelected())
  {
    LogRoutingInfoDialog dlg(m_tableModel->getRoutings().at(getSelectedRouting()));
    if (dlg.exec() == QDialog::Accepted)
    {
      m_tableModel->updateRouting(getSelectedRouting(), dlg.getRoutingInfo());
    }
  }
}

void LogConfigDialog::delSelectedRouting()
{
  if (isRoutingSelected())
  {
    m_tableModel->removeRouting(getSelectedRouting());
  }
}

void LogConfigDialog::copyRouting()
{
  m_tableModel->copyRouting(getSelectedRouting());
}

void LogConfigDialog::addRouting()
{
  SimpleLoggerRoutingInfo info;
  m_tableModel->insertRouting(getSelectedRouting(), info);
}

void LogConfigDialog::loadLogConfiguration()
{
  QString fileExtension;
  QString s = QFileDialog::getOpenFileName(this, tr("Select Log Configuration File To Open"), getConfigFilePath(), tr("XML(*.xml);;Text(*.txt);;All(*.*)"), &fileExtension, QFileDialog::DontUseNativeDialog);
  if (s.length() > 0)
  {
    setConfigFilePath(s);
    QFile file(s);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QMessageBox::warning(this, tr("ERROR"), tr("Failed to open file for reading."));
    }
    else
    {
      QXmlStreamReader reader(&file);
      if (reader.hasError())
      {
        QMessageBox::warning(this, tr("ERROR"), tr("Failed to create output stream for reading logger."));
      }
      else
      {
        SimpleLoggerADP logger;
        logger.read(reader);
        configureDialog(logger);
      }
      file.close();
    }
  }
}

void LogConfigDialog::saveLogConfiguration()
{
  QString fileExtension;
  QString s = QFileDialog::getSaveFileName(this, tr("Save To"), getConfigFilePath(), tr("XML(*.xml);;Text(*.txt);;All(*.*)"), &fileExtension, QFileDialog::DontUseNativeDialog);
  if (s.length() > 0)
  {
    if (!s.contains('.'))
    {
      if (fileExtension.compare(tr("XML(*.xml)"), Qt::CaseInsensitive) == 0)
      {
        s = s + ".xml";
      }
      else if (fileExtension.compare(tr("Text(*.txt)"), Qt::CaseInsensitive) == 0)
      {
        s = s + ".txt";
      }
    }
    setConfigFilePath(s);
    QFile file(s);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QMessageBox::warning(this, tr("ERROR"), tr("Failed to open file for writing."));
    }
    else
    {
      QXmlStreamWriter writer(&file);
      if (writer.hasError())
      {
        QMessageBox::warning(this, tr("ERROR"), tr("Failed to create output stream for writing logger."));
      }
      else
      {
        writer.setAutoFormatting(true);
        SimpleLoggerADP logger;
        configureLogger(logger);
        logger.write(writer);
      }
      file.close();
    }
  }
}

void LogConfigDialog::selectLogFile()
{
  QString fileExtension;
  QString s = QFileDialog::getSaveFileName(this, tr("Write Log To"), getLogFilePath(), tr("Log(*.log);;Text(*.txt);;All(*.*)"), &fileExtension, QFileDialog::DontUseNativeDialog);
  if (s.length() > 0)
  {
    if (!s.contains('.'))
    {
      if (fileExtension.compare(tr("Log(*.log)"), Qt::CaseInsensitive) == 0)
      {
        s = s + ".log";
      }
      else if (fileExtension.compare(tr("Text(*.txt)"), Qt::CaseInsensitive) == 0)
      {
        s = s + ".txt";
      }
      setLogFilePath(s);
    }
  }
}
