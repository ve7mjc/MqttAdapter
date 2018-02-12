#ifndef MQTTADAPTER_H
#define MQTTADAPTER_H

#include <QObject>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <QSettings>

#include "qmqttclient.h"

//enum LogLevel {
//    LOG_LEVEL_ERROR,
//    LOG_LEVEL_SECURITY,
//    LOG_LEVEL_NOTICE,
//    LOG_LEVEL_DEBUG
//};

//enum QosLevel {
//    QOS_0 = 0,
//    QOS_1 = 1,
//    QOS_2 = 2
//};

class MqttAdapter : public QObject
{
    Q_OBJECT
public:
    explicit MqttAdapter(QObject *parent = nullptr);

    void start();

private:

    QString settingsFile;
    void loadSettings(QString iniFile = QString());

    QSettings *settings;

    QString m_mqttRemoteHost;
    QString m_mqttRemotePort;
    QString mqttClientName;
    QString mqttTopicPrefix;

signals:

    void tcpLineReceived(QByteArray line);
    void mqttMessageReceived(QByteArray topic, QByteArray payload);

public slots:
};

#endif // MQTTADAPTER_H
