#-------------------------------------------------
#
# Project created by QtCreator 2011-12-06T19:46:05
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = projet_opti
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /home/cyril/opt/include

SOURCES += recuit.cpp \
    main_recuit.cpp \
    main_heur.cpp \
main.cpp

HEADERS += \
    heuristique.h \
    MyGraph.hpp \
    heuristique1.hpp \
    heuristique2.hpp \
    heuristique3.hpp \
    recuit.h
