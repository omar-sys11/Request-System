#pragma once
#include <QString>
#include <vector>
#include "Request.h"

class RequestService {
public:
    void addRequest(const QString& title,
                    const QString& category,
                    const QString& location);

    std::vector<Request>& getRequests();

    void acceptRequest(int index);
    void closeRequest(int index);

private:
    std::vector<Request> requests;
};
