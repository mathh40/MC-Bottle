#include "ItemArmorStand.h"

#include "EnumFacing.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "../world/biome/Biome.h"
#include "math/AxisAlignedBB.h"
#include "math/Rotations.h"

ItemArmorStand::ItemArmorStand()
{
    setCreativeTab(CreativeTabs::DECORATIONS);
}

EnumActionResult ItemArmorStand::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    if (facing == EnumFacing::DOWN) 
    {
        return EnumActionResult::FAIL;
    }
    else 
    {
        bool flag = worldIn->getBlockState(pos).getBlock().isReplaceable(worldIn, pos);
        BlockPos blockpos = flag ? pos : pos.offset(facing);
        ItemStack itemstack = player->getHeldItem(hand);
        if (!player->canPlayerEdit(blockpos, facing, itemstack)) 
        {
            return EnumActionResult::FAIL;
        }
        else 
        {
            BlockPos blockpos1 = blockpos.up();
            bool flag1 = !worldIn->isAirBlock(blockpos) && !worldIn->getBlockState(blockpos).getBlock().isReplaceable(worldIn, blockpos);
            flag1 |= !worldIn->isAirBlock(blockpos1) && !worldIn->getBlockState(blockpos1).getBlock().isReplaceable(worldIn, blockpos1);
            if (flag1) 
            {
                return EnumActionResult::FAIL;
            }
            else 
            {
                double d0 = (double)blockpos.getx();
                double d1 = (double)blockpos.gety();
                double d2 = (double)blockpos.getz();
                auto list = worldIn->getEntitiesWithinAABBExcludingEntity(nullptr, AxisAlignedBB(d0, d1, d2, d0 + 1.0, d1 + 2.0, d2 + 1.0));
                if (!list.isEmpty()) 
                {
                    return EnumActionResult::FAIL;
                }
                else 
                {
                    if (!worldIn->isRemote) 
                    {
                        worldIn->setBlockToAir(blockpos);
                        worldIn->setBlockToAir(blockpos1);
                        EntityArmorStand entityarmorstand = EntityArmorStand(worldIn, d0 + 0.5, d1, d2 + 0.5);
                        float f = (float)MathHelper::floor((MathHelper::wrapDegrees(player->rotationYaw - 180.0F) + 22.5F) / 45.0F) * 45.0F;
                        entityarmorstand.setLocationAndAngles(d0 + 0.5, d1, d2 + 0.5, f, 0.0F);
                        applyRandomRotations(entityarmorstand, worldIn->rand);
                        ItemMonsterPlacer::applyItemEntityDataToEntity(worldIn, player, itemstack, entityarmorstand);
                        worldIn->spawnEntity(entityarmorstand);
                        worldIn->playSound(nullptr, entityarmorstand.posX, entityarmorstand.posY, entityarmorstand.posZ, SoundEvents::ENTITY_ARMORSTAND_PLACE, SoundCategory::BLOCKS, 0.75F, 0.8F);
                    }

                    itemstack.shrink(1);
                    return EnumActionResult::SUCCESS;
                }
            }
        }
    }
}

void ItemArmorStand::applyRandomRotations(EntityArmorStand *armorStand, pcg32 &rand)
{
    Rotations rotations = armorStand->getHeadRotation();
    float f = MathHelper::nextFloat(rand) * 5.0F;
    float f1 = MathHelper::nextFloat(rand) * 20.0F - 10.0F;
    Rotations rotations1 = Rotations(rotations.getX() + f, rotations.getY() + f1, rotations.getZ());
    armorStand->setHeadRotation(rotations1);
    rotations = armorStand->getBodyRotation();
    f = MathHelper::nextFloat(rand) * 10.0F - 5.0F;
    rotations1 = Rotations(rotations.getX(), rotations.getY() + f, rotations.getZ());
    armorStand->setBodyRotation(rotations1);
}
