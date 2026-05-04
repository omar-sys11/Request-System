#include "databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QUuid>

DatabaseManager::DatabaseManager()
{
}

bool DatabaseManager::initialize()
{
    QSqlDatabase database;

    if (QSqlDatabase::contains("users_connection")) {
        database = QSqlDatabase::database("users_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE", "users_connection");
        database.setDatabaseName("users.db");
    }

    if (!database.open()) {
        lastError = database.lastError().text();
        return false;
    }

    QSqlQuery query(database);

    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id TEXT PRIMARY KEY,"
        "username TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL"
        ")"
    );

    if (!success) {
        lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::signUpUser(const QString& username, const QString& password)
{
    if (!initialize()) {
        return false;
    }

    QSqlDatabase database = QSqlDatabase::database("users_connection");
    QSqlQuery query(database);

    QString id = QUuid::createUuid().toString();

    query.prepare(
        "INSERT INTO users (id, username, password) "
        "VALUES (:id, :username, :password)"
    );

    query.bindValue(":id", id);
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::loginUser(const QString& username, const QString& password, StoredUser& user)
{
    if (!initialize()) {
        return false;
    }

    QSqlDatabase database = QSqlDatabase::database("users_connection");
    QSqlQuery query(database);

    query.prepare(
        "SELECT id, username "
        "FROM users "
        "WHERE username = :username AND password = :password"
    );

    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        lastError = query.lastError().text();
        return false;
    }

    if (!query.next()) {
        lastError = "Invalid username or password.";
        return false;
    }

    user.id = query.value(0).toString();
    user.username = query.value(1).toString();

    return true;
}

QString DatabaseManager::getLastError() const
{
    return lastError;
}