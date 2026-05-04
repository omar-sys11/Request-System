#include "OpenAiAPIClient.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>

OpenAiApiClient::OpenAiApiClient(QObject *parent)
    : QObject(parent),
      networkManager(new QNetworkAccessManager(this)),
      apiKey(QString::fromUtf8(qgetenv("OPENAI_API_KEY"))) {
}

void OpenAiApiClient::askQuestion(const QString &question) {
    if (apiKey.isEmpty()) {
        emit apiError("OPENAI_API_KEY environment variable is not set.");
        return;
    }

    QUrl url("https://api.openai.com/v1/responses");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());

    QString prompt =
        "You are AI TA, a helpful teaching assistant at the American University in Cairo for a student request system app.\n"
        "Answer the student's question clearly and helpfully.\n"
        "Keep the answer concise unless the question needs a detailed explanation.\n\n"
        "Student question:\n" + question;

    QJsonObject body;
    body["model"] = "gpt-5.4-mini";
    body["input"] = prompt;
    body["store"] = false;

    QJsonDocument doc(body);
    QByteArray jsonData = doc.toJson();

    QNetworkReply *reply = networkManager->post(request, jsonData);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onAskQuestionFinished(reply);
    });
}

void OpenAiApiClient::onAskQuestionFinished(QNetworkReply *reply) {
    QByteArray responseData = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        QString errorMessage = "API request failed: " + reply->errorString();

        if (!responseData.isEmpty()) {
            errorMessage += "\nResponse: " + QString::fromUtf8(responseData);
        }

        emit apiError(errorMessage);
        reply->deleteLater();
        return;
    }

    QString text = extractTextFromResponse(responseData).trimmed();

    if (text.isEmpty()) {
        emit apiError("API returned an empty response.");
    } else {
        emit answerReceived(text);
    }

    reply->deleteLater();
}

QString OpenAiApiClient::extractTextFromResponse(const QByteArray &responseData) const {
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return "";
    }

    QJsonObject root = doc.object();

    QJsonArray output = root["output"].toArray();

    for (const QJsonValue &outputValue : output) {
        QJsonObject outputObj = outputValue.toObject();

        QJsonArray content = outputObj["content"].toArray();

        for (const QJsonValue &contentValue : content) {
            QJsonObject contentObj = contentValue.toObject();

            if (contentObj["type"].toString() == "output_text") {
                return contentObj["text"].toString();
            }
        }
    }

    return "";
}
