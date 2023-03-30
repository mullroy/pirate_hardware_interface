QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#static: CONFIG += static
CONFIG += c++11
CONFIG += debug

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    communication/comms_filetx.cpp \
    libraries/SerialPort_lin.cpp \
    libraries/md5sum.cpp \
    main.cpp \
    mainwindow.cpp \
    libraries/crc16.c \
    communication/message_framing.cpp

HEADERS += \
    communication/comms_filetx.h \
    communication/comms_filetx_i.h \
    libraries/md5sum.h \
    mainwindow.h \
    libraries/crc16.h \
    libraries/SerialPort.h \
    communication/message_framing.h

FORMS += \
    mainwindow.ui

# Project base path is referenced with $$PWD
LIBS += -L$$PWD/libraries -ldl -lcrypto

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  keys.qrc \
  pictures.qrc

DISTFILES +=
