QT += core network

INCLUDEPATH += $$PWD

include(QMqttClient\qmqttclient.pri)

HEADERS += \
    $$PWD/mqttadapter.h

SOURCES += \
    $$PWD/mqttadapter.cpp
