#pragma once
#include "../inventory/EntityEquipmentSlot.h"
#include "EnumEnchantmentType.h"
#include "registry/RegistryNamespaced.h"

#include <span>

enum class EnumCreatureAttribute;

namespace DamageSource
{
class DamageSource;
}

class ItemStack;
class EntityLivingBase;
class ResourceLocation;
class Entity;

class Enchantment
{
  public:
    enum class Rarity
    {
        COMMON    = 10,
        UNCOMMON  = 5,
        RARE      = 2,
        VERY_RARE = 1
    };

    static Enchantment *getEnchantmentByID(int32_t id);
    static int32_t getEnchantmentID(Enchantment *enchantmentIn);
    static Enchantment *getEnchantmentByLocation(std::string_view location);
    std::vector<ItemStack> &getEntityEquipment(EntityLivingBase *entityIn) const;
    Enchantment::Rarity getRarity() const;
    virtual int32_t getMinLevel() const;
    virtual int32_t getMaxLevel() const;
    virtual int32_t getMinEnchantability(int32_t enchantmentLevel) const;
    virtual int32_t getMaxEnchantability(int32_t enchantmentLevel) const;
    virtual int32_t calcModifierDamage(int32_t level, const DamageSource::DamageSource &source) const;
    float calcDamageByCreature(int32_t level, const EnumCreatureAttribute &creatureType) const;
    bool isCompatibleWith(const Enchantment &p_191560_1_) const;
    Enchantment &setName(std::string_view enchName);
    virtual std::string getName() const;
    std::string getTranslatedName(int32_t level) const;
    virtual bool canApply(const ItemStack &stack) const;
    void onEntityDamaged(EntityLivingBase *user, Entity *target, int32_t level) const;
    virtual void onUserHurt(EntityLivingBase *user, Entity *attacker, int32_t level) const;
    virtual bool isTreasureEnchantment() const;
    virtual bool isCurse() const;
    static void registerEnchantments();
    friend bool operator==(const Enchantment &lhs, const Enchantment &rhs);
    friend bool operator!=(const Enchantment &lhs, const Enchantment &rhs);

    static RegistryNamespaced<ResourceLocation, Enchantment *> REGISTRY;
    EnumEnchantmentType type;

  protected:
    std::string name;
    Enchantment(Enchantment::Rarity rarityIn, EnumEnchantmentType typeIn, std::span<const EntityEquipmentSlot> slots);
    virtual bool canApplyTogether(const Enchantment &ench) const;

  private:
    std::vector<EntityEquipmentSlot> applicableEquipmentTypes;
    Enchantment::Rarity rarity;
};
