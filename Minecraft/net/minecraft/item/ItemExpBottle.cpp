#include "ItemExpBottle.h"

#include "ItemStack.h"
#include "SoundCategory.h"
#include "../stats/StatList.h"
#include "../world/gen/ChunkGeneratorFlat.h"

ItemExpBottle::ItemExpBottle()
{
    setCreativeTab(CreativeTabs::MISC);
}

bool ItemExpBottle::hasEffect(ItemStack stack)
{
    return true;
}

ActionResult ItemExpBottle::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    if (!playerIn->capabilities.isCreativeMode) 
    {
        itemstack.shrink(1);
    }

    worldIn->playSound(nullptr, playerIn->posX, playerIn->posY, playerIn->posZ, SoundEvents::ENTITY_EXPERIENCE_BOTTLE_THROW, SoundCategory::NEUTRAL, 0.5F, 0.4F / (MathHelper::nextFloat(itemRand) * 0.4F + 0.8F));
    if (!worldIn->isRemote) 
    {
        EntityExpBottle* entityexpbottle = new EntityExpBottle(worldIn, playerIn);
        entityexpbottle.shoot(playerIn, playerIn->rotationPitch, playerIn->rotationYaw, -20.0F, 0.7F, 1.0F);
        worldIn->spawnEntity(entityexpbottle);
    }

    playerIn->addStat(StatList::getObjectUseStats(this));
    return ActionResult(EnumActionResult::SUCCESS, itemstack);
}
