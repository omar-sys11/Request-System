#pragma once
#include "feedItem.h"

class AcceptItem : public FeedItem
{
public:
    using FeedItem::FeedItem;

    FeedType getType() const override;

    std::string getTitle() const override;
    std::string getDisplayColor() const override;
};
