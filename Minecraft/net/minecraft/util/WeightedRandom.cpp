#include "WeightedRandom.h"

#include <stdexcept>

namespace WeightedRandom
{
int32_t getTotalWeight(std::span<WeightedItem> collection)
{
    auto i = 0;
    auto j = 0;

    for (auto &weightedrandomitem : collection)
    {
        i += weightedrandomitem.itemWeight;
    }

    return i;
}

WeightedItem::WeightedItem(int32_t itemWeightIn)
{
    itemWeight = itemWeightIn;
}

WeightedItem getRandomItem(Random &random, std::span<WeightedItem> collection, int32_t totalWeight)
{
    if (totalWeight <= 0)
    {
        throw std::runtime_error("Total Weight muss higher than zero.");
    }
    else
    {
        auto i = random.nextInt(totalWeight);
        return getRandomItem(collection, i);
    }
}

WeightedItem getRandomItem(std::span<WeightedItem> collection, int32_t weight)
{
    int i = 0;

    for (auto &t : collection)
    {
        weight -= t.itemWeight;
        if (weight < 0)
        {
            return t;
        }
    }

    return WeightedItem(0);
}

WeightedItem getRandomItem(Random &random, std::span<WeightedItem> collection)
{
    return getRandomItem(random, collection, getTotalWeight(collection));
}
} // namespace WeightedRandom
