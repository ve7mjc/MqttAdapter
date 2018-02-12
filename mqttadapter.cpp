#include "mqttadapter.h"

MqttAdapter::MqttAdapter(QObject *parent) : QObject(parent)
{

    settings = NULL;

}

// Load settings from supplied INI file via QSettings
//
void MqttAdapter::start()
{
    loadSettings();


}

void MqttAdapter::loadSettings(QString iniFile)
{

    if (!iniFile.isEmpty()) settingsFile = iniFile;
    else settingsFile = "settings.cfg";

    settings = new QSettings(settingsFile, QSettings::IniFormat, this);
    if (settings->status() != QSettings::NoError) {
        qDebug() << "error accessing settings.cfg";
        // writeLog("error accessing settings.cfg", LOG_LEVEL_ERROR);
    } else {

//        settings->beginGroup("it100");
//        it100RemoteHost = settings->value("host", QString()).toString();
//        it100RemotePort = settings->value("port", quint16()).toInt();
//        it100UserCode = settings->value("user_code", QString()).toString();
//        settings->endGroup();

        settings->beginGroup("mqtt");
        m_mqttRemoteHost = settings->value("host", QString()).toString();
        m_mqttRemotePort = settings->value("port", QString()).toInt();
        mqttClientName = settings->value("client_name", QString("it100")).toString();
        mqttTopicPrefix = settings->value("topic_prefix", QString("alarm/")).toString();
        mqttTopicPrefix = mqttTopicPrefix.append(mqttClientName);
        settings->endGroup();

    }
}
