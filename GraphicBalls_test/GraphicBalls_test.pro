QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicBalls_test
TEMPLATE = app

win32:INCLUDEPATH += "C:/Program Files (x86)/googletest-distribution/include"

win32:LIBS += "C:/Program Files (x86)/googletest-distribution/lib/libgtest.a"
unix:LIBS += -L/usr/local/lib -lgtest
unix:LIBS += -lpthread

SOURCES += test_GraphicBalls.cpp \
    ../GraphicBalls/calculateforces.cpp \
    ../GraphicBalls/graphicscene.cpp \
    ../GraphicBalls/graphicball.cpp \
    calculateforcestest.cpp

HEADERS  += ../GraphicBalls/calculateforces.h \
    ../GraphicBalls/graphicscene.h \
    ../GraphicBalls/graphicball.h \
    calculateforcestest.h

INCLUDEPATH += ../GraphicBalls

CONFIG += c++14
