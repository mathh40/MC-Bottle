#pragma once
#include "Item.h"
#include "ToolMaterial.h"

class ItemSword :public Item
{
public:
    explicit ItemSword(ToolMaterial material);
    float getAttackDamage() const;
    float getDestroySpeed(ItemStack stack, IBlockState* state) override;
    bool hitEntity(ItemStack stack, EntityLivingBase* target, EntityLivingBase* attacker) override;
    bool onBlockDestroyed(ItemStack stack, World* worldIn, IBlockState* state, BlockPos pos, EntityLivingBase* entityLiving) override;
    bool isFull3D() const override;
    bool canHarvestBlock(IBlockState* blockIn) override;
    int32_t getItemEnchantability() override;
    std::string getToolMaterialName() const;
    bool getIsRepairable(ItemStack toRepair, ItemStack repair) override;
    std::unordered_multimap<std::string,AttributeModifier> getItemAttributeModifiers(EntityEquipmentSlot equipmentSlot) override;

private:
    float attackDamage;
    ToolMaterial material;
};
