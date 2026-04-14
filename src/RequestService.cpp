#include "RequestService.h"
#include <QDebug>

void RequestService::handleRequest(const QString& title,
                                   const QString& category,
                                   const QString& location)
{
    // simple "business logic"
    qDebug() << "Request stored:"
             << title << category << location;
}
