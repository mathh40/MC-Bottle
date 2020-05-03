#include "ItemNameTag.h"

#include "ItemStack.h"
#include "Util.h"
#include "../pathfinding/NodeProcessor.h"

ItemNameTag::ItemNameTag()
{
    setCreativeTab(CreativeTabs::TOOLS);
}

bool ItemNameTag::itemInteractionForEntity(ItemStack stack, EntityPlayer *playerIn, EntityLivingBase *target,
    EnumHand hand)
{
    if (stack.hasDisplayName() && !(Util::instanceof<EntityPlayer>(target))) 
    {
        target->setCustomNameTag(stack.getDisplayName());
        if (Util::instanceof<EntityLiving>(target)) 
        {
            ((EntityLiving*)target)->enablePersistence();
        }

        stack.shrink(1);
        return true;
    }
    else 
    {
        return false;
    }
}
