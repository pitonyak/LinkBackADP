/********************************************************************************
** Form generated from reading UI file 'backupsetdialog.ui'
**
** Created: Mon Apr 16 10:26:35 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BACKUPSETDIALOG_H
#define UI_BACKUPSETDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableView>

QT_BEGIN_NAMESPACE

class Ui_BackupSetDialog
{
public:
    QLineEdit *fromRootLineEdit;
    QLineEdit *toRootLineEdit;
    QLabel *label_3;
    QLabel *label_6;
    QPushButton *insertFilterButton;
    QPushButton *downFilterButton;
    QPushButton *upFilterButton;
    QPushButton *delFilterButton;
    QPushButton *insertCriteriaButton;
    QPushButton *upCriteriaButton;
    QPushButton *deleteCriteriaButton;
    QPushButton *downCriteriaButton;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QTableView *filtersTableView;
    QTableView *criteriaTableView;
    QPushButton *fromButton;
    QPushButton *toButton;
    QComboBox *hashComboBox;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QLineEdit *configFileLineEdit;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *copyFilterButton;
    QPushButton *copyCriteriaButton;

    void setupUi(QDialog *BackupSetDialog)
    {
        if (BackupSetDialog->objectName().isEmpty())
            BackupSetDialog->setObjectName(QString::fromUtf8("BackupSetDialog"));
        BackupSetDialog->resize(1295, 692);
        fromRootLineEdit = new QLineEdit(BackupSetDialog);
        fromRootLineEdit->setObjectName(QString::fromUtf8("fromRootLineEdit"));
        fromRootLineEdit->setGeometry(QRect(100, 100, 1061, 27));
        toRootLineEdit = new QLineEdit(BackupSetDialog);
        toRootLineEdit->setObjectName(QString::fromUtf8("toRootLineEdit"));
        toRootLineEdit->setGeometry(QRect(100, 150, 1061, 27));
        label_3 = new QLabel(BackupSetDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 190, 51, 17));
        label_6 = new QLabel(BackupSetDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 490, 61, 17));
        insertFilterButton = new QPushButton(BackupSetDialog);
        insertFilterButton->setObjectName(QString::fromUtf8("insertFilterButton"));
        insertFilterButton->setGeometry(QRect(20, 250, 61, 27));
        downFilterButton = new QPushButton(BackupSetDialog);
        downFilterButton->setObjectName(QString::fromUtf8("downFilterButton"));
        downFilterButton->setGeometry(QRect(20, 340, 61, 27));
        upFilterButton = new QPushButton(BackupSetDialog);
        upFilterButton->setObjectName(QString::fromUtf8("upFilterButton"));
        upFilterButton->setGeometry(QRect(20, 310, 61, 27));
        delFilterButton = new QPushButton(BackupSetDialog);
        delFilterButton->setObjectName(QString::fromUtf8("delFilterButton"));
        delFilterButton->setGeometry(QRect(20, 280, 61, 27));
        insertCriteriaButton = new QPushButton(BackupSetDialog);
        insertCriteriaButton->setObjectName(QString::fromUtf8("insertCriteriaButton"));
        insertCriteriaButton->setGeometry(QRect(20, 550, 61, 27));
        upCriteriaButton = new QPushButton(BackupSetDialog);
        upCriteriaButton->setObjectName(QString::fromUtf8("upCriteriaButton"));
        upCriteriaButton->setGeometry(QRect(20, 610, 61, 27));
        deleteCriteriaButton = new QPushButton(BackupSetDialog);
        deleteCriteriaButton->setObjectName(QString::fromUtf8("deleteCriteriaButton"));
        deleteCriteriaButton->setGeometry(QRect(20, 580, 61, 27));
        downCriteriaButton = new QPushButton(BackupSetDialog);
        downCriteriaButton->setObjectName(QString::fromUtf8("downCriteriaButton"));
        downCriteriaButton->setGeometry(QRect(20, 640, 61, 27));
        cancelButton = new QPushButton(BackupSetDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(1190, 70, 85, 27));
        okButton = new QPushButton(BackupSetDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(1190, 30, 85, 27));
        filtersTableView = new QTableView(BackupSetDialog);
        filtersTableView->setObjectName(QString::fromUtf8("filtersTableView"));
        filtersTableView->setGeometry(QRect(100, 190, 1181, 301));
        filtersTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        filtersTableView->horizontalHeader()->setDefaultSectionSize(200);
        criteriaTableView = new QTableView(BackupSetDialog);
        criteriaTableView->setObjectName(QString::fromUtf8("criteriaTableView"));
        criteriaTableView->setGeometry(QRect(100, 510, 1181, 161));
        criteriaTableView->horizontalHeader()->setDefaultSectionSize(150);
        fromButton = new QPushButton(BackupSetDialog);
        fromButton->setObjectName(QString::fromUtf8("fromButton"));
        fromButton->setGeometry(QRect(10, 100, 71, 27));
        toButton = new QPushButton(BackupSetDialog);
        toButton->setObjectName(QString::fromUtf8("toButton"));
        toButton->setGeometry(QRect(10, 150, 71, 27));
        hashComboBox = new QComboBox(BackupSetDialog);
        hashComboBox->setObjectName(QString::fromUtf8("hashComboBox"));
        hashComboBox->setGeometry(QRect(100, 20, 171, 27));
        saveButton = new QPushButton(BackupSetDialog);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(1190, 150, 85, 27));
        loadButton = new QPushButton(BackupSetDialog);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));
        loadButton->setGeometry(QRect(1190, 110, 85, 27));
        configFileLineEdit = new QLineEdit(BackupSetDialog);
        configFileLineEdit->setObjectName(QString::fromUtf8("configFileLineEdit"));
        configFileLineEdit->setGeometry(QRect(100, 60, 1061, 27));
        label_4 = new QLabel(BackupSetDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 60, 71, 17));
        label_5 = new QLabel(BackupSetDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(40, 20, 51, 17));
        copyFilterButton = new QPushButton(BackupSetDialog);
        copyFilterButton->setObjectName(QString::fromUtf8("copyFilterButton"));
        copyFilterButton->setGeometry(QRect(20, 220, 61, 27));
        copyCriteriaButton = new QPushButton(BackupSetDialog);
        copyCriteriaButton->setObjectName(QString::fromUtf8("copyCriteriaButton"));
        copyCriteriaButton->setGeometry(QRect(20, 520, 61, 27));

        retranslateUi(BackupSetDialog);
        QObject::connect(okButton, SIGNAL(clicked()), BackupSetDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), BackupSetDialog, SLOT(reject()));
        QObject::connect(delFilterButton, SIGNAL(clicked()), BackupSetDialog, SLOT(delFilter()));
        QObject::connect(downFilterButton, SIGNAL(clicked()), BackupSetDialog, SLOT(downFilter()));
        QObject::connect(upFilterButton, SIGNAL(clicked()), BackupSetDialog, SLOT(upFilter()));
        QObject::connect(insertCriteriaButton, SIGNAL(clicked()), BackupSetDialog, SLOT(insertCriteria()));
        QObject::connect(insertFilterButton, SIGNAL(clicked()), BackupSetDialog, SLOT(insertFilter()));
        QObject::connect(deleteCriteriaButton, SIGNAL(clicked()), BackupSetDialog, SLOT(delCriteria()));
        QObject::connect(downCriteriaButton, SIGNAL(clicked()), BackupSetDialog, SLOT(downCriteria()));
        QObject::connect(upCriteriaButton, SIGNAL(clicked()), BackupSetDialog, SLOT(upCriteria()));
        QObject::connect(hashComboBox, SIGNAL(textChanged(QString)), BackupSetDialog, SLOT(selectHashMethod(QString)));
        QObject::connect(fromButton, SIGNAL(clicked()), BackupSetDialog, SLOT(selectFromRoot()));
        QObject::connect(toButton, SIGNAL(clicked()), BackupSetDialog, SLOT(selectToRoot()));
        QObject::connect(saveButton, SIGNAL(clicked()), BackupSetDialog, SLOT(saveBackupSet()));
        QObject::connect(loadButton, SIGNAL(clicked()), BackupSetDialog, SLOT(loadBackupSet()));
        QObject::connect(copyFilterButton, SIGNAL(clicked()), BackupSetDialog, SLOT(copyFilter()));
        QObject::connect(copyCriteriaButton, SIGNAL(clicked()), BackupSetDialog, SLOT(copyCriteria()));

        QMetaObject::connectSlotsByName(BackupSetDialog);
    } // setupUi

    void retranslateUi(QDialog *BackupSetDialog)
    {
        BackupSetDialog->setWindowTitle(QApplication::translate("BackupSetDialog", "Dialog", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fromRootLineEdit->setToolTip(QApplication::translate("BackupSetDialog", "Root location from which the backup begins.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        toRootLineEdit->setToolTip(QApplication::translate("BackupSetDialog", "Root location to which the backup is written.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_3->setText(QApplication::translate("BackupSetDialog", "Filters:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("BackupSetDialog", "Criteria:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        insertFilterButton->setToolTip(QApplication::translate("BackupSetDialog", "Insert new filter at the currently selected filter.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        insertFilterButton->setText(QApplication::translate("BackupSetDialog", "Insert", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        downFilterButton->setToolTip(QApplication::translate("BackupSetDialog", "Move the current filter down.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        downFilterButton->setText(QApplication::translate("BackupSetDialog", "Down", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        upFilterButton->setToolTip(QApplication::translate("BackupSetDialog", "Move the current filter up.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        upFilterButton->setText(QApplication::translate("BackupSetDialog", "Up", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        delFilterButton->setToolTip(QApplication::translate("BackupSetDialog", "Delete the selected filter.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        delFilterButton->setText(QApplication::translate("BackupSetDialog", "Delete", 0, QApplication::UnicodeUTF8));
        insertCriteriaButton->setText(QApplication::translate("BackupSetDialog", "Insert", 0, QApplication::UnicodeUTF8));
        upCriteriaButton->setText(QApplication::translate("BackupSetDialog", "Up", 0, QApplication::UnicodeUTF8));
        deleteCriteriaButton->setText(QApplication::translate("BackupSetDialog", "Delete", 0, QApplication::UnicodeUTF8));
        downCriteriaButton->setText(QApplication::translate("BackupSetDialog", "Down", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("BackupSetDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("BackupSetDialog", "OK", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fromButton->setToolTip(QApplication::translate("BackupSetDialog", "Select the root location from which the backup begins.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fromButton->setText(QApplication::translate("BackupSetDialog", "From", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toButton->setToolTip(QApplication::translate("BackupSetDialog", "Select the root location to which the backup is written.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toButton->setText(QApplication::translate("BackupSetDialog", "To", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        hashComboBox->setToolTip(QApplication::translate("BackupSetDialog", "Select the hash method.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        saveButton->setText(QApplication::translate("BackupSetDialog", "Save", 0, QApplication::UnicodeUTF8));
        loadButton->setText(QApplication::translate("BackupSetDialog", "Load", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        configFileLineEdit->setToolTip(QApplication::translate("BackupSetDialog", "Root location to which the backup is written.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("BackupSetDialog", "Config File:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("BackupSetDialog", "Hash:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        copyFilterButton->setToolTip(QApplication::translate("BackupSetDialog", "Move the current filter down.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        copyFilterButton->setText(QApplication::translate("BackupSetDialog", "Copy", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        copyCriteriaButton->setToolTip(QApplication::translate("BackupSetDialog", "Move the current filter down.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        copyCriteriaButton->setText(QApplication::translate("BackupSetDialog", "Copy", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BackupSetDialog: public Ui_BackupSetDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BACKUPSETDIALOG_H
