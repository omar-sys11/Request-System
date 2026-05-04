#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class NetworkClient : public QObject {
    Q_OBJECT

public:
    explicit NetworkClient(QObject* parent = nullptr);

    void connectToServer(const QString& ip, quint16 port);

    void sendNewRequest(const QString& requestId,
                        const QString& title,
                        const QString& category,
                        const QString& location,
                        const QString& poster);

    void sendAccept(const QString& requestId, const QString& acceptedBy);

    void sendClose(const QString& requestId);

    bool isConnected();

signals:
    void messageReceived(QString type, QString requestId, QString title,
                         QString category, QString location,
                         QString poster, QString status);

    void connectedToServer();
    void disconnectedFromServer();
    void connectionError(QString error);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError err);

private:
    QTcpSocket* socket;

    void sendJson(const QString& json);
};

#endif