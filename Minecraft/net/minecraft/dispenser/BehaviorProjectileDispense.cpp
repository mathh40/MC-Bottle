#include "BehaviorProjectileDispense.h"

#include "../entity/IProjectile.h"
#include "../world/World.h"
#include "IBlockSource.h"

ItemStack BehaviorProjectileDispense::dispenseStack(IBlockSource *source, ItemStack stack)
{
    World *world             = source->getWorld();
    IPosition *iposition     = BlockDispenser::getDispensePosition(source);
    EnumFacing enumfacing    = (EnumFacing)source->getBlockState()->getValue(BlockDispenser::FACING);
    IProjectile *iprojectile = getProjectileEntity(world, iposition, stack);
    iprojectile->shoot((double)enumfacing.getXOffset(), (double)((float)enumfacing.getYOffset() + 0.1F),
                       (double)enumfacing.getZOffset(), getProjectileVelocity(), getProjectileInaccuracy());
    world->spawnEntity((Entity *)iprojectile);
    stack.shrink(1);
    return stack;
}

void BehaviorProjectileDispense::playDispenseSound(IBlockSource *source)
{
    source->getWorld()->playEvent(1002, source->getBlockPos(), 0);
}

float BehaviorProjectileDispense::getProjectileInaccuracy()
{
    return 6.0F;
}

float BehaviorProjectileDispense::getProjectileVelocity()
{
    return 1.1F;
}
