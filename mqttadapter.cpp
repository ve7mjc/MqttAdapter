#include "mqttadapter.h"

MqttAdapter::MqttAdapter(QObject *parent) : QObject(parent)
{

    mqttClient = new QMQTT::Client();

    connect(mqttClient, SIGNAL(connected()),
            this, SLOT(on_mqttConnected()));
    connect(mqttClient, SIGNAL(received(QMQTT::Message)),
            this, SLOT(on_mqttMessageReceived(QMQTT::Message)));

    tcpClient = new TcpClient();

    settings = NULL;

    connect(tcpClient, SIGNAL(lineReceived(QByteArray)),
            this, SLOT(on_tcpLineReceived(QByteArray)));

}

// Load settings from supplied INI file via QSettings
//
void MqttAdapter::start()
{

    loadSettings();

    // TCP
    // tcpClient->connectToHost();

    // MQTT //
    mqttClient->setClientId(mqttClientName);

    mqttClient->setWillTopic(QString("client/%1/status").arg(mqttClientName));
    mqttClient->setWillMessage("{ \"state:\" : \"disconnected\" }");
    mqttClient->setWillQos(QOS_2);
    mqttClient->setWillRetain(true);

    mqttClient->setHost(QHostAddress(m_mqttRemoteHost));
    mqttClient->setPort(m_mqttRemotePort);

    qDebug() << qPrintable(QString("Connecting to MQTT Broker %1:%2").arg(mqttClient->host().toString()).arg(mqttClient->port()));

    mqttClient->connectToHost();

}

void MqttAdapter::mqttPublish(QString topic, QString payload)
{
    QMQTT::Message msg;
    msg.setTopic(topic);
    msg.setPayload(payload.toUtf8());
    mqttClient->publish(msg);
}

void MqttAdapter::mqttSubscribe(const QString &topic, const quint8 qos)
{
    mqttClient->subscribe(topic, qos);
}

void MqttAdapter::loadSettings(QString iniFile)
{

    if (!iniFile.isEmpty()) settingsFile = iniFile;
    else settingsFile = "settings.cfg";

    settings = new QSettings(settingsFile, QSettings::IniFormat, this);
    if (settings->status() != QSettings::NoError) {
        qDebug() << qPrintable(QString("error loading settings from %1").arg(settingsFile));
        // writeLog("error accessing settings.cfg", LOG_LEVEL_ERROR);
    } else {

        qDebug() << qPrintable(QString("loading settings from %1").arg(settingsFile));

        settings->beginGroup("mqtt");
        m_mqttRemoteHost = settings->value("host", QString()).toString();
        m_mqttRemotePort = settings->value("port", 1883).toInt();
        mqttClientName = settings->value("client_name", QString("mqtt-adapter")).toString();
        mqttTopicPrefix = settings->value("topic_prefix", QString("test/")).toString();
        mqttTopicPrefix = mqttTopicPrefix.append(mqttClientName);
        settings->endGroup();

        if (!settings->contains("mode")) {
            qDebug() << qPrintable("mode not specified in config");
        } else {
            // TCP Client Mode
            if (settings->value("mode").toString().contains("tcp-client")) {
                qDebug() << qPrintable(QString("Mode: %1").arg(settings->value("mode").toString()));

                settings->beginGroup("tcp-client");
                tcpClient->setRemoteHostAddress(QHostAddress(settings->value("host", QString()).toString()));
                tcpClient->setRemoteHostPort(settings->value("port", quint16()).toInt());
                tcpClient->setReceiveDataTimeoutMs(settings->value("receive_timeout", 0).toInt());
                qDebug() << qPrintable(QString("Receive data Timeout: %1 ms").arg(tcpClient->receiveDateTimeoutMs()));
                settings->endGroup();
            }
        }
    }
}

void MqttAdapter::on_mqttConnected()
{
    qDebug() << qPrintable("Connected to MQTT broker");

    emit mqttConnected();

    clientStatus.toJson();
}

void MqttAdapter::on_tcpLineReceived(QByteArray line)
{
    emit tcpLineReceived(line);
}

void MqttAdapter::on_mqttMessageReceived(QMQTT::Message message)
{
    emit mqttMessageReceived(message);
    emit mqttMessageReceived(message.topic().toUtf8(),message.payload());
}
