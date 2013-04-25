#ifndef LOGROUTINGINFODIALOG_H
#define LOGROUTINGINFODIALOG_H

#include <QDialog>

namespace Ui {
class LogRoutingInfoDialog;
}

class LogRoutingInfoDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit LogRoutingInfoDialog(QWidget *parent = 0);
  ~LogRoutingInfoDialog();
  
private:
  Ui::LogRoutingInfoDialog *ui;
};

#endif // LOGROUTINGINFODIALOG_H
