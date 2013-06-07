#ifndef LOGROUTINGINFODIALOG_H
#define LOGROUTINGINFODIALOG_H

#include <QDialog>

#include "simpleloggerroutinginfo.h"
#include "messagecomponenttablemodel.h"


//**************************************************************************
/*! \class LogRoutingInfoDialog
 * \brief Edit a single SimpleLoggerRoutingInfo object.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

namespace Ui {
class LogRoutingInfoDialog;
}

class LogRoutingInfoDialog : public QDialog
{
  Q_OBJECT
  
public:
  //**************************************************************************
  //! Default Constructor
  /*!
   * \param [in] parent This is a QObject, so you can set a parent that will control the lifetime of this object.
   ***************************************************************************/
  explicit LogRoutingInfoDialog(QWidget *parent = 0);

  //**************************************************************************
  //! Set the object to edit
  /*!
   * \param [in] routingInfo Object to copy into this object.
   * \param [in] parent This is a QObject, so you can set a parent that will control the lifetime of this object.
   ***************************************************************************/
  explicit LogRoutingInfoDialog(const SimpleLoggerRoutingInfo& routingInfo, QWidget *parent = 0);

  /*! \brief Desctructor */
  ~LogRoutingInfoDialog();

  //**************************************************************************
  /*! \brief Get an object representing the current state of the dialog.
   *
   *  \return Object representing the current state of the dialog.
   ***************************************************************************/
  SimpleLoggerRoutingInfo getRoutingInfo() const;

  //**************************************************************************
  /*! \brief Configure the dialog to use the specified routing info.
   *
   *  \param [in] routingInfo object used to configure the dialog.
   *  \return Flags that apply to the item specified by the index.
   ***************************************************************************/
  void setRoutingInfo(const SimpleLoggerRoutingInfo& routingInfo);

private:
  /*! \brief Set list columns, delegates, and initial values. */
  void initialize();

  /*! \brief Get the index of the current row. */
  int getSelectedCatRow() const;

  /*! \brief Enable/disable buttons based on dialog values. */
  void enableButtons();

  /*! \brief Return True if a category row is currently selected. */
  bool isCatSelected() const;

public slots:
  /*! \brief Insert a new copy of the currently selected MessageCategory at the current location. */
  void copyMessageCategory();

  /*! \brief Insert a new "default" MessageCategory at the current location. */
  void insertMessageCategory();

  /*! \brief Delete the currently selected MessageCategory. */
  void delMessageCategory();

  /*! \brief Move the currently selected MessageCategory up one row (if possible). */
  void upMessageCategory();

  /*! \brief Move the currently selected MessageCategory down one row (if possible). */
  void downMessageCategory();

  /*! \brief Enables and disables buttons such as "up" and "down" */
  void currentMessageCategoryRowChanged ( const QModelIndex & current, const QModelIndex &    previous );

  void closeRequested();

  void testMessage();

private:
  Ui::LogRoutingInfoDialog *ui;
  MessageComponentTableModel m_messageComponentTableModel;
};

#endif // LOGROUTINGINFODIALOG_H
