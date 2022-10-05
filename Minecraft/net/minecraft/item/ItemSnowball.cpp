#include "ItemSnowball.h"

#include "../stats/StatList.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "biome/Biome.h"

ItemSnowball::ItemSnowball()
{
    maxStackSize = 16;
    setCreativeTab(CreativeTabs::MISC);
}

ActionResult ItemSnowball::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    if (!playerIn->capabilities.isCreativeMode)
    {
        itemstack.shrink(1);
    }

    worldIn->playSound(nullptr, playerIn->posX, playerIn->posY, playerIn->posZ, SoundEvents::ENTITY_SNOWBALL_THROW,
                       SoundCategory::NEUTRAL, 0.5F, 0.4F / (MathHelper::nextFloat(itemRand) * 0.4F + 0.8F));
    if (!worldIn->isRemote)
    {
        EntitySnowball *entitysnowball = new EntitySnowball(worldIn, playerIn);
        entitysnowball->shoot(playerIn, playerIn->rotationPitch, playerIn->rotationYaw, 0.0F, 1.5F, 1.0F);
        worldIn->spawnEntity(entitysnowball);
    }

    playerIn->addStat(StatList::getObjectUseStats(this));
    return ActionResult(EnumActionResult::SUCCESS, itemstack);
}
