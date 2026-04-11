#pragma once
#include <vector>
#include <memory>
#include "FeedItem.h"

class FeedBoard
{
private:
    std::vector<std::unique_ptr<FeedItem>> items;

public:
    void add(std::unique_ptr<FeedItem> item);
    void remove(size_t index);

    const FeedItem* get(size_t index) const;
    size_t size() const;

    void clear();

    std::vector<const FeedItem*> getAll() const;
};
