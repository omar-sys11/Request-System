#include "RequestService.h"
#include <QDebug>

RequestService::RequestService() {
}

void RequestService::addRequest(const QString &title,
                                const QString &category,
                                const QString &location,
                                const QString &ownerId) {
    qDebug() << "Request added:";
    qDebug() << "Title:" << title;
    qDebug() << "Category:" << category;
    qDebug() << "Location:" << location;
    qDebug() << "OwnerId:" << ownerId;
}
