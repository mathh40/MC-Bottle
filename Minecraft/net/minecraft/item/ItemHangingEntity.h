#pragma once
#include "Item.h"

#include <typeindex>

class ItemHangingEntity : public Item
{
  public:
    ItemHangingEntity(const std::type_index &entityClass);
    EnumActionResult onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
                               float hitX, float hitY, float hitZ) override;

  private:
    EntityHanging *createEntity(World *worldIn, BlockPos pos, EnumFacing clickedSide);

    std::type_index hangingEntityClass;
};
