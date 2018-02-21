QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicBalls
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicball.cpp \
    graphicscene.cpp \
    calculateforces.cpp

HEADERS  += mainwindow.h \
    graphicball.h \
    graphicscene.h \
    calculateforces.h
