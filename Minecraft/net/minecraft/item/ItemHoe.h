#pragma once
#include "Item.h"
#include "ToolMaterial.h"

class ItemHoe :public Item
{
public:
    explicit ItemHoe(const ToolMaterial& material);
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
    bool hitEntity(ItemStack stack, EntityLivingBase* target, EntityLivingBase* attacker) override;
    bool isFull3D() const override;
    std::string getMaterialName() const;
    std::unordered_multimap<std::string,AttributeModifier> getItemAttributeModifiers(EntityEquipmentSlot equipmentSlot);

protected:
    void setBlock(ItemStack stack, EntityPlayer* player, World* worldIn, BlockPos pos, IBlockState* state);

    ToolMaterial toolMaterial;
private:
    float speed;
};
