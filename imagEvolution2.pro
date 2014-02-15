#-------------------------------------------------
#
# Project created by QtCreator 2014-02-14T23:42:50
#
#-------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered


QT *= core gui widgets
CONFIG -= app_bundle
TEMPLATE = app
DESTDIR = $$PWD/deploy

HEADERS += \
    GA.h \
    GuiDisp.h \
    Population.h \
    Candidate.h

SOURCES += \
    main.cpp \
    GA.cpp \
    GuiDisp.cpp \
    Population.cpp \
    Candidate.cpp
