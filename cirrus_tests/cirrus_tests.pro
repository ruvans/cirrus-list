QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    test_main.cpp \
    ../mapmanager.cpp


HEADERS += \
    ../mapmanager.h
