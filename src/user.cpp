#include "user.h"

User::User(const std::string &id, const std::string &name)
    : id(id), displayName(name), connected(true) {
}

std::string User::getId() const {
    return id;
}

std::string User::getDisplayName() const {
    return displayName;
}

bool User::isConnected() const {
    return connected;
}

void User::setDisplayName(const std::string &name) {
    displayName = name;
}

void User::setConnectionStatus(bool status) {
    connected = status;
}
