#pragma once
#include "pcg_random.hpp"

#include <span>
namespace WeightedRandom
{
	struct WeightedItem
	{
        int32_t itemWeight;
		WeightedItem(int32_t itemWeightIn);
	};

    int32_t getTotalWeight(std::span<WeightedItem> collection);
	WeightedItem getRandomItem(pcg32_unique random, std::span<WeightedItem> collection, int32_t totalWeight);
	WeightedItem getRandomItem(std::span<WeightedItem> collection, int32_t weight);
	WeightedItem getRandomItem(pcg32& random, std::span<WeightedItem> collection);


}
