#include "ItemEnderEye.h"

#include "../stats/StatList.h"
#include "../world/WorldServer.h"
#include "EnumFacing.h"
#include "EnumParticleTypes.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "spdlog/sinks/null_sink.h"

ItemEnderEye::ItemEnderEye()
{
    setCreativeTab(CreativeTabs::MISC);
}

EnumActionResult ItemEnderEye::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                         EnumFacing facing, float hitX, float hitY, float hitZ)
{
    IBlockState *iblockstate = worldIn->getBlockState(pos);
    ItemStack itemstack      = player->getHeldItem(hand);
    if (player->canPlayerEdit(pos.offset(facing), facing, itemstack) &&
        iblockstate->getBlock() == Blocks::END_PORTAL_FRAME && !iblockstate->getValue(BlockEndPortalFrame::EYE))
    {
        if (worldIn->isRemote)
        {
            return EnumActionResult::SUCCESS;
        }
        else
        {
            worldIn->setBlockState(pos, iblockstate->withProperty(BlockEndPortalFrame::EYE, true), 2);
            worldIn->updateComparatorOutputLevel(pos, Blocks::END_PORTAL_FRAME);
            itemstack.shrink(1);

            for (int i = 0; i < 16; ++i)
            {
                double d0 = (double)((float)pos.getX() + (5.0F + itemRand.nextFloat() * 6.0F) / 16.0F);
                double d1 = (double)((float)pos.getY() + 0.8125F);
                double d2 = (double)((float)pos.getZ() + (5.0F + itemRand.nextFloat() * 6.0F) / 16.0F);
                worldIn->spawnParticle(EnumParticleTypes.SMOKE_NORMAL, d0, d1, d2, 0.0, 0.0, 0.0);
            }

            worldIn->playSound((EntityPlayer)null, pos, SoundEvents::BLOCK_END_PORTAL_FRAME_FILL, SoundCategory::BLOCKS,
                               1.0F, 1.0F);
            BlockPattern::PatternHelper blockpattern$patternhelper =
                BlockEndPortalFrame::getOrCreatePortalShape().match(worldIn, pos);
            if (blockpattern$patternhelper != null)
            {
                BlockPos blockpos = blockpattern$patternhelper.getFrontTopLeft().add(-3, 0, -3);

                for (int j = 0; j < 3; ++j)
                {
                    for (int k = 0; k < 3; ++k)
                    {
                        worldIn->setBlockState(blockpos.add(j, 0, k), Blocks::END_PORTAL.getDefaultState(), 2);
                    }
                }

                worldIn->playBroadcastSound(1038, blockpos.add(1, 0, 1), 0);
            }

            return EnumActionResult::SUCCESS;
        }
    }
    else
    {
        return EnumActionResult::FAIL;
    }
}

ActionResult ItemEnderEye::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack           = playerIn->getHeldItem(handIn);
    RayTraceResult raytraceresult = rayTrace(worldIn, playerIn, false);
    if (raytraceresult != nullptr && raytraceresult.typeOfHit == RayTraceResult::Type::BLOCK &&
        worldIn->getBlockState(raytraceresult.getBlockPos()).getBlock() == Blocks::END_PORTAL_FRAME)
    {
        return ActionResult(EnumActionResult::PASS, itemstack);
    }
    else
    {
        playerIn->setActiveHand(handIn);
        if (!worldIn->isRemote)
        {
            BlockPos blockpos = ((WorldServer *)worldIn)
                                    .getChunkProvider()
                                    .getNearestStructurePos(worldIn, "Stronghold", new BlockPos(playerIn), false);
            if (blockpos != nullptr)
            {
                EntityEnderEye *entityendereye = new EntityEnderEye(
                    worldIn, playerIn->posX, playerIn->posY + (double)(playerIn->height / 2.0F), playerIn->posZ);
                entityendereye.moveTowards(blockpos);
                worldIn->spawnEntity(entityendereye);
                if (Util:: instanceof <EntityPlayerMP>(playerIn))
                {
                    CriteriaTriggers;
                    ;
                    USED_ENDER_EYE::trigger((EntityPlayerMP *)playerIn, blockpos);
                }

                worldIn->playSound(nullptr, playerIn->posX, playerIn->posY, playerIn->posZ,
                                   SoundEvents::ENTITY_ENDEREYE_LAUNCH, SoundCategory::NEUTRAL, 0.5F,
                                   0.4F / (MathHelper::nextFloat(itemRand) * 0.4F + 0.8F));
                worldIn->playEvent(nullptr, 1003, BlockPos(playerIn), 0);
                if (!playerIn->capabilities.isCreativeMode)
                {
                    itemstack.shrink(1);
                }

                playerIn->addStat(StatList::getObjectUseStats(this));
                return ActionResult(EnumActionResult::SUCCESS, itemstack);
            }
        }

        return ActionResult(EnumActionResult::SUCCESS, itemstack);
    }
}
