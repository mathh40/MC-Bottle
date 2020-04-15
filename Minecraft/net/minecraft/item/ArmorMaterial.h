#pragma once
#include "SoundEvent.h"
class Item;
class ArmorMaterial
{
public:
    static ArmorMaterial LEATHER;
    static ArmorMaterial CHAIN;
    static ArmorMaterial IRON;
    static ArmorMaterial GOLD;
    static ArmorMaterial DIAMOND;

    ArmorMaterial(std::string nameIn, int32_t maxDamageFactorIn, const std::vector<int32_t>& damageReductionAmountArrayIn, int32_t enchantabilityIn, SoundEvent soundEventIn, float toughnessIn);
    int32_t getDurability(EntityEquipmentSlot armorType);
    int32_t getDamageReductionAmount(EntityEquipmentSlot armorType);
    int32_t getEnchantability() const;
    SoundEvent getSoundEvent() const;
    Item* getRepairItem();
    std::string getName() const;
    float getToughness() const;

private:

    static std::vector<int32_t> MAX_DAMAGE_ARRAY;
    std::string name;
    int32_t maxDamageFactor;
    std::vector<int32_t> damageReductionAmountArray;
    int32_t enchantability;
    SoundEvent soundEvent;
    float toughness;
};