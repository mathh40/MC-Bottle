#include "ArmorMaterial.h"

#include "ItemArmor.h"

std::vector<int32_t> ArmorMaterial::MAX_DAMAGE_ARRAY = {13, 15, 16, 11};

ArmorMaterial ArmorMaterial::LEATHER("leather", 5, {1, 2, 3, 1}, 15, SoundEvents::ITEM_ARMOR_EQUIP_LEATHER, 0.0F);
ArmorMaterial ArmorMaterial::CHAIN("chainmail", 15, {1, 4, 5, 2}, 12, SoundEvents::ITEM_ARMOR_EQUIP_CHAIN, 0.0F);
ArmorMaterial ArmorMaterial::IRON("iron", 15, {2, 5, 6, 2}, 9, SoundEvents::ITEM_ARMOR_EQUIP_IRON, 0.0F);
ArmorMaterial ArmorMaterial::GOLD("gold", 7, {1, 3, 5, 2}, 25, SoundEvents::ITEM_ARMOR_EQUIP_GOLD, 0.0F);
ArmorMaterial ArmorMaterial::DIAMOND("diamond", 33, {3, 6, 8, 3}, 10, SoundEvents::ITEM_ARMOR_EQUIP_DIAMOND, 2.0F);

ArmorMaterial::ArmorMaterial(std::string nameIn, int32_t maxDamageFactorIn,
                             const std::vector<int32_t> &damageReductionAmountArrayIn, int32_t enchantabilityIn,
                             SoundEvent soundEventIn, float toughnessIn)
    : name(nameIn), maxDamageFactor(maxDamageFactorIn), damageReductionAmountArray(damageReductionAmountArrayIn),
      enchantability(enchantabilityIn), soundEvent(soundEventIn), toughness(toughnessIn)
{
}

int32_t ArmorMaterial::getDurability(EntityEquipmentSlot armorType)
{
    return MAX_DAMAGE_ARRAY[armorType.getIndex()] * maxDamageFactor;
}

int32_t ArmorMaterial::getDamageReductionAmount(EntityEquipmentSlot armorType)
{
    return damageReductionAmountArray[armorType.getIndex()];
}

int32_t ArmorMaterial::getEnchantability() const
{
    return enchantability;
}

SoundEvent ArmorMaterial::getSoundEvent() const
{
    return soundEvent;
}

Item *ArmorMaterial::getRepairItem()
{
    if (this == LEATHER)
    {
        return Items::LEATHER;
    }
    else if (this == CHAIN)
    {
        return Items::IRON_INGOT;
    }
    else if (this == GOLD)
    {
        return Items::GOLD_INGOT;
    }
    else if (this == IRON)
    {
        return Items::IRON_INGOT;
    }
    else
    {
        return this == DIAMOND ? Items::DIAMOND : nullptr;
    }
}

std::string ArmorMaterial::getName() const
{
    return name;
}

float ArmorMaterial::getToughness() const
{
    return toughness;
}
