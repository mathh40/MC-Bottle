#pragma once
#include "../util/random/Random.h"

#include <span>
namespace WeightedRandom
{
struct WeightedItem
{
    int32_t itemWeight;
    WeightedItem(int32_t itemWeightIn);
};

int32_t getTotalWeight(std::span<WeightedItem> collection);
WeightedItem getRandomItem(Random &random, std::span<WeightedItem> collection, int32_t totalWeight);
WeightedItem getRandomItem(std::span<WeightedItem> collection, int32_t weight);
WeightedItem getRandomItem(Random &random, std::span<WeightedItem> collection);

} // namespace WeightedRandom
