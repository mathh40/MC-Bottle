#include "ItemBoat.h"

#include "ItemStack.h"
#include "../stats/StatList.h"
#include "../world/biome/Biome.h"

ItemBoat::ItemBoat(EntityBoat::Type typeIn)
    :type(typeIn)
{
    maxStackSize = 1;
    setCreativeTab(CreativeTabs::TRANSPORTATION);
    setTranslationKey("boat." + typeIn.getName());
}

ActionResult ItemBoat::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    float f = 1.0F;
    float f1 = playerIn->prevRotationPitch + (playerIn->rotationPitch - playerIn->prevRotationPitch) * 1.0F;
    float f2 = playerIn->prevRotationYaw + (playerIn->rotationYaw - playerIn->prevRotationYaw) * 1.0F;
    double d0 = playerIn->prevPosX + (playerIn->posX - playerIn->prevPosX) * 1.0;
    double d1 = playerIn->prevPosY + (playerIn->posY - playerIn->prevPosY) * 1.0 + (double)playerIn->getEyeHeight();
    double d2 = playerIn->prevPosZ + (playerIn->posZ - playerIn->prevPosZ) * 1.0;
    Vec3d vec3d = Vec3d(d0, d1, d2);
    float f3 = MathHelper::cos(-f2 * 0.017453292F - 3.1415927F);
    float f4 = MathHelper::sin(-f2 * 0.017453292F - 3.1415927F);
    float f5 = -MathHelper::cos(-f1 * 0.017453292F);
    float f6 = MathHelper::sin(-f1 * 0.017453292F);
    float f7 = f4 * f5;
    float f8 = f3 * f5;
    double d3 = 5.0;
    Vec3d vec3d1 = vec3d.add((double)f7 * 5.0, (double)f6 * 5.0, (double)f8 * 5.0);
    RayTraceResult raytraceresult = worldIn->rayTraceBlocks(vec3d, vec3d1, true);
    if (raytraceresult == nullptr) 
    {
        return new ActionResult(EnumActionResult.PASS, itemstack);
    }
    else 
    {
        Vec3d vec3d2 = playerIn->getLook(1.0F);
        bool flag = false;
        auto list = worldIn->getEntitiesWithinAABBExcludingEntity(playerIn, playerIn->getEntityBoundingBox().expand(vec3d2.getx() * 5.0, vec3d2.gety() * 5.0, vec3d2.getz() * 5.0).grow(1.0));

        for(auto i = 0; i < list.size(); ++i) 
        {
            Entity* entity = list.get(i);
            if (entity->canBeCollidedWith()) 
            {
                AxisAlignedBB axisalignedbb = entity->getEntityBoundingBox().grow((double)entity->getCollisionBorderSize());
                if (axisalignedbb.contains(vec3d)) 
                {
                    flag = true;
                }
            }
        }

        if (flag) 
        {
            return ActionResult(EnumActionResult::PASS, itemstack);
        }
        else if (raytraceresult.typeOfHit != RayTraceResult::Type::BLOCK) 
        {
            return ActionResult(EnumActionResult::PASS, itemstack);
        }
        else 
        {
            Block* block = worldIn->getBlockState(raytraceresult.getBlockPos()).getBlock();
            bool flag1 = block == Blocks::WATER || block == Blocks::FLOWING_WATER;
            EntityBoat* entityboat = new EntityBoat(worldIn, raytraceresult.hitVec.x, flag1 ? raytraceresult.hitVec.y - 0.12D : raytraceresult.hitVec.y, raytraceresult.hitVec.z);
            entityboat->setBoatType(type);
            entityboat->rotationYaw = playerIn->rotationYaw;
            if (!worldIn->getCollisionBoxes(entityboat, entityboat.getEntityBoundingBox().grow(-0.1)).isEmpty()) 
            {
                return ActionResult(EnumActionResult::FAIL, itemstack);
            }
            else 
            {
                if (!worldIn->isRemote) 
                {
                    worldIn->spawnEntity(entityboat);
                }

                if (!playerIn->capabilities.isCreativeMode) 
                {
                    itemstack.shrink(1);
                }

                playerIn->addStat(StatList::getObjectUseStats(this));
                return ActionResult(EnumActionResult::SUCCESS, itemstack);
            }
        }
    }
}
