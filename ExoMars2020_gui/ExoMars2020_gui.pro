#-------------------------------------------------
#
# Project created by QtCreator 2018-12-18T15:10:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExoMars2020_gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        exomars2020_gui.cpp \
    dbviewer.cpp \
    mainwidget.cpp \
    secondtab.cpp \
    simucontrol.cpp

HEADERS += \
        exomars2020_gui.h \
    dbviewer.h \
    mainwidget.h \
    secondtab.h \
    simucontrol.h

FORMS += \
        exomars2020_gui.ui \
    dbviewer.ui \
    mainwidget.ui \
    secondtab.ui \
    simucontrol.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L'C:/Program Files/sqlite3/lib/' -lsqlite3

INCLUDEPATH += 'C:/Program Files/sqlite3/include'
DEPENDPATH += 'C:/Program Files/sqlite3/include'

win32: LIBS += -L'C:/Program Files/SystemC/lib/' -lsystemc

INCLUDEPATH += 'C:/Program Files/SystemC/include'
DEPENDPATH += 'C:/Program Files/SystemC/include'
