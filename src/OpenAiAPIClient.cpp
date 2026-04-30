#include "OpenAiApiClient.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QDebug>

OpenAiApiClient::OpenAiApiClient(QObject *parent)
    : QObject(parent),
      networkManager(new QNetworkAccessManager(this)),
      apiKey(QString::fromUtf8(qgetenv("OPENAI_API_KEY"))) {
}

void OpenAiApiClient::suggestCategory(const QString &title, const QString &location) {
    if (apiKey.isEmpty()) {
        emit apiError("OPENAI_API_KEY environment variable is not set.");
        return;
    }

    QUrl url("https://api.openai.com/v1/responses");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());

    QString prompt =
        "You are helping categorize community help requests.\n"
        "Return exactly one category from this list only:\n"
        "Food, Transport, Study, Medical, Housing, Technology, Other.\n\n"
        "Request title: " + title + "\n"
        "Location: " + location + "\n\n"
        "Category:";

    QJsonObject body;
    body["model"] = "gpt-5.4-mini";
    body["input"] = prompt;
    body["store"] = false;

    QJsonDocument doc(body);
    QByteArray jsonData = doc.toJson();

    QNetworkReply *reply = networkManager->post(request, jsonData);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onSuggestCategoryFinished(reply);
    });
}

void OpenAiApiClient::onSuggestCategoryFinished(QNetworkReply *reply) {
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
        emit categorySuggested(text);
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
