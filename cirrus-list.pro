QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    maploaddisplay.cpp \
    mapmanager.cpp \
    mapviewer.cpp \
    node.cpp

HEADERS += \
    mainwindow.h \
    maploaddisplay.h \
    mapmanager.h \
    mapviewer.h \
    node.h

FORMS += \
    mainwindow.ui

resources.files = \
    resources/cloudbackground.jpg
resources.prefix = /

RESOURCES = resources

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
