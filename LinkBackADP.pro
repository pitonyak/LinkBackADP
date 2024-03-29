#-------------------------------------------------
#
# Project created by QtCreator 2009-06-06T22:49:31
#
#-------------------------------------------------

QT       += xml testlib
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QMAKE_CXXFLAGS += -std=c++0x

TARGET = LinkBackADP
TEMPLATE = app


SOURCES += main.cpp\
        linkbackupadp.cpp \
    backupsetdialog.cpp \
    linkbackfilter.cpp \
    stringhelper.cpp \
    backupset.cpp \
    filtertablemodel.cpp \
    linkbackfilterdelegate.cpp \
    checkboxonlydelegate.cpp \
    criteriaforfilematchtablemodel.cpp \
    criteriaforfilematch.cpp \
    dbfileentry.cpp \
    dbfileentries.cpp \
    linkbackupthread.cpp \
    copylinkutil.cpp \
    simpleloggeradp.cpp \
    simpleloggerroutinginfo.cpp \
    xmlutility.cpp \
    enhancedqcryptographichash.cpp \
    logmessagecontainer.cpp \
    logmessagequeue.cpp \
    qtenummapper.cpp \
    logroutinginfodialog.cpp \
    messagecomponenttablemodel.cpp \
    logconfigdialog.cpp \
    logroutinginfotablemodel.cpp \
    restorebackup.cpp \
    dbfileentrytreeitem.cpp \
    dbfileentriestreemodel.cpp

HEADERS  += linkbackupadp.h \
    backupsetdialog.h \
    linkbackfilter.h \
    stringhelper.h \
    backupset.h \
    filtertablemodel.h \
    linkbackfilterdelegate.h \
    checkboxonlydelegate.h \
    criteriaforfilematchtablemodel.h \
    criteriaforfilematch.h \
    dbfileentry.h \
    dbfileentries.h \
    linkbackupthread.h \
    copylinkutil.h \
    linkbackupglobals.h \
    simpleloggeradp.h \
    simpleloggerroutinginfo.h \
    xmlutility.h \
    enhancedqcryptographichash.h \
    logmessagecontainer.h \
    logmessagequeue.h \
    qtenummapper.h \
    logroutinginfodialog.h \
    messagecomponenttablemodel.h \
    logconfigdialog.h \
    logroutinginfotablemodel.h \
    restorebackup.h \
    dbfileentrytreeitem.h \
    dbfileentriestreemodel.h

FORMS    += linkbackupadp.ui \
    backupsetdialog.ui \
    logroutinginfodialog.ui
