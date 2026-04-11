#include "AcceptItem.h"

FeedType AcceptItem::getType() const
{
    return FeedType::Accept;
}

std::string AcceptItem::getTitle() const
{
    return sender + " accepted your request";
}

std::string AcceptItem::getDisplayColor() const
{
    return "purple";
}
