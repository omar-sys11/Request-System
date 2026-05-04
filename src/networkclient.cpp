#include "networkclient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>

NetworkClient::NetworkClient(QObject* parent) : QObject(parent) {
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &NetworkClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkClient::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);
    connect(socket, &QAbstractSocket::errorOccurred, this, &NetworkClient::onError);
}

void NetworkClient::connectToServer(const QString& ip, quint16 port) {
    socket->connectToHost(ip, port);
}

bool NetworkClient::isConnected() {
    return socket->state() == QAbstractSocket::ConnectedState;
}

void NetworkClient::sendJson(const QString& json) {
    if (!isConnected()) return;
    socket->write(json.toUtf8());
    socket->flush();
}

void NetworkClient::sendNewRequest(const QString& title, const QString& category,
                                    const QString& location, const QString& poster) {
    QJsonObject obj;
    obj["type"] = "new_request";
    obj["title"] = title;
    obj["category"] = category;
    obj["location"] = location;
    obj["poster"] = poster;
    obj["status"] = "open";
    obj["request_id"] = QString::number(QDateTime::currentMSecsSinceEpoch());

    QJsonDocument doc(obj);
    sendJson(doc.toJson(QJsonDocument::Compact));
}

void NetworkClient::sendAccept(const QString& requestId, const QString& acceptedBy) {
    QJsonObject obj;
    obj["type"] = "accept_request";
    obj["request_id"] = requestId;
    obj["accepted_by"] = acceptedBy;
    obj["status"] = "accepted";

    QJsonDocument doc(obj);
    sendJson(doc.toJson(QJsonDocument::Compact));
}

void NetworkClient::sendClose(const QString& requestId) {
    QJsonObject obj;
    obj["type"] = "close_request";
    obj["request_id"] = requestId;
    obj["status"] = "closed";

    QJsonDocument doc(obj);
    sendJson(doc.toJson(QJsonDocument::Compact));
}

void NetworkClient::onConnected() {
    emit connectedToServer();
}

void NetworkClient::onDisconnected() {
    emit disconnectedFromServer();
}

void NetworkClient::onReadyRead() {
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()){ 
	return;
}
    QJsonObject obj = doc.object();
    QString type = obj["type"].toString();
    QString requestId = obj["request_id"].toString();
    QString title = obj["title"].toString();
    QString category = obj["category"].toString();
    QString location = obj["location"].toString();
    QString poster = obj["poster"].toString();
    QString status = obj["status"].toString();

    emit messageReceived(type, requestId, title, category, location, poster, status);
}

void NetworkClient::onError(QAbstractSocket::SocketError err) {
    emit connectionError(socket->errorString());
}
