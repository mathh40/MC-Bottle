#pragma once
#include "../util/WeightedRandom.h"
#include "pcg_random.hpp"

class Enchantment;

namespace EnchantmenRandom
{
class EnchantmentData : public WeightedRandom::WeightedItem
{
  public:
    Enchantment *enchantment;
    int32_t enchantmentLevel;

    EnchantmentData(Enchantment *enchantmentObj, int32_t enchLevel);
};

int32_t getTotalWeight(std::span<EnchantmentData> collection);
EnchantmentData getRandomItem(pcg32_unique random, std::span<EnchantmentData> collection, int32_t totalWeight);
EnchantmentData getRandomItem(std::span<EnchantmentData> collection, int32_t weight);
EnchantmentData getRandomItem(pcg32 &random, std::span<EnchantmentData> collection);
} // namespace EnchantmenRandom
