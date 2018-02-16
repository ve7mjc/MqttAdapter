QT += core network

win32:INCLUDEPATH += c:/Qt510/5.10.0/mingw53_32/include/qmqtt/
unix:INCLUDEPATH += /usr/include/arm-linux-gnueabihf/qt5/qmqtt/

win32:INCLUDEPATH += c:/Qt510/5.10.0/mingw53_32/lib
unix:INCLUDEPATH += /usr/local/include/qmqtt

LIBS += -lqmqtt

HEADERS += \
    $$PWD/mqttadapter.h \
    $$PWD/tcpclient.h \
    $$PWD/clientstatus.h

SOURCES += \
    $$PWD/mqttadapter.cpp \
    $$PWD/tcpclient.cpp \
    $$PWD/clientstatus.cpp
