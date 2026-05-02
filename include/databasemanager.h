#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QList>

struct StoredRequest
{
    QString title;
    QString category;
    QString location;
    QString status;
};

class DatabaseManager
{
public:
    DatabaseManager();

    bool initialize();
    bool addRequest(const QString& title,
                    const QString& category,
                    const QString& location,
                    const QString& status);

    QList<StoredRequest> loadRequests();
    QString getLastError() const;

private:
    QString lastError;
};

#endif // DATABASEMANAGER_H
