#pragma once
#include <string>

class User {
private:
    std::string id;
    std::string displayName;
    bool connected;

public:
    User(const std::string& id, const std::string& name);

    std::string getId() const;
    std::string getDisplayName() const;

    bool isConnected() const;

    void setDisplayName(const std::string& name);
    void setConnectionStatus(bool status);
};
