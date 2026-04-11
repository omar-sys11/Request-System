#include "requestItem.h"

FeedType RequestItem::getType() const
{
    return FeedType::Request;
}

std::string RequestItem::getTitle() const
{
    return "Request from " + sender;
}

std::string RequestItem::getDisplayColor() const
{
    return "blue";
}
