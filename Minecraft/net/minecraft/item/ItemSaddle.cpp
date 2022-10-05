#include "ItemSaddle.h"

#include "ItemStack.h"
#include "SoundCategory.h"
#include "Util.h"

ItemSaddle::ItemSaddle()
{
    maxStackSize = 1;
    setCreativeTab(CreativeTabs::TRANSPORTATION);
}

bool ItemSaddle::itemInteractionForEntity(ItemStack stack, EntityPlayer *playerIn, EntityLivingBase *target,
                                          EnumHand hand)
{
    if (Util:: instanceof <EntityPig>(target))
    {
        EntityPig *entitypig = (EntityPig *)target;
        if (!entitypig.getSaddled() && !entitypig.isChild())
        {
            entitypig.setSaddled(true);
            entitypig.world.playSound(playerIn, entitypig.posX, entitypig.posY, entitypig.posZ,
                                      SoundEvents.ENTITY_PIG_SADDLE, SoundCategory::NEUTRAL, 0.5F, 1.0F);
            stack.shrink(1);
        }

        return true;
    }
    else
    {
        return false;
    }
}
