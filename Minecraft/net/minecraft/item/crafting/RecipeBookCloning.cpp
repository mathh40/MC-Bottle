#include "RecipeBookCloning.h"

#include "../ItemWrittenBook.h"
#include "Util.h"

bool RecipeBookCloning::matches(InventoryCrafting *inv, World *worldIn)
{
    auto i              = 0;
    ItemStack itemstack = ItemStack::EMPTY;

    for (auto j = 0; j < inv->getSizeInventory(); ++j)
    {
        ItemStack itemstack1 = inv->getStackInSlot(j);
        if (!itemstack1.isEmpty())
        {
            if (itemstack1.getItem() == Items::WRITTEN_BOOK)
            {
                if (!itemstack.isEmpty())
                {
                    return false;
                }

                itemstack = itemstack1;
            }
            else
            {
                if (itemstack1.getItem() != Items::WRITABLE_BOOK)
                {
                    return false;
                }

                ++i;
            }
        }
    }

    return !itemstack.isEmpty() && itemstack.hasTagCompound() && i > 0;
}

ItemStack RecipeBookCloning::getCraftingResult(InventoryCrafting *inv)
{
    auto i              = 0;
    ItemStack itemstack = ItemStack::EMPTY;

    for (auto j = 0; j < inv->getSizeInventory(); ++j)
    {
        ItemStack itemstack1 = inv->getStackInSlot(j);
        if (!itemstack1.isEmpty())
        {
            if (itemstack1.getItem() == Items::WRITTEN_BOOK)
            {
                if (!itemstack.isEmpty())
                {
                    return ItemStack::EMPTY;
                }

                itemstack = itemstack1;
            }
            else
            {
                if (itemstack1.getItem() != Items::WRITABLE_BOOK)
                {
                    return ItemStack::EMPTY;
                }

                ++i;
            }
        }
    }

    if (!itemstack.isEmpty() && itemstack.hasTagCompound() && i >= 1 && ItemWrittenBook::getGeneration(itemstack) < 2)
    {
        ItemStack itemstack2 = new ItemStack(Items::WRITTEN_BOOK, i);
        itemstack2.setTagCompound(itemstack.getTagCompound().copy());
        itemstack2.getTagCompound().setInteger("generation", ItemWrittenBook::getGeneration(itemstack) + 1);
        if (itemstack.hasDisplayName())
        {
            itemstack2.setStackDisplayName(itemstack.getDisplayName());
        }

        return itemstack2;
    }
    else
    {
        return ItemStack::EMPTY;
    }
}

ItemStack RecipeBookCloning::getRecipeOutput()
{
    return ItemStack::EMPTY;
}

std::vector<ItemStack> RecipeBookCloning::getRemainingItems(InventoryCrafting *inv)
{
    std::vector<ItemStack> nonnulllist(inv->getSizeInventory(), ItemStack::EMPTY);

    for (auto i = 0; i < nonnulllist.size(); ++i)
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (Util:: instanceof <ItemWrittenBook>(itemstack.getItem()))
        {
            ItemStack itemstack1 = itemstack.copy();
            itemstack1.setCount(1);
            nonnulllist[i] = itemstack1;
            break;
        }
    }

    return nonnulllist;
}

bool RecipeBookCloning::isDynamic()
{
    return true;
}

bool RecipeBookCloning::canFit(int32_t width, int32_t height)
{
    return width >= 3 && height >= 3;
}
