#include "RequestManager.h"
#include <QUuid>

const std::vector<Request>& RequestManager::getRequests() const {
    return requests;
}

void RequestManager::addRequest(const QString &title,
                                const QString &category,
                                const QString &location,
                                const QString &ownerId,
                                const QString &requestId) {
    QString id = requestId;

    if (id.isEmpty()) {
        id = QUuid::createUuid().toString();
    }

    if (findById(id) != nullptr) {
        return;
    }

    Request r;
    r.id = id;
    r.title = title;
    r.category = category;
    r.location = location;
    r.ownerId = ownerId;
    r.status = RequestStatus::Open;

    requests.push_back(r);
}

Request* RequestManager::findById(const QString &id) {
    for (auto &r : requests) {
        if (r.id == id) {
            return &r;
        }
    }

    return nullptr;
}

bool RequestManager::acceptRequest(const QString &id, const QString &userId) {
    Request *r = findById(id);

    if (!r) return false;
    if (r->ownerId == userId) return false;
    if (r->status != RequestStatus::Open) return false;

    r->status = RequestStatus::Accepted;
    return true;
}

bool RequestManager::closeRequest(const QString &id, const QString &userId) {
    Request *r = findById(id);

    if (!r) return false;
    if (r->ownerId != userId) return false;

    r->status = RequestStatus::Closed;
    return true;
}

bool RequestManager::markAccepted(const QString &id) {
    Request *r = findById(id);

    if (!r) return false;

    r->status = RequestStatus::Accepted;
    return true;
}

bool RequestManager::markClosed(const QString &id) {
    Request *r = findById(id);

    if (!r) return false;

    r->status = RequestStatus::Closed;
    return true;
}