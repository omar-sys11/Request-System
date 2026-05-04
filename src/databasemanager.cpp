#include "databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

DatabaseManager::DatabaseManager()
{
}

bool DatabaseManager::initialize()
{
    QSqlDatabase database;

    if (QSqlDatabase::contains("requests_connection")) {
        database = QSqlDatabase::database("requests_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE", "requests_connection");
        database.setDatabaseName("requests.db");
    }

    if (!database.open()) {
        lastError = database.lastError().text();
        return false;
    }

    QSqlQuery query(database);

    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS requests ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL,"
        "category TEXT NOT NULL,"
        "location TEXT NOT NULL,"
        "status TEXT NOT NULL"
        ")"
    );

    if (!success) {
        lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::addRequest(const QString& title,
                                 const QString& category,
                                 const QString& location,
                                 const QString& status)
{
    QSqlDatabase database = QSqlDatabase::database("requests_connection");

    if (!database.isOpen()) {
        if (!initialize()) {
            return false;
        }
    }

    QSqlQuery query(database);

    query.prepare(
        "INSERT INTO requests (title, category, location, status) "
        "VALUES (:title, :category, :location, :status)"
    );

    query.bindValue(":title", title);
    query.bindValue(":category", category);
    query.bindValue(":location", location);
    query.bindValue(":status", status);

    if (!query.exec()) {
        lastError = query.lastError().text();
        return false;
    }

    return true;
}

QList<StoredRequest> DatabaseManager::loadRequests()
{
    QList<StoredRequest> requests;

    QSqlDatabase database = QSqlDatabase::database("requests_connection");

    if (!database.isOpen()) {
        if (!initialize()) {
            return requests;
        }
    }

    QSqlQuery query(database);

    bool success = query.exec(
        "SELECT title, category, location, status "
        "FROM requests "
        "ORDER BY id DESC"
    );

    if (!success) {
        lastError = query.lastError().text();
        return requests;
    }

    while (query.next()) {
        StoredRequest request;
        request.title = query.value(0).toString();
        request.category = query.value(1).toString();
        request.location = query.value(2).toString();
        request.status = query.value(3).toString();

        requests.append(request);
    }

    return requests;
}

QString DatabaseManager::getLastError() const
{
    return lastError;
}
