#pragma once
#include "Enchantment.h"
#include "math/MathHelper.h"

class BlockPos;
class World;

class EnchantmentFrostWalker : public Enchantment
{
  public:
    EnchantmentFrostWalker(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    bool isTreasureEnchantment() const override;
    int32_t getMaxLevel() const override;

    static void freezeNearby(EntityLivingBase *living, World *worldIn, const BlockPos &pos, int32_t level);

    bool canApplyTogether(const Enchantment &ench) const override;
};
