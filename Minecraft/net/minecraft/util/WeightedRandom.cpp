#include "WeightedRandom.h"
#include <pcg_random.hpp>
#include <random>

namespace WeightedRandom
{
    int32_t getTotalWeight(std::span<WeightedItem> collection)
	{
		auto i = 0;
        auto j = 0;

		for (auto& weightedrandomitem : collection) {
			i += weightedrandomitem.itemWeight;
		}

		return i;
	}

	WeightedItem::WeightedItem(int32_t itemWeightIn)
	{
		itemWeight = itemWeightIn;
	}

	WeightedItem getRandomItem(pcg32& random, std::span<WeightedItem> collection, int32_t totalWeight)
	{
		if (totalWeight <= 0) {
			throw std::runtime_error("Total Weight muss higher than zero.");
		}
		else {
			std::uniform_int_distribution<int> uniform_dist(1, totalWeight);
			int i = uniform_dist(random);
			return getRandomItem(collection, i);
		}
	}

	WeightedItem getRandomItem(std::span<WeightedItem> collection, int32_t weight)
	{
		int i = 0;

		for (auto& t : collection) {
			weight -= t.itemWeight;
			if (weight < 0) {
				return t;
			}
		}

		return WeightedItem(0);
	}

	WeightedItem getRandomItem(pcg32_unique random, std::span<WeightedItem> collection)
	{
		return getRandomItem(random, collection, getTotalWeight(collection));
	}
}
