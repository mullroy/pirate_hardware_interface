QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#static: CONFIG += static
CONFIG += c++11
CONFIG += debug


QMAKE_CXXFLAGS += -Wno-deprecated-declarations

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
unix:!macx {
	LIBS += -L$$PWD/libraries -L/usr/local/lib -ldl -lcrypto
        ICON = images/pirate.ico
}
macx: {
	LIBS += -L$$PWD/libraries -L/opt/local/lib -dl -lcrypto
        ICON = images/pirate.icns
}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  keys.qrc \
  pictures.qrc

DISTFILES +=

unix:!macx {
	INCLUDEPATH += /usr/local/include
}

# Apple: Expect openssl (from MacPorts) in /opt/local
macx: { 
	INCLUDEPATH += /opt/local/include
}
