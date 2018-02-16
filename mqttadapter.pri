QT += core network

INCLUDEPATH += $$PWD
INCLUDEPATH += c:/Qt510/5.10.0/mingw53_32/include/qmqtt/

INCLUDEPATH += c:/Qt510/5.10.0/mingw53_32/lib

LIBS += -lqmqtt

HEADERS += \
    $$PWD/mqttadapter.h \
    $$PWD/tcpclient.h \
    $$PWD/clientstatus.h

SOURCES += \
    $$PWD/mqttadapter.cpp \
    $$PWD/tcpclient.cpp \
    $$PWD/clientstatus.cpp
