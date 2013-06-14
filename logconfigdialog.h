#ifndef LOGCONFIGDIALOG_H
#define LOGCONFIGDIALOG_H

#include <QDialog>
#include "simpleloggeradp.h"

//**************************************************************************
/*! \class LogConfigDialog
 *  \brief Configure
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 *
 **************************************************************************/

class QLineEdit;
class QTableView;
class LogRoutingInfoTableModel;

class LogConfigDialog : public QDialog
{
  Q_OBJECT
public:
  explicit LogConfigDialog(QWidget *parent = 0);
  ~LogConfigDialog();

  void configureDialog(const SimpleLoggerADP& logger);
  void configureLogger(SimpleLoggerADP& logger);

  void setConfigFilePath(const QString& path);
  QString getConfigFilePath() const;

signals:
  
public slots:
  void editSelectedRouting();
  void delSelectedRouting();
  void addRouting();
  void loadLogConfiguration();
  void saveLogConfiguration();
  void selectLogFile();

private:
  void buildDialog();

  QLineEdit* m_configFilePath;
  QLineEdit* m_logFilePath;
  QTableView* m_routingTableView;
  LogRoutingInfoTableModel* m_tableModel;
};

#endif // LOGCONFIGDIALOG_H
