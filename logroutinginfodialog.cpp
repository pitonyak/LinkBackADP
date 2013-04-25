#include "logroutinginfodialog.h"
#include "ui_logroutinginfodialog.h"

LogRoutingInfoDialog::LogRoutingInfoDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LogRoutingInfoDialog)
{
  ui->setupUi(this);
}

LogRoutingInfoDialog::~LogRoutingInfoDialog()
{
  delete ui;
}
