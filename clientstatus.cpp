#include "clientstatus.h"

#include <QJsonObject>
#include <QDebug>

ClientStatus::ClientStatus(QObject *parent) : QObject(parent)
{

}

QByteArray ClientStatus::toJson()
{
    QJsonObject clientStatus;
    clientStatus["state"] = "test";

    QJsonDocument doc(clientStatus);

    QByteArray results = doc.toJson(QJsonDocument::Compact);

    qDebug() << qPrintable(results);

    return results;
}

