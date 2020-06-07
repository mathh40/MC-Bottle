#include "InventoryHelper.h"

#include <random>



#include "IInventory.h"
#include "World.h"
#include "../item/ItemStack.h"
#include "math/BlockPos.h"
#include "math/MathHelper.h"

void InventoryHelper::dropInventoryItems(World *worldIn, BlockPos pos, IInventory *inventory)
{
    dropInventoryItems(worldIn, pos.getx(), pos.gety(), pos.getz(), inventory);
}

void InventoryHelper::dropInventoryItems(World *worldIn, Entity *entityAt, IInventory *inventory)
{
    dropInventoryItems(worldIn, entityAt->posX, entityAt->posY, entityAt->posZ, inventory);
}

void InventoryHelper::dropInventoryItems(World *worldIn, double x, double y, double z, IInventory *inventory)
{
    for(auto i = 0; i < inventory->getSizeInventory(); ++i) 
    {
        ItemStack itemstack = inventory->getStackInSlot(i);
        if (!itemstack.isEmpty()) 
        {
            spawnItemStack(worldIn, x, y, z, itemstack);
        }
    }
}

void InventoryHelper::spawnItemStack(World *worldIn, double x, double y, double z, ItemStack stack)
{
    static pcg32 RANDOM;
    std::normal_distribution<double> distribution(5.0,2.0);

    float f = RANDOM.nextFloat() * 0.8F + 0.1F;
    float f1 = RANDOM.nextFloat() * 0.8F + 0.1F;
    float f2 = RANDOM.nextFloat() * 0.8F + 0.1F;

    while(!stack.isEmpty()) 
    {
        EntityItem* entityitem = new EntityItem(worldIn, x + (double)f, y + (double)f1, z + (double)f2, stack.splitStack(RANDOM(21) + 10));
        float f3 = 0.05F;
        entityitem->motionX = MathHelper::nextGaussian(RANDOM) * 0.05000000074505806;
        entityitem->motionY = MathHelper::nextGaussian(RANDOM) * 0.05000000074505806 + 0.20000000298023224;
        entityitem->motionZ = MathHelper::nextGaussian(RANDOM) * 0.05000000074505806;
        worldIn->spawnEntity(entityitem);
    }
}
