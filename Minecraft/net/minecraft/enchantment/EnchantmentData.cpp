#include "EnchantmentData.h"
#include "Enchantment.h"
#include "pcg_random.hpp"

#include <random>

namespace EnchantmenRandom{
    EnchantmentData::EnchantmentData(Enchantment* enchantmentObj, int32_t enchLevel):
        WeightedRandom::WeightedItem(static_cast<int32_t>(enchantmentObj->getRarity())),
        enchantment(enchantmentObj),enchantmentLevel(enchLevel){
    }

    int32_t getTotalWeight(std::span<EnchantmentData> collection)
	{
		auto i = 0;
        auto j = 0;

		for (auto& weightedrandomitem : collection) {
			i += weightedrandomitem.itemWeight;
		}

		return i;
	}

	EnchantmentData getRandomItem(pcg32& random, std::span<EnchantmentData> collection, int32_t totalWeight)
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

	EnchantmentData getRandomItem(std::span<EnchantmentData> collection, int32_t weight)
	{
		int i = 0;

		for (auto& t : collection) {
			weight -= t.itemWeight;
			if (weight < 0) {
				return t;
			}
		}

		return EnchantmentData(nullptr,0);
	}

	EnchantmentData getRandomItem(pcg32_unique random, std::span<EnchantmentData> collection)
	{
		return getRandomItem(random, collection, getTotalWeight(collection));
	}
}
