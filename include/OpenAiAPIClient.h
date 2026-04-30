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

    void suggestCategory(const QString &title, const QString &location);

signals:
    void categorySuggested(const QString &category);
    void apiError(const QString &message);

private slots:
    void onSuggestCategoryFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;

    QString apiKey;
    QString extractTextFromResponse(const QByteArray &responseData) const;
};

#endif
