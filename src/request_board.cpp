#include "FeedBoard.h"

void FeedBoard::add(std::unique_ptr<FeedItem> item)
{
    items.push_back(std::move(item));
}

void FeedBoard::remove(size_t index)
{
    if (index < items.size())
        items.erase(items.begin() + index);
}

const FeedItem* FeedBoard::get(size_t index) const
{
    if (index >= items.size()) return nullptr;
    return items[index].get();
}

size_t FeedBoard::size() const
{
    return items.size();
}

void FeedBoard::clear()
{
    items.clear();
}

std::vector<const FeedItem*> FeedBoard::getAll() const
{
    std::vector<const FeedItem*> result;
    result.reserve(items.size());

    for (const auto& item : items)
        result.push_back(item.get());

    return result;
}
