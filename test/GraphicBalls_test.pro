QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicBalls_test
TEMPLATE = app

LIBS += -L/usr/local/lib -lgtest
LIBS += -lpthread

SOURCES += test_GraphicBalls.cpp \
    ../calculateforces.cpp \
    ../graphicscene.cpp \
    ../graphicball.cpp

HEADERS  += ../calculateforces.h \
    ../graphicscene.h \
    ../graphicball.h

INCLUDEPATH += ../

CONFIG += c++11
