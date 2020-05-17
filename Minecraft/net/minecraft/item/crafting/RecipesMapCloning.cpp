#include "RecipesMapCloning.h"

bool RecipesMapCloning::matches(InventoryCrafting *inv, World *worldIn)
{
    int i = 0;
    ItemStack itemstack = ItemStack::EMPTY;

    for(int j = 0; j < inv->getSizeInventory(); ++j) 
    {
        ItemStack itemstack1 = inv->getStackInSlot(j);
        if (!itemstack1.isEmpty()) 
        {
            if (itemstack1.getItem() == Items::FILLED_MAP) 
            {
                if (!itemstack.isEmpty()) 
                {
                    return false;
                }

                itemstack = itemstack1;
            }
            else 
            {
                if (itemstack1.getItem() != Items::MAP) 
                {
                    return false;
                }

                ++i;
            }
        }
    }

    return !itemstack.isEmpty() && i > 0;
}

ItemStack RecipesMapCloning::getCraftingResult(InventoryCrafting *inv)
{
    int i = 0;
    ItemStack itemstack = ItemStack::EMPTY;

    for(int j = 0; j < inv->getSizeInventory(); ++j) 
    {
        ItemStack itemstack1 = inv->getStackInSlot(j);
        if (!itemstack1.isEmpty()) 
        {
            if (itemstack1.getItem() == Items::FILLED_MAP) 
            {
                if (!itemstack.isEmpty()) 
                {
                    return ItemStack::EMPTY;
                }

                itemstack = itemstack1;
            }
            else 
            {
                if (itemstack1.getItem() != Items::MAP) 
                {
                    return ItemStack::EMPTY;
                }

                ++i;
            }
        }
    }

    if (!itemstack.isEmpty() && i >= 1) 
    {
        ItemStack itemstack2 = ItemStack(Items::FILLED_MAP, i + 1, itemstack.getMetadata());
        if (itemstack.hasDisplayName()) 
        {
            itemstack2.setStackDisplayName(itemstack.getDisplayName());
        }

        if (itemstack.hasTagCompound()) 
        {
            itemstack2.setTagCompound(itemstack.getTagCompound());
        }

        return itemstack2;
    }
    else 
    {
        return ItemStack::EMPTY;
    }
}

ItemStack RecipesMapCloning::getRecipeOutput()
{
    return ItemStack::EMPTY;
}

std::vector<ItemStack> RecipesMapCloning::getRemainingItems(InventoryCrafting *inv)
{
    std::vector<ItemStack> nonnulllist(inv->getSizeInventory(), ItemStack::EMPTY);

    for(int i = 0; i < nonnulllist.size(); ++i) 
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (itemstack.getItem()->hasContainerItem()) 
        {
            nonnulllist[i] = ItemStack(itemstack.getItem()->getContainerItem());
        }
    }

    return nonnulllist;
}

bool RecipesMapCloning::isDynamic()
{
    return true;
}

bool RecipesMapCloning::canFit(int32_t width, int32_t height)
{
    return width >= 3 && height >= 3;
}
