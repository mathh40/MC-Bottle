#include "ItemEndCrystal.h"

#include "../world/WorldProviderEnd.h"
#include "../world/end/DragonFightManager.h"
#include "ItemStack.h"

ItemEndCrystal::ItemEndCrystal()
{
    setTranslationKey("end_crystal");
    setCreativeTab(CreativeTabs::DECORATIONS);
}

EnumActionResult ItemEndCrystal::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                           EnumFacing facing, float hitX, float hitY, float hitZ)
{
    IBlockState *iblockstate = worldIn->getBlockState(pos);
    if (iblockstate->getBlock() != Blocks.OBSIDIAN && iblockstate->getBlock() != Blocks.BEDROCK)
    {
        return EnumActionResult::FAIL;
    }
    else
    {
        BlockPos blockpos   = pos.up();
        ItemStack itemstack = player->getHeldItem(hand);
        if (!player->canPlayerEdit(blockpos, facing, itemstack))
        {
            return EnumActionResult::FAIL;
        }
        else
        {
            BlockPos blockpos1 = blockpos.up();
            bool flag          = !worldIn->isAirBlock(blockpos) &&
                        !worldIn->getBlockState(blockpos).getBlock().isReplaceable(worldIn, blockpos);
            flag |= !worldIn->isAirBlock(blockpos1) &&
                    !worldIn->getBlockState(blockpos1).getBlock().isReplaceable(worldIn, blockpos1);
            if (flag)
            {
                return EnumActionResult::FAIL;
            }
            else
            {
                double d0 = (double)blockpos.getx();
                double d1 = (double)blockpos.gety();
                double d2 = (double)blockpos.getz();
                auto list = worldIn->getEntitiesWithinAABBExcludingEntity(
                    nullptr, AxisAlignedBB(d0, d1, d2, d0 + 1.0, d1 + 2.0, d2 + 1.0));
                if (!list.isEmpty())
                {
                    return EnumActionResult::FAIL;
                }
                else
                {
                    if (!worldIn->isRemote)
                    {
                        EntityEnderCrystal entityendercrystal =
                            EntityEnderCrystal(worldIn, (double)((float)pos.getx() + 0.5F), (double)(pos.gety() + 1),
                                               (double)((float)pos.getz() + 0.5F));
                        entityendercrystal.setShowBottom(false);
                        worldIn->spawnEntity(entityendercrystal);
                        if (Util:: instanceof <WorldProviderEnd>(worldIn->provider))
                        {
                            DragonFightManager dragonfightmanager =
                                ((WorldProviderEnd *)worldIn->provider)->getDragonFightManager();
                            dragonfightmanager.respawnDragon();
                        }
                    }

                    itemstack.shrink(1);
                    return EnumActionResult::SUCCESS;
                }
            }
        }
    }
}

bool ItemEndCrystal::hasEffect(ItemStack stack)
{
    return true;
}
