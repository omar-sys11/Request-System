#include "messageItem.h"

FeedType MessageItem::getType() const
{
    return FeedType::Message;
}

std::string MessageItem::getTitle() const
{
    return "Message from " + sender;
}

std::string MessageItem::getDisplayColor() const
{
    return "green";
}
