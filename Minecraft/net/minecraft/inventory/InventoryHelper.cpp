#include "InventoryHelper.h"

#include <random>

#include "IInventory.h"
#include "../entity/item/EntityItem.h"
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

    float f = MathHelper::nextFloat(RANDOM) * 0.8F + 0.1F;
    float f1 = MathHelper::nextFloat(RANDOM) * 0.8F + 0.1F;
    float f2 = MathHelper::nextFloat(RANDOM) * 0.8F + 0.1F;

    while(!stack.isEmpty()) 
    {
        EntityItem* entityitem = new EntityItem(worldIn, x + (double)f, y + (double)f1, z + (double)f2, stack.splitStack(RANDOM(21) + 10));
        float f3 = 0.05F;
        entityitem->motionX = MathHelper::nextGaussian<double>(RANDOM) * 0.05000000074505806;
        entityitem->motionY = MathHelper::nextGaussian<double>(RANDOM) * 0.05000000074505806 + 0.20000000298023224;
        entityitem->motionZ = MathHelper::nextGaussian<double>(RANDOM) * 0.05000000074505806;
        worldIn->spawnEntity(entityitem);
    }
}

bool InventoryHelper::canMergeStacks(ItemStack stack1, ItemStack stack2,IInventory* inventory) {
    return !stack1.isEmpty() && stackEqualExact(stack1, stack2) && stack1.isStackable() && stack1.getCount() < stack1.getMaxStackSize() && stack1.getCount() < inventory->getInventoryStackLimit();
}

bool InventoryHelper::stackEqualExact(ItemStack stack1, ItemStack stack2) {
    return stack1.getItem() == stack2.getItem() && (!stack1.getHasSubtypes() || stack1.getMetadata() == stack2.getMetadata()) && ItemStack::areItemStackTagsEqual(stack1, stack2);
}

bool InventoryHelper::isInventoryEmpty(std::span<const ItemStack> inventory) {
    if ( std::adjacent_find( inventory.begin(), inventory.end(), [](const ItemStack& stack){return !stack.isEmpty();} ) == inventory.end() )
    {
        return true;
    }
    return false;
}
