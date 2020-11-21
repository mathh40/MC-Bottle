#include"EnumEnchantmentType.h"

#include "Util.h"
#include "../item/ItemArmor.h"
#include "../item/ItemBlock.h"
#include "../item/ItemBow.h"
#include "../item/ItemElytra.h"
#include "../item/ItemFishingRod.h"
#include "../item/ItemSkull.h"
#include "../item/ItemSword.h"
#include "../item/ItemTool.h"

EnumEnchantmentType EnumEnchantmentType::ALL = EnumEnchantmentType(Type::ALL);
EnumEnchantmentType EnumEnchantmentType::ARMOR= EnumEnchantmentType(Type::ARMOR);
EnumEnchantmentType EnumEnchantmentType::ARMOR_FEET = EnumEnchantmentType(Type::ARMOR_FEET);
EnumEnchantmentType EnumEnchantmentType::ARMOR_LEGS = EnumEnchantmentType(Type::ARMOR_LEGS);
EnumEnchantmentType EnumEnchantmentType::ARMOR_CHEST = EnumEnchantmentType(Type::ARMOR_CHEST);
EnumEnchantmentType EnumEnchantmentType::ARMOR_HEAD = EnumEnchantmentType(Type::ARMOR_HEAD);
EnumEnchantmentType EnumEnchantmentType::WEAPON = EnumEnchantmentType(Type::WEAPON);
EnumEnchantmentType EnumEnchantmentType::DIGGER = EnumEnchantmentType(Type::DIGGER);
EnumEnchantmentType EnumEnchantmentType::FISHING_ROD = EnumEnchantmentType(Type::FISHING_ROD);
EnumEnchantmentType EnumEnchantmentType::BREAKABLE = EnumEnchantmentType(Type::BREAKABLE);
EnumEnchantmentType EnumEnchantmentType::BOW = EnumEnchantmentType(Type::BOW);
EnumEnchantmentType EnumEnchantmentType::WEARABLE = EnumEnchantmentType(Type::WEARABLE);

EnumEnchantmentType::EnumEnchantmentType(Type type)
:type(type){
    val.emplace_back(this);
}

bool EnumEnchantmentType::canEnchantItem(Item *itemIn) {
    switch (type) {

    case Type::ALL:
        for(auto* enumenchantmenttype: val){
            if (enumenchantmenttype->type != ALL.type && enumenchantmenttype->canEnchantItem(itemIn)) {
               return true;
            }
         }

         return false;
    case Type::ARMOR:
        return Util::instanceof<ItemArmor>(itemIn);
    case Type::ARMOR_FEET:
        return Util::instanceof<ItemArmor>(itemIn) && ((ItemArmor*)itemIn)->armorType == EntityEquipmentSlot::FEET;
    case Type::ARMOR_LEGS:
        return Util::instanceof<ItemArmor>(itemIn) && ((ItemArmor*)itemIn)->armorType == EntityEquipmentSlot::LEGS;
    case Type::ARMOR_CHEST:
        return Util::instanceof<ItemArmor>(itemIn) && ((ItemArmor*)itemIn)->armorType == EntityEquipmentSlot::CHEST;
    case Type::ARMOR_HEAD:
        return Util::instanceof<ItemArmor>(itemIn) && ((ItemArmor*)itemIn)->armorType == EntityEquipmentSlot::HEAD;
    case Type::WEAPON:
        return Util::instanceof<ItemSword>(itemIn);
    case Type::DIGGER:
        return Util::instanceof<ItemTool>(itemIn);
    case Type::FISHING_ROD:
        return Util::instanceof<ItemFishingRod>(itemIn);
    case Type::BREAKABLE:
        return itemIn->isDamageable();
    case Type::BOW:
        return Util::instanceof<ItemBow>(itemIn);
    case Type::WEARABLE:
        bool flag = Util::instanceof<ItemBlock>(itemIn) && Util::instanceof<BlockPumpkin>(((ItemBlock*)itemIn)->getBlock());
         return 
        Util::instanceof< ItemArmor>(itemIn) || 
        Util::instanceof< ItemElytra>(itemIn) || 
        Util::instanceof< ItemSkull>(itemIn) || flag;
    }
    return false;
}

std::vector<EnumEnchantmentType *> EnumEnchantmentType::value() {
    return val;
}

bool operator==(const EnumEnchantmentType &lhs, const EnumEnchantmentType &rhs) {
    return lhs.type == rhs.type;
}

bool operator!=(const EnumEnchantmentType &lhs, const EnumEnchantmentType &rhs) {
    return !(lhs == rhs);
}
