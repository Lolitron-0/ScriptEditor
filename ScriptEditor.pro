QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/block.cpp \
    src/blockconnection.cpp \
    src/blockconnectionpin.cpp \
    src/em1.cpp \
    src/graphicelementbase.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/block.hpp \
    src/blockconnection.hpp \
    src/blockconnectionpin.hpp \
    src/em1.hpp \
    src/graphicelementbase.h \
    src/inputlistener.hpp \
    src/mainwindow.h \
    src/utils.hpp

FORMS += \
    mainwindow.ui

INCLUDEPATH += src/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/res.qrc
