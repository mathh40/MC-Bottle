#include "ItemSnow.h"

#include "ItemStack.h"
#include "SoundCategory.h"
#include "SoundType.h"
#include "Util.h"
#include "math/AxisAlignedBB.h"

ItemSnow::ItemSnow(Block *block) : ItemBlock(block)
{
    setMaxDamage(0);
}

EnumActionResult ItemSnow::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                     EnumFacing facing, float hitX, float hitY, float hitZ)
{
    ItemStack itemstack = player.getHeldItem(hand);
    if (!itemstack.isEmpty() && player.canPlayerEdit(pos, facing, itemstack))
    {
        IBlockState *iblockstate = worldIn->getBlockState(pos);
        Block *block             = iblockstate->getBlock();
        BlockPos blockpos        = pos;
        if ((facing != EnumFacing::UP || block != block) && !block->isReplaceable(worldIn, pos))
        {
            blockpos    = pos.offset(facing);
            iblockstate = worldIn->getBlockState(blockpos);
            block       = iblockstate->getBlock();
        }

        if (block == block)
        {
            auto i = iblockstate->getValue(BlockSnow::LAYERS);
            if (i < 8)
            {
                IBlockState *iblockstate1   = iblockstate->withProperty(BlockSnow::LAYERS, i + 1);
                AxisAlignedBB axisalignedbb = iblockstate1.getCollisionBoundingBox(worldIn, blockpos);
                if (axisalignedbb != Block::NULL_AABB &&
                    worldIn->checkNoEntityCollision(axisalignedbb.offset(blockpos)) &&
                    worldIn->setBlockState(blockpos, iblockstate1, 10))
                {
                    SoundType soundtype = block->getSoundType();
                    worldIn->playSound(player, blockpos, soundtype.getPlaceSound(), SoundCategory::BLOCKS,
                                       (soundtype.getVolume() + 1.0F) / 2.0F, soundtype.getPitch() * 0.8F);
                    if (Util:: instanceof <EntityPlayerMP>(player))
                    {
                        CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP *)player, pos, itemstack);
                    }

                    itemstack.shrink(1);
                    return EnumActionResult::SUCCESS;
                }
            }
        }

        return ItemBlock::onItemUse(player, worldIn, pos, hand, facing, hitX, hitY, hitZ);
    }
    else
    {
        return EnumActionResult::FAIL;
    }
}

int32_t ItemSnow::getMetadata(int32_t damage) const
{
    return damage;
}
