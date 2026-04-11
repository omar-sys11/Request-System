#include "request.h"

Request::Request(const std::string& id,
                 const std::string& title,
                 const std::string& description,
                 Category category,
                 const std::string& location,
                 Urgency urgency,
                 const std::string& creatorId)
    : id(id),
      title(title),
      description(description),
      category(category),
      location(location),
      urgency(urgency),
      creatorId(creatorId),
      status(RequestStatus::Open),
      timestamp(std::time(nullptr)) {}

std::string Request::getId() const { return id; }
std::string Request::getTitle() const { return title; }
std::string Request::getDescription() const { return description; }
Category Request::getCategory() const { return category; }
Urgency Request::getUrgency() const { return urgency; }
RequestStatus Request::getStatus() const { return status; }
std::string Request::getLocation() const { return location; }
std::time_t Request::getTimestamp() const { return timestamp; }
std::string Request::getCreatorId() const { return creatorId; }

void Request::accept() {
    status = RequestStatus::Accepted;
}

void Request::close() {
    status = RequestStatus::Closed;
}
