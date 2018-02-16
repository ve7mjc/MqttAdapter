#ifndef MQTTADAPTER_H
#define MQTTADAPTER_H

#include <QObject>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <QSettings>

#include "qmqtt.h"
#include "tcpclient.h"

#include "clientstatus.h"

//enum LogLevel {
//    LOG_LEVEL_ERROR,
//    LOG_LEVEL_SECURITY,
//    LOG_LEVEL_NOTICE,
//    LOG_LEVEL_DEBUG
//};


/*

  Take advantage of Mqtt Client LWT Will to track client status.



 */

enum QosLevel {
    QOS_0 = 0,
    QOS_1 = 1,
    QOS_2 = 2
};

class MqttAdapter : public QObject
{
    Q_OBJECT
public:
    explicit MqttAdapter(QObject *parent = nullptr);

    void start();

    void mqttPublish(QString topic, QString payload);

private:

    QString settingsFile;
    void loadSettings(QString iniFile = QString());
    QSettings *settings;

    QMQTT::Client *mqttClient;
    QString m_mqttRemoteHost;
    unsigned int m_mqttRemotePort;
    QString mqttClientName;
    QString mqttTopicPrefix;

    ClientStatus clientStatus;

    TcpClient *tcpClient;


signals:

    void tcpLineReceived(QByteArray line);
    void mqttMessageReceived(QByteArray topic, QByteArray payload);

public slots:

    void on_mqttConnected();
    void on_mqttMessageReceived(QByteArray topic, QByteArray payload);

    void on_tcpLineReceived(QByteArray line);


};

#endif // MQTTADAPTER_H
