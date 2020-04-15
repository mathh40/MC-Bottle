#pragma once
#include "Item.h"

class EntityArmorStand;
class ItemArmorStand :public Item
{
public:
    ItemArmorStand();
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
private:
    void applyRandomRotations(EntityArmorStand* armorStand, pcg32& rand);
};