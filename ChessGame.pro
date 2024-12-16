# ChessGame.pro
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChessGame
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           piece.cpp \
           terrain.cpp

HEADERS += mainwindow.h \
           piece.h \
           terrain.h \
           terrain.h

FORMS += mainwindow.ui
