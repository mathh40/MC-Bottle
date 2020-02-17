#pragma once
#include "pcg_random.hpp"

namespace WeightedRandom
{
	struct WeightedItem
	{
		int itemWeight;
		explicit WeightedItem(int itemWeightIn);
	};

	int getTotalWeight(List collection);
	WeightedItem getRandomItem(pcg32_unique random, List collection, int totalWeight);
	WeightedItem getRandomItem(List collection, int weight);
	WeightedItem getRandomItem(Random random, List collection);


}
