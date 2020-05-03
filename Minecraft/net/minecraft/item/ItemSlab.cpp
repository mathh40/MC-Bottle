#include "ItemSlab.h"

#include "ItemStack.h"
#include "SoundCategory.h"
#include "Util.h"
#include "properties/IProperty.h"
#include "state/IBlockState.h"

ItemSlab::ItemSlab(Block *block, BlockSlab *singleSlab, BlockSlab *doubleSlab)
    :ItemBlock(block),singleSlab(singleSlab),doubleSlab(doubleSlab)
{
    setMaxDamage(0);
    setHasSubtypes(true);
}

int32_t ItemSlab::getMetadata(int32_t damage) const
{
    return damage;
}

std::string ItemSlab::getTranslationKey(ItemStack stack) const
{
    return singleSlab->getTranslationKey(stack.getMetadata());
}

EnumActionResult ItemSlab::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    ItemStack itemstack = player->getHeldItem(hand);
    if (!itemstack.isEmpty() && player->canPlayerEdit(pos.offset(facing), facing, itemstack)) 
    {
        Comparable comparable = singleSlab->getTypeForItem(itemstack);
        IBlockState* iblockstate = worldIn->getBlockState(pos);
        if (iblockstate->getBlock() == singleSlab) 
        {
            IProperty* iproperty = singleSlab->getVariantProperty();
            Comparable comparable1 = iblockstate->getValue(iproperty);
            BlockSlab::EnumBlockHalf blockslab$enumblockhalf = (BlockSlab::EnumBlockHalf)iblockstate->getValue(BlockSlab::HALF);
            if ((facing == EnumFacing::UP && blockslab$enumblockhalf == BlockSlab::EnumBlockHalf::BOTTOM || facing == EnumFacing::DOWN && blockslab$enumblockhalf == BlockSlab::EnumBlockHalf::TOP) && comparable1 == comparable) 
            {
                IBlockState* iblockstate1 = makeState(iproperty, comparable1);
                AxisAlignedBB axisalignedbb = iblockstate1->getCollisionBoundingBox(worldIn, pos);
                if (axisalignedbb != Block::NULL_AABB && worldIn->checkNoEntityCollision(axisalignedbb.offset(pos)) && worldIn->setBlockState(pos, iblockstate1, 11)) 
                {
                    SoundType soundtype = doubleSlab->getSoundType();
                    worldIn->playSound(player, pos, soundtype.getPlaceSound(), SoundCategory::BLOCKS, (soundtype.getVolume() + 1.0F) / 2.0F, soundtype.getPitch() * 0.8F);
                    itemstack.shrink(1);
                    if (Util::instanceof<EntityPlayerMP>(player)) 
                    {
                        CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP*)player, pos, itemstack);
                    }
                }

                return EnumActionResult::SUCCESS;
            }
        }

        return tryPlace(player, itemstack, worldIn, pos.offset(facing), comparable) ? EnumActionResult::SUCCESS : ItemBlock::onItemUse(player, worldIn, pos, hand, facing, hitX, hitY, hitZ);
    }
    else 
    {
        return EnumActionResult::FAIL;
    }
}

bool ItemSlab::canPlaceBlockOnSide(World *worldIn, BlockPos pos, EnumFacing side, EntityPlayer *player, ItemStack stack)
{
    BlockPos blockpos = pos;
    IProperty* iproperty = singleSlab->getVariantProperty();
    Comparable comparable = singleSlab->getTypeForItem(stack);
    IBlockState* iblockstate = worldIn->getBlockState(pos);
    if (iblockstate->getBlock() == singleSlab) 
    {
        bool flag = iblockstate->getValue(BlockSlab::HALF) == BlockSlab::EnumBlockHalf::TOP;
        if ((side == EnumFacing::UP && !flag || side == EnumFacing::DOWN && flag) && comparable == iblockstate->getValue(iproperty)) {
            return true;
        }
    }

    pos = pos.offset(side);
    IBlockState* iblockstate1 = worldIn->getBlockState(pos);
    return iblockstate1->getBlock() == singleSlab && comparable == iblockstate1->getValue(iproperty) ? true : ItemBlock::canPlaceBlockOnSide(worldIn, blockpos, side, player, stack);
}

bool ItemSlab::tryPlace(EntityPlayer *player, ItemStack stack, World *worldIn, BlockPos pos,
    Comparable itemSlabType)
{
    IBlockState* iblockstate = worldIn->getBlockState(pos);
    if (iblockstate->getBlock() == singleSlab) 
    {
        Comparable comparable = iblockstate->getValue(singleSlab->getVariantProperty());
        if (comparable == itemSlabType) 
        {
            IBlockState* iblockstate1 = makeState(singleSlab->getVariantProperty(), comparable);
            AxisAlignedBB axisalignedbb = iblockstate1->getCollisionBoundingBox(worldIn, pos);
            if (axisalignedbb != Block::NULL_AABB && worldIn->checkNoEntityCollision(axisalignedbb.offset(pos)) && worldIn->setBlockState(pos, iblockstate1, 11)) 
            {
                SoundType soundtype = doubleSlab->getSoundType();
                worldIn->playSound(player, pos, soundtype.getPlaceSound(), SoundCategory::BLOCKS, (soundtype.getVolume() + 1.0F) / 2.0F, soundtype.getPitch() * 0.8F);
                stack.shrink(1);
            }

            return true;
        }
    }

    return false;
}

IBlockState* ItemSlab::makeState(IProperty* p_185055_1_, Comparable p_185055_2_)
{
    return doubleSlab->getDefaultState()->withProperty(p_185055_1_, p_185055_2_);
}