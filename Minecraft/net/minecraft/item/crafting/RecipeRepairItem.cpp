#include "RecipeRepairItem.h"

bool RecipeRepairItem::matches(InventoryCrafting *inv, World *worldIn)
{
    std::vector<ItemStack> list;

    for(auto i = 0; i < inv->getSizeInventory(); ++i) 
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (!itemstack.isEmpty()) 
        {
            list.emplace_back(itemstack);
            if (list.size() > 1) 
            {
                ItemStack itemstack1 = list[0];
                if (itemstack.getItem() != itemstack1.getItem() || itemstack1.getCount() != 1 || itemstack.getCount() != 1 || !itemstack1.getItem()->isDamageable()) 
                {
                    return false;
                }
            }
        }
    }

    return list.size() == 2;
}

ItemStack RecipeRepairItem::getCraftingResult(InventoryCrafting *inv)
{
    std::vector<ItemStack> list;

    ItemStack itemstack = ItemStack::EMPTY;
    for(int i = 0; i < inv->getSizeInventory(); ++i) 
    {
        itemstack = inv->getStackInSlot(i);
        if (!itemstack.isEmpty()) 
        {
            list.emplace_back(itemstack);
            if (list.size() > 1) 
            {
                ItemStack itemstack1 = list[0];
                if (itemstack.getItem() != itemstack1.getItem() || itemstack1.getCount() != 1 || itemstack.getCount() != 1 || !itemstack1.getItem()->isDamageable()) 
                {
                    return ItemStack::EMPTY;
                }
            }
        }
    }

    if (list.size() == 2) 
    {
        ItemStack itemstack2 = list[0];
        itemstack = list[1];
        if (itemstack2.getItem() == itemstack.getItem() && itemstack2.getCount() == 1 && itemstack.getCount() == 1 && itemstack2.getItem()->isDamageable()) 
        {
            Item* item = itemstack2.getItem();
            auto j = item->getMaxDamage() - itemstack2.getItemDamage();
            auto k = item->getMaxDamage() - itemstack.getItemDamage();
            auto l = j + k + item->getMaxDamage() * 5 / 100;
            auto i1 = item->getMaxDamage() - l;
            if (i1 < 0) 
            {
                i1 = 0;
            }

            return ItemStack(itemstack2.getItem(), 1, i1);
        }
    }

    return ItemStack::EMPTY;
}

ItemStack RecipeRepairItem::getRecipeOutput()
{
    return ItemStack::EMPTY;
}

std::vector<ItemStack> RecipeRepairItem::getRemainingItems(InventoryCrafting *inv)
{
    std::vector<ItemStack> nonnulllist(inv->getSizeInventory(), ItemStack::EMPTY);

    for(auto i = 0; i < nonnulllist.size(); ++i) 
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (itemstack.getItem()->hasContainerItem()) 
        {
            nonnulllist[i] = ItemStack(itemstack.getItem()->getContainerItem());
        }
    }

    return nonnulllist;
}

bool RecipeRepairItem::isDynamic()
{
    return true;
}

bool RecipeRepairItem::canFit(int32_t width, int32_t height)
{
    return width * height >= 2;
}
