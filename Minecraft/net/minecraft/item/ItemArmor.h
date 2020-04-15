#pragma once
#include <vector>

#include "ArmorMaterial.h"
#include "Item.h"

class IBehaviorDispenseItem;
class IBlockSource;
class ItemArmor :public Item
{
public:
    static ItemStack dispenseArmor(IBlockSource* blockSource, ItemStack stack);
    ItemArmor(ArmorMaterial materialIn, int32_t renderIndexIn, EntityEquipmentSlot equipmentSlotIn);
    EntityEquipmentSlot getEquipmentSlot();
    int32_t getItemEnchantability() override;
    ArmorMaterial getArmorMaterial() const;
    bool hasColor(ItemStack stack);
    int32_t getColor(ItemStack stack);
    void removeColor(ItemStack stack);
    void setColor(ItemStack stack, int32_t color);
    bool getIsRepairable(ItemStack toRepair, ItemStack repair) override;
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
    std::unordered_multimap<std::string,AttributeModifier> getItemAttributeModifiers(EntityEquipmentSlot equipmentSlot);

    static constexpr std::array<const char *, 4> EMPTY_SLOT_NAMES = {"minecraft:items/empty_armor_slot_boots",
                                                                     "minecraft:items/empty_armor_slot_leggings",
                                                                     "minecraft:items/empty_armor_slot_chestplate",
                                                                     "minecraft:items/empty_armor_slot_helmet"};
    static const IBehaviorDispenseItem* DISPENSER_BEHAVIOR;
    EntityEquipmentSlot armorType;
    int32_t damageReduceAmount;
    float toughness;
    int32_t renderIndex;
private:
    static std::vector<xg::Guid> ARMOR_MODIFIERS;
    ArmorMaterial material;
};