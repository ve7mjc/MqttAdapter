#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QDateTime>
#include <QTcpSocket>
#include <QTimer>
#include <QStringList>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

    void connectToHost(QHostAddress remoteAddr, quint16 port);
    void connectToHost();

    void setRemoteHostAddress(QHostAddress address);
    void setRemoteHostPort(quint16 port);
    void setReceiveDataTimeoutMs(unsigned int time);
    unsigned int receiveDateTimeoutMs() { return communicationsTimeoutTimeMs; }

private:

    QDateTime lastTransmittedCommsAt; // When serial comms last sent to IT-100
    QDateTime lastReceivedCommsAt; // When serial comms last received

    bool isConnected() { return _connected; }
    bool setCommunicationsGood(bool state);

//    void sendCommand(QByteArray command, QByteArray data);
//    void sendCommand(QByteArray command, quint16 data);
//    void sendCommand(QByteArray command);

    QHostAddress remoteHostAddress;
    quint16 remoteHostPort;

    QTcpSocket *socket;

    bool _connected;
    bool _connectionIntent;
    int _connectionAttempts;
    bool communicationsGood;
    bool waitingForResponse;
    bool _waitingForStatusUpdate;

    unsigned int communicationsTimeoutTimeMs;

    QString receivedData;

    int processReceivedLine(QByteArray data);

    // QTimers
    QTimer *pollTimer;
    QTimer *timedisciplineTimer;
    QTimer *communicationsTimeoutTimer;
    QTimer *moduleReconnectTimer;

private slots:

    void onPollTimerTimeout() {}
    void onConnected();
    void onCommunicationsTimeoutTimerTimeout();
    void on_tcpSocketStateChanged(QAbstractSocket::SocketState);
    void on_tcpSocketReadyRead();
    void on_connectRetryDelayTimeout();

signals:

    void connected();
    void disconnected();

    /**
     * Signal called when data is received from the remote host
     * This signal is line based, data is grouped by line and a signal
     * is emitted for each line.
     * \param data the line of text just received.
     */
    void lineReceived(QByteArray data);

    /**
     * Signal called when data is sent to the socket
     */
    void lineSent(QString data);

    /**
     * Communications have been received and remote host
     * is believed to be communicating
     */
    void communicationsBegin();

    /**
      * Signal called when data has not been received
      * for a period of time
      */
    void communicationsTimeout();

};

#endif // TCPCLIENT_H
