#include "FeedItem.h"

FeedItem::FeedItem(std::string sender, std::string content, long long timestamp)
    : sender(std::move(sender)), content(std::move(content)), timestamp(timestamp) {}

std::string FeedItem::getSender() const { return sender; }
std::string FeedItem::getContent() const { return content; }
long long FeedItem::getTimestamp() const { return timestamp; }
