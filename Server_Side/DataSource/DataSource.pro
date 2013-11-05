#-------------------------------------------------
#
# Project created by QtCreator 2013-10-19T22:44:31
#
#-------------------------------------------------

QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataSource
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    serialconnection.cpp \
    dbconnection.cpp \
    mainwidget.cpp \
    customermonitor.cpp \
    posdataworker.cpp \
    errorlog.cpp \
    customerlist.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    serialconnection.h \
    config.h \
    dbconnection.h \
    mainwidget.h \
    sqldefines.h \
    customermonitor.h \
    serialdefines.h \
    posdataworker.h \
    errorlog.h \
    customerlist_shared.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    errorlog.ui \
    mainwidget.ui
