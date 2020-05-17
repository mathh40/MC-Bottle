#include "ShulkerBoxRecipes.h"

#include "Util.h"
#include "../EnumDyeColor.h"

namespace ShulkerBoxRecipes
{
    bool ShulkerBoxColoring::matches(InventoryCrafting *inv, World *worldIn)
    {
        int32_t i = 0;
        int32_t j = 0;

        for(int32_t k = 0; k < inv->getSizeInventory(); ++k) 
        {
            ItemStack itemstack = inv->getStackInSlot(k);
            if (!itemstack.isEmpty()) 
            {
                if (Util::instanceof<BlockShulkerBox>(Block::getBlockFromItem(itemstack.getItem()))) 
                {
                    ++i;
                }
                else 
                {
                    if (itemstack.getItem() != Items::DYE) 
                    {
                        return false;
                    }

                    ++j;
                }

                if (j > 1 || i > 1) 
                {
                    return false;
                }
            }
        }

        return i == 1 && j == 1;
    }

    ItemStack ShulkerBoxColoring::getCraftingResult(InventoryCrafting *inv)
    {
        ItemStack itemstack = ItemStack::EMPTY;
        ItemStack itemstack1 = ItemStack::EMPTY;

        for(auto i = 0; i < inv->getSizeInventory(); ++i) 
        {
            ItemStack itemstack2 = inv->getStackInSlot(i);
            if (!itemstack2.isEmpty()) 
            {
                if (Util::instanceof< BlockShulkerBox>(Block::getBlockFromItem(itemstack2.getItem()))) 
                {
                    itemstack = itemstack2;
                }
                else if (itemstack2.getItem() == Items::DYE) 
                {
                    itemstack1 = itemstack2;
                }
            }
        }

        ItemStack itemstack3 = BlockShulkerBox::getColoredItemStack(EnumDyeColor::byDyeDamage(itemstack1.getMetadata()));
        if (itemstack.hasTagCompound()) 
        {
            itemstack3.setTagCompound(itemstack.getTagCompound()->copy());
        }

        return itemstack3;
    }

    ItemStack ShulkerBoxColoring::getRecipeOutput()
    {
        return ItemStack::EMPTY;
    }

    std::vector<ItemStack> ShulkerBoxColoring::getRemainingItems(InventoryCrafting *inv)
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

    bool ShulkerBoxColoring::isDynamic()
    {
        return true;
    }

    bool ShulkerBoxColoring::canFit(int32_t width, int32_t height)
    {
        return width * height >= 2;
    }
}
