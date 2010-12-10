#-------------------------------------------------
#
# Project created by QtCreator 2010-10-18T18:18:37
#
#-------------------------------------------------

CONFIG += testlib

QT       += testlib \
            core gui

TARGET = GEF
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            serialinterface.cpp \
            filelogging.cpp \
    guiwidget.cpp \
    consolewidget.cpp \
    interpreter.cpp \
    configurationwidget.cpp

HEADERS  += mainwindow.h \
            defines.h \
            includes.h \
            serialinterface.h \
            filelogging.h \
    guiwidget.h \
    consolewidget.h \
    interpreter.h \
    configurationwidget.h

FORMS    += mainwindow.ui \
    consolewidget.ui \
    configurationwidget.ui

CONFIG +=   static
