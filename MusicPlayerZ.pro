#-------------------------------------------------
#
# Project created by QtCreator 2018-02-07T10:55:17
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayerZ
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    playerlogic.cpp \
    songsfile.cpp \
    lyricsfile.cpp \
    settingwidget.cpp \
    desklyricwidget.cpp \
    desktopremote.cpp \
    welcomewidget.cpp

HEADERS  += mainwidget.h \
    playerlogic.h \
    songsfile.h \
    lyricsfile.h \
    settingwidget.h \
    desklyricwidget.h \
    desktopremote.h \
    welcomewidget.h

FORMS    += mainwidget.ui \
    settingwidget.ui \
    desklyricwidget.ui \
    desktopremote.ui \
    welcomewidget.ui

RESOURCES += \
    resource.qrc
