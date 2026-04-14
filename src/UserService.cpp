#include "UserService.h"
#include <QUuid>

User UserService::createUser(const std::string &name) {
    std::string id = QUuid::createUuid().toString().toStdString();
    return User(id, name);
}
