#include "ItemFishingRod.h"

#include "../stats/StatList.h"
#include "../world/gen/ChunkGeneratorFlat.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "Util.h"

ItemFishingRod::ItemFishingRod()
{
    setMaxDamage(64);
    setMaxStackSize(1);
    setCreativeTab(CreativeTabs::TOOLS);
    addPropertyOverride(
        ResourceLocation("cast"), [&](ItemStack stack, World *worldIn, EntityLivingBase *entityIn) -> float {
            if (entityIn == nullptr)
            {
                return 0.0F;
            }
            else
            {
                bool flag  = entityIn->getHeldItemMainhand() == stack;
                bool flag1 = entityIn->getHeldItemOffhand() == stack;
                if (Util:: instanceof <ItemFishingRod>(entityIn->getHeldItemMainhand().getItem()))
                {
                    flag1 = false;
                }

                return (flag || flag1) && Util:: instanceof
                    <EntityPlayer>(entityIn) && ((EntityPlayer *)entityIn)->fishEntity != nullptr ? 1.0F : 0.0F;
            }
        });
}

bool ItemFishingRod::isFull3D() const
{
    return true;
}

bool ItemFishingRod::shouldRotateAroundWhenRendering()
{
    return true;
}

ActionResult ItemFishingRod::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    if (playerIn->fishEntity != nullptr)
    {
        int i = playerIn->fishEntity.handleHookRetraction();
        itemstack.damageItem(i, playerIn);
        playerIn->swingArm(handIn);
        worldIn->playSound(nullptr, playerIn->posX, playerIn->posY, playerIn->posZ, SoundEvents::ENTITY_BOBBER_RETRIEVE,
                           SoundCategory::NEUTRAL, 1.0F, 0.4F / (MathHelper::nextFloat(itemRand) * 0.4F + 0.8F));
    }
    else
    {
        worldIn->playSound(nullptr, playerIn->posX, playerIn->posY, playerIn->posZ, SoundEvents::ENTITY_BOBBER_THROW,
                           SoundCategory::NEUTRAL, 0.5F, 0.4F / (MathHelper::nextFloat(itemRand) * 0.4F + 0.8F));
        if (!worldIn->isRemote)
        {
            EntityFishHook entityfishhook = new EntityFishHook(worldIn, playerIn);
            int j                         = EnchantmentHelper::getFishingSpeedBonus(itemstack);
            if (j > 0)
            {
                entityfishhook.setLureSpeed(j);
            }

            int k = EnchantmentHelper::getFishingLuckBonus(itemstack);
            if (k > 0)
            {
                entityfishhook.setLuck(k);
            }

            worldIn->spawnEntity(entityfishhook);
        }

        playerIn->swingArm(handIn);
        playerIn->addStat(StatList::getObjectUseStats(this));
    }

    return ActionResult(EnumActionResult::SUCCESS, itemstack);
}

int32_t ItemFishingRod::getItemEnchantability()
{
    return 1;
}
