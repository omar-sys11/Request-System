#ifndef OPENAIAPICLIENT_H
#define OPENAIAPICLIENT_H

#include <QObject>
#include <QString>

class QNetworkAccessManager;
class QNetworkReply;

class OpenAiApiClient : public QObject {
    Q_OBJECT

public:
    explicit OpenAiApiClient(QObject *parent = nullptr);

    void askQuestion(const QString &question);

signals:
    void answerReceived(const QString &answer);
    void apiError(const QString &message);

private slots:
    void onAskQuestionFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;

    QString apiKey;
    QString extractTextFromResponse(const QByteArray &responseData) const;
};

#endif
