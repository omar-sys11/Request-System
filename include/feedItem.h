#pragma once
#include <string>

enum class FeedType
{
    Request,
    Message,
    Accept
};

class FeedItem
{
protected:
    std::string sender;
    std::string content;
    long long timestamp;

public:
    FeedItem(std::string sender, std::string content, long long timestamp);
    virtual ~FeedItem() = default;

    virtual FeedType getType() const = 0;

    virtual std::string getTitle() const = 0;
    virtual std::string getDisplayColor() const = 0;

    std::string getSender() const;
    std::string getContent() const;
    long long getTimestamp() const;
};
