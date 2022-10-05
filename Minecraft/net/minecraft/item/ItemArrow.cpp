#include "ItemArrow.h"

ItemArrow::ItemArrow()
{
    setCreativeTab(CreativeTabs::COMBAT);
}

EntityArrow *ItemArrow::createArrow(World *worldIn, ItemStack stack, EntityLivingBase *shooter)
{
    EntityTippedArrow *entitytippedarrow = new EntityTippedArrow(worldIn, shooter);
    entitytippedarrow->setPotionEffect(stack);
    return entitytippedarrow;
}
