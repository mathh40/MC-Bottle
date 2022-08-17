#include "BehaviorDefaultDispenseItem.h"

#include "IBlockSource.h"
#include "IPosition.h"
#include "../entity/item/EntityItem.h"
#include "../block/BlockDispenser.h"

ItemStack BehaviorDefaultDispenseItem::dispense(IBlockSource *source, const ItemStack &stack) {
    ItemStack itemstack = dispenseStack(source, stack);
    playDispenseSound(source);
    spawnDispenseParticles(source, source->getBlockState()->getValue(BlockDispenser::FACING));
    return itemstack;
}

void BehaviorDefaultDispenseItem::doDispense(World *worldIn, const ItemStack &stack, int32_t speed, EnumFacing facing,
                                             IPosition *position) {
    double d0 = position->getx();
    double d1 = position->gety();
    double d2 = position->getz();
    if (facing.getAxis() == Axis::Y) {
        d1 -= 0.125;
    } else {
        d1 -= 0.15625;
    }

    EntityItem* entityitem = new EntityItem(worldIn, d0, d1, d2, stack);
    double d3 = worldIn->rand.nextDouble() * 0.1 + 0.2;
    entityitem->motionX = (double)facing.getXOffset() * d3;
    entityitem->motionY = 0.20000000298023224;
    entityitem->motionZ = (double)facing.getZOffset() * d3;
    entityitem->motionX += worldIn->rand.nextGaussian() * 0.007499999832361937 * (double)speed;
    entityitem->motionY += worldIn->rand.nextGaussian() * 0.007499999832361937 * (double)speed;
    entityitem->motionZ += worldIn->rand.nextGaussian() * 0.007499999832361937 * (double)speed;
    worldIn->spawnEntity(entityitem);
}

ItemStack BehaviorDefaultDispenseItem::dispenseStack(IBlockSource *source, ItemStack stack) {
    EnumFacing enumfacing = (EnumFacing)source->getBlockState()->getValue(BlockDispenser::FACING);
    IPosition* iposition = BlockDispenser::getDispensePosition(source);
    ItemStack itemstack = stack.splitStack(1);
    doDispense(source->getWorld(), itemstack, 6, enumfacing, iposition);
    return stack;
}

void BehaviorDefaultDispenseItem::playDispenseSound(IBlockSource *source) {
    source->getWorld()->playEvent(1000, source->getBlockPos(), 0);
}

void BehaviorDefaultDispenseItem::spawnDispenseParticles(IBlockSource *source, EnumFacing facingIn) const {
    source->getWorld()->playEvent(2000, source->getBlockPos(), getWorldEventDataFrom(facingIn));
}

int32_t BehaviorDefaultDispenseItem::getWorldEventDataFrom(EnumFacing facingIn) {
    return facingIn.getXOffset() + 1 + (facingIn.getZOffset() + 1) * 3;
}
