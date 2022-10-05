#include "ItemEnderPearl.h"

#include "../stats/StatList.h"
#include "../world/gen/ChunkGeneratorFlat.h"
#include "ItemStack.h"
#include "SoundCategory.h"

ItemEnderPearl::ItemEnderPearl()
{
    maxStackSize = 16;
    setCreativeTab(CreativeTabs::MISC);
}

ActionResult ItemEnderPearl::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    if (!playerIn->capabilities.isCreativeMode)
    {
        itemstack.shrink(1);
    }

    worldIn->playSound(nullptr, playerIn->posX, playerIn->posY, playerIn->posZ, SoundEvents::ENTITY_ENDERPEARL_THROW,
                       SoundCategory::NEUTRAL, 0.5F, 0.4F / (MathHelper::nextFloat(itemRand) * 0.4F + 0.8F));
    playerIn->getCooldownTracker().setCooldown(this, 20);
    if (!worldIn->isRemote)
    {
        EntityEnderPearl *entityenderpearl = new EntityEnderPearl(worldIn, playerIn);
        entityenderpearl.shoot(playerIn, playerIn->rotationPitch, playerIn->rotationYaw, 0.0F, 1.5F, 1.0F);
        worldIn->spawnEntity(entityenderpearl);
    }

    playerIn->addStat(StatList::getObjectUseStats(this));
    return ActionResult(EnumActionResult::SUCCESS, itemstack);
}
