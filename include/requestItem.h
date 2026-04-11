#pragma once
#include "FeedItem.h"

class RequestItem : public FeedItem
{
public:
    using FeedItem::FeedItem;

    FeedType getType() const override;

    std::string getTitle() const override;
    std::string getDisplayColor() const override;
};
