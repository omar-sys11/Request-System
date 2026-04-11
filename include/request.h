#pragma once

#include <string>
#include <ctime>
#include "enums.h"

class Request {
private:
    std::string id;
    std::string title;
    std::string description;
    Category category;
    std::string location;
    Urgency urgency;
    RequestStatus status;
    std::time_t timestamp;
    std::string creatorId;

public:
    Request(const std::string& id,
            const std::string& title,
            const std::string& description,
            Category category,
            const std::string& location,
            Urgency urgency,
            const std::string& creatorId);

    std::string getId() const;
    std::string getTitle() const;
    std::string getDescription() const;

    Category getCategory() const;
    Urgency getUrgency() const;
    RequestStatus getStatus() const;

    std::string getLocation() const;
    std::time_t getTimestamp() const;

    std::string getCreatorId() const;

    void accept();
    void close();
};
