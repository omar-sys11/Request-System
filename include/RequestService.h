#pragma once
#include <QString>

class RequestService {
public:
    void handleRequest(const QString& title,
                       const QString& category,
                       const QString& location);
};
