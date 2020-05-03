#include "ItemSpectralArrow.h"

#include "ItemStack.h"

EntityArrow* ItemSpectralArrow::createArrow(World *worldIn, ItemStack stack, EntityLivingBase *shooter)
{
    return new EntitySpectralArrow(worldIn, shooter);
}
