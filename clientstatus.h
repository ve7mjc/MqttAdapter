#ifndef CLIENTSTATUS_H
#define CLIENTSTATUS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <QList>

enum ClientState {
    STATUS_OK = 0,
    STATUS_FAULT,
    STATUS_WARNING,
    STATUS
};

class ClientStatus : public QObject
{
    Q_OBJECT
public:
    explicit ClientStatus(QObject *parent = nullptr);

    ClientState clientState() { return state; }
    void setClientState(ClientState newState) { state = newState; }

    QByteArray toJson();

private:

    ClientState state;
    QList<QString> messages;


signals:

public slots:
};

#endif // CLIENTSTATUS_H
