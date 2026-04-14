#pragma once
#include <QString>

class RequestService {
public:
    void addRequest(const QString& title,
                    const QString& category,
                    const QString& location,
                    const QString& ownerId);
};
