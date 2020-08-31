#pragma once
#include "Item.h"

class CreativeTabs;

class ItemMonsterPlacer :public Item
{
public:
    ItemMonsterPlacer();
    std::string getItemStackDisplayName(ItemStack stack) const override;
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
    void applyItemEntityDataToEntity(World* entityWorld, EntityPlayer* player, ItemStack stack, Entity* targetEntity);
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
    Entity* spawnCreature(World* worldIn, ResourceLocation entityID, double x, double y, double z);
    void getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items);
    void applyEntityIdToItemStack(ItemStack stack, ResourceLocation entityId);
    static ResourceLocation getNamedIdFrom(ItemStack stack);
protected:
    double getYOffset(World* p_190909_1_, BlockPos p_190909_2_);
private:
};
