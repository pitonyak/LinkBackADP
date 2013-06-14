#include "logroutinginfodialog.h"
#include "ui_logroutinginfodialog.h"
#include "linkbackfilterdelegate.h"

#include <QMessageBox>

LogRoutingInfoDialog::LogRoutingInfoDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LogRoutingInfoDialog)
{
  initialize();
}


LogRoutingInfoDialog::LogRoutingInfoDialog(const SimpleLoggerRoutingInfo& routingInfo, QWidget *parent) :
  QDialog(parent), ui(new Ui::LogRoutingInfoDialog)
{
  m_messageComponentTableModel.setMessageComponents(routingInfo.getMessageComponents());
  initialize();
  setRoutingInfo(routingInfo);
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

  info.setCategoryLevel(SimpleLoggerRoutingInfo::TraceMessage, ui->traceSpinBox->value());
  info.setCategoryLevel(SimpleLoggerRoutingInfo::DebugMessage, ui->debugSpinBox->value());
  info.setCategoryLevel(SimpleLoggerRoutingInfo::InformationMessage, ui->infoSpinBox->value());
  info.setCategoryLevel(SimpleLoggerRoutingInfo::WarningMessage, ui->warnSpinBox->value());
  info.setCategoryLevel(SimpleLoggerRoutingInfo::ErrorMessage, ui->errorSpinBox->value());
  info.setCategoryLevel(SimpleLoggerRoutingInfo::UserMessage, ui->userSpinBox->value());

  info.setLocationRegExp(ui->locRegExpEdit->displayText());
  info.setMessageRegExp(ui->msgRegExpEdit->displayText());

  // TODO: Get regular expression case sensitivity and pattern type.

  for (int i=0; i<m_messageComponentTableModel.rowCount(); ++i)
  {
    const QPair<SimpleLoggerRoutingInfo::MessageComponent, QString> & pair = m_messageComponentTableModel.getMessageComponents()[i];
    info.addMessageFormat(pair.first, pair.second);
  }

  return info;
}

void LogRoutingInfoDialog::setRoutingInfo(const SimpleLoggerRoutingInfo &routingInfo)
{
  ui->nameEdit->setText(routingInfo.getName());
  ui->locRegExpEdit->setText(routingInfo.getLocationRegExpString());
  ui->msgRegExpEdit->setText(routingInfo.getMessageRegExpString());
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

  // TODO: Set regular expression states

}

void LogRoutingInfoDialog::initialize()
{
  ui->setupUi(this);
  ui->componentTableView->setModel(&m_messageComponentTableModel);
  LinkBackFilterDelegate* delegate = new LinkBackFilterDelegate(ui->componentTableView);
  ui->componentTableView->setItemDelegate(delegate);
  ui->componentTableView->setColumnWidth(m_messageComponentTableModel.fieldColumn, 240);
  ui->componentTableView->setColumnWidth(m_messageComponentTableModel.stringColumn, 240);

  connect(ui->componentTableView->selectionModel(), SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)), this, SLOT(currentMessageCategoryRowChanged ( const QModelIndex &, const QModelIndex &)));
  connect(ui->copyCompButton, SIGNAL(clicked(bool)), this, SLOT( copyMessageCategory()));
  connect(ui->insertCompButton, SIGNAL(clicked(bool)), this, SLOT(insertMessageCategory()));
  connect(ui->upCompButton, SIGNAL(clicked(bool)), this, SLOT(upMessageCategory()));
  connect(ui->downCompButton, SIGNAL(clicked(bool)), this, SLOT(downMessageCategory()));
  connect(ui->delCompButton, SIGNAL(clicked(bool)), this, SLOT(delMessageCategory()));

  connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(closeRequested()));
  connect(ui->testButton, SIGNAL(clicked(bool)), this, SLOT(testMessage()));

  ui->locCaseSensitive->addItem(tr("Case Sensitive"), Qt::CaseSensitive);
  ui->locCaseSensitive->addItem(tr("Case Insensitive"), Qt::CaseInsensitive);
  ui->locCaseSensitive->setToolTip(tr("Set location regular expression case sensitivity."));

  ui->msgCaseSensitive->addItem(tr("Case Sensitive"), Qt::CaseSensitive);
  ui->msgCaseSensitive->addItem(tr("Case Insensitive"), Qt::CaseInsensitive);
  ui->msgCaseSensitive->setToolTip(tr("Set location regular expression case sensitivity."));

  ui->locSyntax->addItem(tr("Perl No Greedy"), QRegExp::RegExp);
  ui->locSyntax->addItem(tr("Perl Greedy"), QRegExp::RegExp2);
  ui->locSyntax->addItem(tr("Wildcard"), QRegExp::Wildcard);
  ui->locSyntax->addItem(tr("Wildcard Unix"), QRegExp::WildcardUnix);
  ui->locSyntax->addItem(tr("String"), QRegExp::FixedString);
  ui->locSyntax->addItem(tr("W3C XML"), QRegExp::W3CXmlSchema11);
  ui->locSyntax->setToolTip(tr("Set location regular expression pattern syntax."));

  ui->msgSyntax->addItem(tr("Perl No Greedy"), QRegExp::RegExp);
  ui->msgSyntax->addItem(tr("Perl Greedy"), QRegExp::RegExp2);
  ui->msgSyntax->addItem(tr("Wildcard"), QRegExp::Wildcard);
  ui->msgSyntax->addItem(tr("Wildcard Unix"), QRegExp::WildcardUnix);
  ui->msgSyntax->addItem(tr("String"), QRegExp::FixedString);
  ui->msgSyntax->addItem(tr("W3C XML"), QRegExp::W3CXmlSchema11);
  ui->locSyntax->setToolTip(tr("Set unformatted message regular expression pattern syntax."));

  ui->locRegExpEdit->setToolTip(tr("Match location filename:line_number must match the regular expression; use to filter by filename or line number."));
  ui->msgRegExpEdit->setToolTip(tr("Match based on the unformatted message text."));

  ui->nameEdit->setToolTip(tr("User recognizable name for this object to pick it out in a list."));

  enableButtons();
}

int LogRoutingInfoDialog::getSelectedCatRow() const
{
  return ui->componentTableView->currentIndex().row();
}

void LogRoutingInfoDialog::enableButtons()
{
  int count = m_messageComponentTableModel.messageComponentCount();
  int row = count > 0 ? getSelectedCatRow() : -1;
  bool b = count > 0 && row >= 0;
  ui->copyCompButton->setEnabled(b);
  ui->delCompButton->setEnabled(b);
  //ui->upCompButton->setEnabled(b && row > 0);
  //ui->downCompButton->setEnabled(b && row < (count - 1));
}

bool LogRoutingInfoDialog::isCatSelected() const
{
  return m_messageComponentTableModel.messageComponentCount() > 0 && ui->componentTableView->currentIndex().row() >= 0;
}

void LogRoutingInfoDialog::copyMessageCategory()
{
  m_messageComponentTableModel.copyMessageComponent(ui->componentTableView->currentIndex().row());
}

void LogRoutingInfoDialog::insertMessageCategory()
{
  const QPair<SimpleLoggerRoutingInfo::MessageComponent, QString> component(SimpleLoggerRoutingInfo::MessageText, "");
  m_messageComponentTableModel.insertMessageComponent(ui->componentTableView->currentIndex().row(), component);
}

void LogRoutingInfoDialog::delMessageCategory()
{
  m_messageComponentTableModel.removeMessageComponent(ui->componentTableView->currentIndex().row());
}

void LogRoutingInfoDialog::upMessageCategory()
{
  m_messageComponentTableModel.moveMessageComponentUp(ui->componentTableView->currentIndex().row());
}

void LogRoutingInfoDialog::downMessageCategory()
{
  m_messageComponentTableModel.moveMessageComponentDown(ui->componentTableView->currentIndex().row());
}

void LogRoutingInfoDialog::currentMessageCategoryRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
  // Avoid a compiler warning
  (void)previous;

  int row = current.row();
  bool b = (row >= 0);
  ui->copyCompButton->setEnabled(b);
  ui->delCompButton->setEnabled(b);

  //int count = m_messageComponentTableModel.messageComponentCount();
  //ui->upCompButton->setEnabled(b && row > 0);
  //ui->downCompButton->setEnabled(b && row < (count - 1));
}

void LogRoutingInfoDialog::closeRequested()
{
  // TODO: Validate
  QString s = ui->locRegExpEdit->displayText();
  if (!s.isEmpty())
  {
    QRegExp regExp(s);
    if (!regExp.isValid())
    {
      QPalette pal = ui->locRegExpEdit->palette();
      pal.setColor(QPalette::Text, Qt::red);
      ui->locRegExpEdit->setPalette(pal);

      QMessageBox::warning(this, tr("ERROR"), tr("Invalid Location Regular Expression"));
      return;
    }
  }
  ui->locRegExpEdit->setBackgroundRole(QPalette::NoRole);

  s = ui->msgRegExpEdit->displayText();
  if (!s.isEmpty())
  {
    QRegExp regExp(s);
    if (!regExp.isValid())
    {
      QPalette pal = ui->msgRegExpEdit->palette();
      pal.setColor(QPalette::Text, Qt::red);
      ui->msgRegExpEdit->setPalette(pal);

      QMessageBox::warning(this, tr("ERROR"), tr("Invalid Location Regular Expression"));
      return;
    }
  }
  ui->msgRegExpEdit->setBackgroundRole(QPalette::NoRole);

  accept();
}

QString LogRoutingInfoDialog::isValid() const
{
  QString errorString = "";
  QString s = ui->locRegExpEdit->displayText();
  if (!s.isEmpty())
  {
    QRegExp regExp(s);
    if (!regExp.isValid())
    {
      errorString = tr("Invalid location Regular Expression.");
    }
  }

  s = ui->msgRegExpEdit->displayText();
  if (!s.isEmpty())
  {
    QRegExp regExp(s);
    if (!regExp.isValid())
    {
      if (errorString.length() > 0)
      {
        errorString = errorString + " ";
      }
      errorString = errorString + tr("Invalid message Regular Expression.");
    }
  }

  return errorString;
}

void LogRoutingInfoDialog::testMessage()
{
  QString s = isValid();
  if (s.isEmpty())
  {
    // TODO: Generate a test message!
    SimpleLoggerRoutingInfo info = getRoutingInfo();
    s = info.formatMessage(tr("Test log message"), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), SimpleLoggerRoutingInfo::ErrorMessage, 1);
    // TODO: Add more test messages

//#define ERROR_MSG(msg, level) errorMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
//#define WARN_MSG( msg, level) warnMessage((msg),  QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
//#define INFO_MSG( msg, level) infoMessage((msg),  QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
//#define TRACE_MSG(msg, level) traceMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
//#define DEBUG_MSG(msg, level) debugMessage((msg), QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));
//#define USER_MSG( msg, level) userMessage((msg),  QString(QObject::tr("%1:%2")).arg(__FILE__, QString::number(__LINE__)), QDateTime::currentDateTime(), (level));

  }
  ui->sampleTextEdit->setText(s);
}
