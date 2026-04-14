#pragma once
#include "user.h"
#include <string>

class UserService {
public:
    User createUser(const std::string& name);
};
