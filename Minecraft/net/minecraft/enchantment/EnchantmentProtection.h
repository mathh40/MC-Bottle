#pragma once
#include "DamageSource.h"
#include "Enchantment.h"
#include "Util.h"

class EnchantmentProtection : public Enchantment
{
  public:
    static class Type
    {
      public:
        static Type ALL;
        static Type FIRE;
        static Type FALL;
        static Type EXPLOSION;
        static Type PROJECTILE;

        Type(std::string_view name, int32_t minimal, int32_t perLevelEnchantability, int32_t p_i47051_6_);
        std::string_view getTypeName() const;
        int32_t getMinimalEnchantability() const;
        int32_t getEnchantIncreasePerLevel() const;

        friend bool operator==(const Type &lhs, const Type &rhs);
        friend bool operator!=(const Type &lhs, const Type &rhs);

      private:
        std::string_view typeName;
        int32_t minEnchantability;
        int32_t levelCost;
        int32_t levelCostSpan;
    };

    EnchantmentProtection(Enchantment::Rarity rarityIn, EnchantmentProtection::Type protectionTypeIn,
                          std::span<const EntityEquipmentSlot> slots);

    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
    int32_t calcModifierDamage(int32_t level, const DamageSource::DamageSource &source) const override;
    std::string getName() const override;
    bool canApplyTogether(const Enchantment &ench) const override;
    static int32_t getFireTimeForEntity(EntityLivingBase *p_92093_0_, int32_t p_92093_1_);
    static double getBlastDamageReduction(EntityLivingBase *entityLivingBaseIn, double damage);

  private:
    Type protectionType;
};
