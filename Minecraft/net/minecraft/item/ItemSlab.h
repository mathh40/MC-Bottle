#pragma once
#include "ItemBlock.h"

class IProperty;

namespace ObjectWithConversions
{
struct Object;
}

class BlockSlab;
class ItemSlab : public ItemBlock
{
  public:
    ItemSlab(Block *block, BlockSlab *singleSlab, BlockSlab *doubleSlab);
    int32_t getMetadata(int32_t damage) const override;
    std::string getTranslationKey(ItemStack stack) const override;
    EnumActionResult onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
                               float hitX, float hitY, float hitZ) override;
    bool canPlaceBlockOnSide(World *worldIn, BlockPos pos, EnumFacing side, EntityPlayer *player,
                             ItemStack stack) override;

  protected:
    IBlockState *makeState(IProperty *p_185055_1_, Comparable p_185055_2_);

  private:
    bool tryPlace(EntityPlayer *player, ItemStack stack, World *worldIn, BlockPos pos, Comparable itemSlabType);

    BlockSlab *singleSlab;
    BlockSlab *doubleSlab;
};
