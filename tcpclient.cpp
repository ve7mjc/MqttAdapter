#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

    // we have not yet begun communicating
    _connectionIntent = false;
    _connectionAttempts = 0;
    communicationsGood = false;
    waitingForResponse = false;
    _waitingForStatusUpdate = false;

    // Create TCP Socket
    _connected = false;
    socket = new QTcpSocket();
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(on_tcpSocketReadyRead()));
    connect(this->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(on_tcpSocketStateChanged(QAbstractSocket::SocketState)));

    // QTimer Poll Timer
    this->pollTimer = new QTimer(this);
    connect(this->pollTimer, SIGNAL(timeout()), this, SLOT(onPollTimerTimeout()));

    // Module Reconnect Timer
    moduleReconnectTimer = new QTimer();

    // Communications Timeout Timer
    communicationsTimeoutTimer = new QTimer(this);
    connect(communicationsTimeoutTimer, SIGNAL(timeout()),
            this, SLOT(onCommunicationsTimeoutTimerTimeout()));

}

void TcpClient::connectToHost(QHostAddress remoteAddr, quint16 port)
{
    remoteHostAddress = remoteAddr;
    remoteHostPort = port;

    // maintain connection after disconnect
    _connectionAttempts++;
    _connectionIntent = true;

    socket->connectToHost(remoteAddr,port);
}

void TcpClient::connectToHost()
{
    if (remoteHostAddress.isNull()) return;
    if (remoteHostPort > 65535 || remoteHostPort <= 0) return;
    connectToHost(remoteHostAddress, remoteHostPort);
}

void TcpClient::setRemoteHostAddress(QHostAddress address)
{
    remoteHostAddress = address;
}

void TcpClient::setRemoteHostPort(quint16 port)
{
    remoteHostPort = port;
}

void TcpClient::setReceiveDataTimeoutMs(unsigned int time)
{
    communicationsTimeoutTimeMs = time;
}

void TcpClient::onConnected()
{

    // Start Poll QTimer if timeout is greater than 1
//    if (it100CommunicationsTimeoutTime > 1)
//        pollTimer->start((it100CommunicationsTimeoutTime-1) * 1000);
//    else if (it100CommunicationsTimeoutTime == 1)
//        pollTimer->start(500);

    if (communicationsTimeoutTimeMs > 0) {
        communicationsTimeoutTimer->start(communicationsTimeoutTimeMs);
    }

}

void TcpClient::onCommunicationsTimeoutTimerTimeout()
{
    qDebug() << "communications timeout!";
}

void TcpClient::on_tcpSocketStateChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::HostLookupState:
        //qDebug() << qPrintable(QString("Looking up host %1").arg(remoteHostAddress.toString()));
        break;

    case QAbstractSocket::ConnectingState:
        qDebug() << qPrintable(QString("Connecting to %1:%2").arg(remoteHostAddress.toString()).arg(QString::number(remoteHostPort)));
        break;

    case QAbstractSocket::ConnectedState:
        _connected = true;
        _connectionAttempts = 0;
        waitingForResponse = false; // need to get things going again
        onConnected();
        emit connected();
        qDebug() << qPrintable(QString("Connected to %1:%2").arg(remoteHostAddress.toString()).arg(QString::number(remoteHostPort)));
        break;

    case QAbstractSocket::UnconnectedState:

        // Maintain state tracking
        _connected = false;
        communicationsGood = false;

        emit disconnected();
        qDebug() << qPrintable(QString("Disconnected from %1:%2").arg(remoteHostAddress.toString()).arg(QString::number(remoteHostPort)));

        // begin reconnect process if disconnect was unexpected
        // who are we kidding, there is no disconnect method anyways
        if (_connectionIntent) {
            if (_connectionAttempts++<=2) {
                qDebug() << qPrintable("attempting immediate reconnect");
                // connectToHost();
            }
            else { // delay connection attempt
              QTimer::singleShot(1000, this, SLOT(on_connectRetryDelayTimeout()));
            }
        }

        break;
    default:
        qDebug() << state;
        break;
    }

}

void TcpClient::on_tcpSocketReadyRead()
{

    // maintain communications timeout timer
    communicationsTimeoutTimer->start(communicationsTimeoutTimeMs);

    int avail = socket->bytesAvailable();
    if( avail > 0 ) {
        QByteArray receivedBytes;
        receivedBytes.resize(avail);
        int read = socket->read(receivedBytes.data(), receivedBytes.size());
        if( read > 0 ) {
            // Tokenize for newlines
            receivedData+=QString::fromUtf8(receivedBytes.data(),receivedBytes.size());
            if(receivedData.contains('\n'))
            {
                QStringList lineList=receivedData.split(QRegExp("\r\n|\n"));
                //If line ends with \n lineList will contain a trailing empty string
                //otherwise it will contain part of a line without the terminating \n
                //In both cases lineList.at(lineList.size()-1) should not be sent
                //with emit.
                int numLines=lineList.size()-1;
                receivedData=lineList.at(lineList.size()-1);
                for(int i=0;i<numLines;i++)
                {
                    emit lineReceived(lineList.at(i).toUtf8());
                    // qDebug() << qPrintable(QString("Received: %1").arg(lineList.at(i)));
                    // processReceivedLine(lineList.at(i).toUtf8());
                }
            }
        }
    }
}

void TcpClient::on_connectRetryDelayTimeout()
{

}

