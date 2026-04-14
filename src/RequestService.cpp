#include "RequestService.h"
#include <QDebug>

void RequestService::addRequest(const QString& title,
                                const QString& category,
                                const QString& location)
{
    Request r;
    r.title = title;
    r.category = category;
    r.location = location;
    r.status = RequestStatus::Open;

    requests.push_back(r);

}

std::vector<Request>& RequestService::getRequests()
{
    return requests;
}

void RequestService::acceptRequest(int index)
{
    if (index < 0 || index >= (int)requests.size()) return;

    requests[index].status = RequestStatus::Accepted;
}

void RequestService::closeRequest(int index)
{
    if (index < 0 || index >= (int)requests.size()) return;

    requests[index].status = RequestStatus::Closed;
}
