#-------------------------------------------------
#
# Project created by QtCreator 2014-07-19T07:30:01
#
#-------------------------------------------------

QT       += core gui network webkitwidgets widgets sql

QTPLUGIN += qsqlite

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:RC_ICONS += opswat-logo.ico

ICON = opswat-logo.icns

QMAKE_INFO_PLIST = Info.plist
OTHER_FILES += Info.plist \
    createDB.txt

TARGET = MoUploader
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            httpprovider.cpp \
            hashprovider.cpp \
            mofile.cpp \
            htmltemplate.cpp \
            settingsdialog.cpp \
            aboutdialog.cpp \
            stringprovider.cpp \
    datetimeprovider.cpp \
    sqlliteprovider.cpp \
    eventlog.cpp \
    tablescan.cpp \

HEADERS  += mainwindow.h \
            httpprovider.h \
            Definition.h \
            hashprovider.h \
            mofile.h \
            htmltemplate.h \
            settingsdialog.h \
            aboutdialog.h \
            stringprovider.h \
    datetimeprovider.h \
    sqlliteprovider.h \
    eventlog.h \
    tablescan.h \

FORMS    += mainwindow.ui \
            settingsdialog.ui \
            aboutdialog.ui

RESOURCES += MoResources.qrc
