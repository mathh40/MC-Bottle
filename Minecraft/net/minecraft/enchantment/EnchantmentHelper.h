#pragma once
#include "DamageSource.h"
#include "../../../../pcg-cpp/pcg_random.hpp"
#include "../entity/EnumCreatureAttribute.h"

#include <cstdint>
#include <span>

namespace EnchantmenRandom {
    class EnchantmentData;
}

namespace WeightedRandom {
    struct WeightedItem;
}

class ItemStack;
class Enchantment;
namespace EnchantmentHelper {
    class IModifier {
    public:
        virtual ~IModifier() = default;
        virtual void calculateModifier(const Enchantment& var1, int32_t var2) = 0;
   };

    class ModifierLiving :public IModifier {
    public:
        float livingModifier;
      EnumCreatureAttribute entityLiving;

        void calculateModifier(const Enchantment& enchantmentIn, int32_t enchantmentLevel) override;
   };

   class ModifierDamage :public IModifier {
   public:
       int32_t damageModifier;
       std::optional<DamageSource::DamageSource> source;

       void calculateModifier(const Enchantment& enchantmentIn, int32_t enchantmentLevel) override;
   };

   class HurtIterator :public IModifier {
   public: EntityLivingBase* user;
      Entity* attacker;

      void calculateModifier(const Enchantment& enchantmentIn, int32_t enchantmentLevel) override;
   };

   class DamageIterator :public IModifier {
   public: EntityLivingBase* user;
      Entity* target;

      void calculateModifier(const Enchantment& enchantmentIn, int32_t enchantmentLevel) override;
   };

    int32_t getEnchantmentLevel(const Enchantment& enchID, const ItemStack& stack);
    std::unordered_map<Enchantment*,int16_t> getEnchantments(const ItemStack& stack);
    void setEnchantments(std::unordered_map<Enchantment*,int16_t> enchMap, ItemStack& stack);
    int32_t getEnchantmentModifierDamage(std::span<ItemStack> stacks, DamageSource::DamageSource source);
    float getModifierForCreature(const ItemStack& stack, const EnumCreatureAttribute& creatureAttribute);
    float getSweepingDamageRatio(EntityLivingBase* p_191527_0_);
    void applyThornEnchantments(EntityLivingBase* p_151384_0_, Entity* p_151384_1_);
    void applyArthropodEnchantments(EntityLivingBase* p_151385_0_, Entity* p_151385_1_);
    int32_t getMaxEnchantmentLevel(const Enchantment& p_185284_0_, EntityLivingBase* p_185284_1_);
    int32_t getKnockbackModifier(EntityLivingBase* player);
    int32_t getFireAspectModifier(EntityLivingBase* player);
    int32_t getRespirationModifier(EntityLivingBase *p_185292_0_);
    int32_t getDepthStriderModifier(EntityLivingBase *p_185294_0_);
    int32_t getEfficiencyModifier(EntityLivingBase *p_185293_0_);
    int32_t getFishingLuckBonus(const ItemStack &p_191529_0_);
    int32_t getFishingSpeedBonus(const ItemStack &p_191528_0_);
    int32_t getLootingModifier(EntityLivingBase *p_185283_0_);
    bool getAquaAffinityModifier(EntityLivingBase *p_185287_0_);
    bool hasFrostWalkerEnchantment(EntityLivingBase *player);
    bool hasBindingCurse(const ItemStack &p_190938_0_);
    bool hasVanishingCurse(const ItemStack &p_190939_0_);
    ItemStack getEnchantedItem(const Enchantment& p_92099_0_, EntityLivingBase* p_92099_1_);
    int32_t calcItemStackEnchantability(pcg32& rand, int32_t enchantNum, int32_t power, ItemStack stack);
    ItemStack addRandomEnchantment(pcg32& random, ItemStack& stack, int32_t level, bool allowTreasure);
    std::vector<EnchantmenRandom::EnchantmentData> buildEnchantmentList(pcg32& randomIn, ItemStack& itemStackIn, int32_t level, bool allowTreasure);
    void removeIncompatible(std::vector<EnchantmenRandom::EnchantmentData> &p_185282_0_, const EnchantmenRandom::EnchantmentData &p_185282_1_);
    std::vector<EnchantmenRandom::EnchantmentData> getEnchantmentDatas(int32_t p_185291_0_, ItemStack& p_185291_1_, bool allowTreasure);
}
