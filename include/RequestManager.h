#pragma once

#include <vector>
#include "Request.h"
#include <QString>

class RequestManager {
public:
    const std::vector<Request>& getRequests() const;

    void addRequest(const QString &title,
                    const QString &category,
                    const QString &location,
                    const QString &ownerId,
                    const QString &requestId = "");

    bool acceptRequest(const QString &requestId, const QString &userId);
    bool closeRequest(const QString &requestId, const QString &userId);

    bool markAccepted(const QString &requestId);
    bool markClosed(const QString &requestId);

private:
    std::vector<Request> requests;

    Request* findById(const QString &id);
};