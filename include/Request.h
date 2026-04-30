#pragma once

#include <QString>

enum class RequestStatus {
    Open,
    Accepted,
    Closed
};

struct Request {
    QString id;
    QString title;
    QString category;
    QString location;
    QString ownerId;
    RequestStatus status;
};