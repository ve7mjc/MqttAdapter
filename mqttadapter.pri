QT += core network

INCLUDEPATH += $$PWD

DEFINES += QT_BUILD_MQTT_LIB
include(QMqttClient\qmqttclient.pri)

HEADERS += \
    $$PWD/mqttadapter.h

SOURCES += \
    $$PWD/mqttadapter.cpp
