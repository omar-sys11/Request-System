#pragma once
#include <QString>

enum class RequestStatus {
    Open,
    Accepted,
    Closed
};

struct Request {
    QString title;
    QString category;
    QString location;
    RequestStatus status;
};
