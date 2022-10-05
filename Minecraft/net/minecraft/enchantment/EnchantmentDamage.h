#pragma once
#include "Enchantment.h"
#include "Util.h"
#include "math/MathHelper.h"

class EnchantmentDamage : public Enchantment
{
  public:
    EnchantmentDamage(Enchantment::Rarity rarityIn, int32_t damageTypeIn, std::span<const EntityEquipmentSlot> slots);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
    float calcDamageByCreature(int32_t level, const EnumCreatureAttribute &creatureType) const;
    std::string getName() const override;
    bool canApplyTogether(const Enchantment &ench) override;
    bool canApply(const ItemStack &stack) const;
    void onEntityDamaged(EntityLivingBase *user, Entity *target, int32_t level) const;
    int32_t damageType;

  private:
    static constexpr std::array<std::string_view, 3> DAMAGE_NAMES = {"all", "undead", "arthropods"};
    static constexpr std::array<int32_t, 3> MIN_COST              = {1, 5, 5};
    static constexpr std::array<int32_t, 3> LEVEL_COST            = {11, 8, 8};
    static constexpr std::array<int32_t, 3> LEVEL_COST_SPAN       = {20, 20, 20};
};
