#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>

struct StoredUser
{
    QString id;
    QString username;
};

class DatabaseManager
{
public:
    DatabaseManager();

    bool initialize();

    bool signUpUser(const QString& username, const QString& password);
    bool loginUser(const QString& username, const QString& password, StoredUser& user);

    QString getLastError() const;

private:
    QString lastError;
};

#endif // DATABASEMANAGER_H