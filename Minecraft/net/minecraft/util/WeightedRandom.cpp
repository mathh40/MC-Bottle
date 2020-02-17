#include "WeightedRandom.h"
#include <random>

namespace WeightedRandom
{
	int getTotalWeight(List collection)
	{
		int i = 0;
		int j = 0;

		for (int k = collection.size(); j < k; ++j) {
			WeightedItem weightedrandomitem = collection.get(j);
			i += weightedrandomitem.itemWeight;
		}

		return i;
	}

	WeightedItem::WeightedItem(int itemWeightIn)
	{
		itemWeight = itemWeightIn;
	}

	WeightedItem getRandomItem(pcg32_unique random, List collection, int totalWeight)
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

	WeightedItem getRandomItem(List collection, int weight)
	{
		int i = 0;

		for (int j = collection.size(); i < j; ++i) {
			WeightedItem t = collection.get(i);
			weight -= t.itemWeight;
			if (weight < 0) {
				return t;
			}
		}

		return WeightedItem(0);
	}

	WeightedItem getRandomItem(pcg32_unique random, List collection)
	{
		return getRandomItem(random, collection, getTotalWeight(collection));
	}
}
