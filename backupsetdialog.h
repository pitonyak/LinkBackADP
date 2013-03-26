#ifndef BACKUPSETDIALOG_H
#define BACKUPSETDIALOG_H

#include <QDialog>

#include "backupset.h"
#include "filtertablemodel.h"
#include "criteriaforfilematchtablemodel.h"

namespace Ui {
class BackupSetDialog;
}

//**************************************************************************
/*! \class BackupSetDialog
 * \brief Provide a list view for a list of Filters.
 *
 * Represents the dialog used to edit the backup set. Filters and criteria are
 * used in the order in which they appear in the dialogs.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

class BackupSetDialog : public QDialog
{
    Q_OBJECT

public:
    //**************************************************************************
    //! Default Constructor
    /*!
     * \param [in] parent This is a QObject, so you can set a parent that will control the lifetime of this object.
     ***************************************************************************/
    explicit BackupSetDialog(QWidget *parent = 0);

    //**************************************************************************
    //! Copy Constructor
    /*!
     * \param [in] backupSet Object to copy into this object.
     * \param [in] parent This is a QObject, so you can set a parent that will control the lifetime of this object.
     ***************************************************************************/
    explicit BackupSetDialog(const BackupSet& backupSet, QWidget *parent = 0);

    /*! \brief Desctructor */
    ~BackupSetDialog();

    //**************************************************************************
    /*! \brief Get a backup set object representing the current state of the dialog.
     *
     *  A new backup set object is created and returned.
     *  \return Object representing the current state of the dialog.
     ***************************************************************************/
    BackupSet getBackupSet() const;

    //**************************************************************************
    /*! \brief Configure the dialog to use the specified backup set.
     *
     *  Each field in the dialog is set based on the backup set parameter.
     *  \param [in] backupSet object used to configure the dialog.
     *  \return Flags that apply to the item specified by the index.
     ***************************************************************************/
    void setBackupSet(const BackupSet& backupSet);

    /*! \brief Get the full path to where this object was last written or read. */
    QString getConfigFilePath() const;

    /*! \brief Set the full path to where this object was last written or read. */
    void setConfigFilePath(const QString& path);

private:
    /*! \brief Set list columns, delegates, and initial values. */
    void initialize();

    /*! \brief Get the index of the current filter row. */
    int getSelectedFilterRow() const;

    /*! \brief Enable/disable buttons based on dialog values. */
    void enableButtons();

    /*! \brief Return True if a filter row is currently selected. */
    bool isFilterSelected() const;

    /*! \brief Return True if a criteria row is currently selected. */
    bool isCriteriaSelected() const;

public slots:
    /*! \brief Insert a new copy of the currently selected filter at the current location. */
    void copyFilter();

    /*! \brief Insert a new "default" filter at the current location. */
    void insertFilter();

    /*! \brief Delete the currently selected filter. */
    void delFilter();

    /*! \brief Move the currently selected filter up one row (if possible). */
    void upFilter();

    /*! \brief Move the currently selected filter down one row (if possible). */
    void downFilter();

    /*! \brief Insert a new copy of the currently selected criteria at the current location. */
    void copyCriteria();

    /*! \brief Insert a new "default" criteria at the current location. */
    void insertCriteria();

    /*! \brief Delete the currently selected criteria. */
    void delCriteria();

    /*! \brief Move the currently selected criteria up one row (if possible). */
    void upCriteria();

    /*! \brief Move the currently selected criteria down one row (if possible). */
    void downCriteria();

    /*! \brief Open a dialog and choose the "from" back-up directory. */
    void selectFromRoot();

    /*! \brief Open a dialog and choose the "to" back-up directory. */
    void selectToRoot();

    /*! \brief Currently does nothing */
    void selectHashMethod(const QString& method);

    /*! \brief Open a dialog to choose where to save the XML file, then write this object as an XML file to disk. */
    void saveBackupSet();

    /*! \brief Open a dialog and ask from where to load a backup set.
     *
     *  The user selects the file to load. The backup set object is used to read
     *  the file. A call to setBackupSet is used to set the values in the dialog.
     */
    void loadBackupSet();

    /*! \brief Enables and disables buttons such as "up" and "down" */
    void currentFilterRowChanged ( const QModelIndex & current, const QModelIndex &    previous );

    /*! \brief Enables and disables buttons such as "up" and "down" */
    void currentCriteriaRowChanged ( const QModelIndex & current, const QModelIndex &    previous );

private:
    Ui::BackupSetDialog *ui;
    FilterTableModel m_filterTableModel;
    CriteriaForFileMatchTableModel m_criteriaForFileMatchTableModel;

};

#endif // BACKUPSETDIALOG_H
