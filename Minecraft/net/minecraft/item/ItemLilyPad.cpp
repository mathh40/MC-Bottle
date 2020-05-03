#include "ItemLilyPad.h"

#include "ItemStack.h"
#include "SoundCategory.h"
#include "../block/state/IBlockState.h"
#include "../stats/StatList.h"
#include "../world/World.h"

ItemLilyPad::ItemLilyPad(Block *block)
    :ItemColored(block,false)
{
}

ActionResult ItemLilyPad::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    RayTraceResult raytraceresult = rayTrace(worldIn, playerIn, true);
    if (raytraceresult == nullptr)
    {
        return ActionResult(EnumActionResult::PASS, itemstack);
    }
    else 
    {
        if (raytraceresult.typeOfHit == RayTraceResult::Type::BLOCK) 
        {
            BlockPos blockpos = raytraceresult.getBlockPos();
            if (!worldIn->isBlockModifiable(playerIn, blockpos) || !playerIn->canPlayerEdit(blockpos.offset(raytraceresult.sideHit), raytraceresult.sideHit, itemstack)) 
            {
                return ActionResult(EnumActionResult::FAIL, itemstack);
            }

            BlockPos blockpos1 = blockpos.up();
            IBlockState* iblockstate = worldIn->getBlockState(blockpos);
            if (iblockstate->getMaterial() == Material::WATER && iblockstate->getValue(BlockLiquid::LEVEL) == 0 && worldIn->isAirBlock(blockpos1)) 
            {
                worldIn->setBlockState(blockpos1, Blocks.WATERLILY.getDefaultState(), 11);
                if (Util::instanceof<EntityPlayerMP>(playerIn)) 
                {
                    CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP)playerIn, blockpos1, itemstack);
                }

                if (!playerIn->capabilities.isCreativeMode) 
                {
                    itemstack.shrink(1);
                }

                playerIn->addStat(StatList::getObjectUseStats(this));
                worldIn->playSound(playerIn, blockpos, SoundEvents::BLOCK_WATERLILY_PLACE, SoundCategory::BLOCKS, 1.0F, 1.0F);
                return ActionResult(EnumActionResult::SUCCESS, itemstack);
            }
        }

        return ActionResult(EnumActionResult::FAIL, itemstack);
    }
}
