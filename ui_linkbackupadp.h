/********************************************************************************
** Form generated from reading UI file 'linkbackupadp.ui'
**
** Created: Mon Apr 16 10:26:35 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINKBACKUPADP_H
#define UI_LINKBACKUPADP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LinkBackupADP
{
public:
    QAction *actionNew;
    QAction *actionExit;
    QAction *actionEditBackup;
    QAction *actionStartBackup;
    QAction *actionCancelBackup;
    QWidget *centralWidget;
    QTextEdit *logTextEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuBackup;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LinkBackupADP)
    {
        if (LinkBackupADP->objectName().isEmpty())
            LinkBackupADP->setObjectName(QString::fromUtf8("LinkBackupADP"));
        LinkBackupADP->resize(914, 658);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LinkBackupADP->sizePolicy().hasHeightForWidth());
        LinkBackupADP->setSizePolicy(sizePolicy);
        actionNew = new QAction(LinkBackupADP);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionExit = new QAction(LinkBackupADP);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionEditBackup = new QAction(LinkBackupADP);
        actionEditBackup->setObjectName(QString::fromUtf8("actionEditBackup"));
        actionStartBackup = new QAction(LinkBackupADP);
        actionStartBackup->setObjectName(QString::fromUtf8("actionStartBackup"));
        actionCancelBackup = new QAction(LinkBackupADP);
        actionCancelBackup->setObjectName(QString::fromUtf8("actionCancelBackup"));
        centralWidget = new QWidget(LinkBackupADP);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        logTextEdit = new QTextEdit(centralWidget);
        logTextEdit->setObjectName(QString::fromUtf8("logTextEdit"));
        logTextEdit->setGeometry(QRect(0, 0, 911, 581));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(4);
        sizePolicy2.setVerticalStretch(4);
        sizePolicy2.setHeightForWidth(logTextEdit->sizePolicy().hasHeightForWidth());
        logTextEdit->setSizePolicy(sizePolicy2);
        logTextEdit->setMinimumSize(QSize(50, 50));
        logTextEdit->setSizeIncrement(QSize(20, 20));
        LinkBackupADP->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(LinkBackupADP);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 914, 29));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuBackup = new QMenu(menuBar);
        menuBackup->setObjectName(QString::fromUtf8("menuBackup"));
        LinkBackupADP->setMenuBar(menuBar);
        mainToolBar = new QToolBar(LinkBackupADP);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        LinkBackupADP->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(LinkBackupADP);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        LinkBackupADP->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuBackup->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuBackup->addAction(actionEditBackup);
        menuBackup->addAction(actionStartBackup);
        menuBackup->addAction(actionCancelBackup);

        retranslateUi(LinkBackupADP);

        QMetaObject::connectSlotsByName(LinkBackupADP);
    } // setupUi

    void retranslateUi(QMainWindow *LinkBackupADP)
    {
        LinkBackupADP->setWindowTitle(QApplication::translate("LinkBackupADP", "LinkBackupADP", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("LinkBackupADP", "New", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("LinkBackupADP", "Exit", 0, QApplication::UnicodeUTF8));
        actionEditBackup->setText(QApplication::translate("LinkBackupADP", "Edit", 0, QApplication::UnicodeUTF8));
        actionStartBackup->setText(QApplication::translate("LinkBackupADP", "Start", 0, QApplication::UnicodeUTF8));
        actionCancelBackup->setText(QApplication::translate("LinkBackupADP", "Cancel", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("LinkBackupADP", "File", 0, QApplication::UnicodeUTF8));
        menuBackup->setTitle(QApplication::translate("LinkBackupADP", "Backup", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LinkBackupADP: public Ui_LinkBackupADP {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINKBACKUPADP_H
