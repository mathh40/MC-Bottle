#include "ItemChorusFruit.h"

#include "ItemStack.h"
#include "SoundCategory.h"
#include "Util.h"
#include "math/MathHelper.h"

ItemChorusFruit::ItemChorusFruit(int32_t amount, float saturation) : ItemFood(amount, saturation, false)
{
}

ItemStack ItemChorusFruit::onItemUseFinish(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving)
{
    ItemStack itemstack = ItemFood::onItemUseFinish(stack, worldIn, entityLiving);
    if (!worldIn->isRemote)
    {
        double d0 = entityLiving->posX;
        double d1 = entityLiving->posY;
        double d2 = entityLiving->posZ;

        for (int i = 0; i < 16; ++i)
        {
            double d3 = entityLiving->posX + (entityLiving->getRNG().nextDouble() - 0.5) * 16.0;
            double d4 = MathHelper::clamp(entityLiving->posY + (double)(entityLiving->getRNG().nextInt(16) - 8), 0.0,
                                          (double)(worldIn->getActualHeight() - 1));
            double d5 = entityLiving->posZ + (entityLiving->getRNG().nextDouble() - 0.5) * 16.0;
            if (entityLiving->isRiding())
            {
                entityLiving->dismountRidingEntity();
            }

            if (entityLiving->attemptTeleport(d3, d4, d5))
            {
                worldIn->playSound((EntityPlayer *)nullptr, d0, d1, d2, SoundEvents::ITEM_CHORUS_FRUIT_TELEPORT,
                                   SoundCategory::PLAYERS, 1.0F, 1.0F);
                entityLiving->playSound(SoundEvents::ITEM_CHORUS_FRUIT_TELEPORT, 1.0F, 1.0F);
                break;
            }
        }

        if (Util:: instanceof <EntityPlayer>(entityLiving))
        {
            ((EntityPlayer *)entityLiving)->getCooldownTracker().setCooldown(this, 20);
        }
    }

    return itemstack;
}
