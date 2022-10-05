#include "ShieldRecipes.h"
namespace ShieldRecipes
{
bool Decoration::matches(InventoryCrafting *inv, World *worldIn)
{
    ItemStack itemstack  = ItemStack::EMPTY;
    ItemStack itemstack1 = ItemStack::EMPTY;

    for (int i = 0; i < inv->getSizeInventory(); ++i)
    {
        ItemStack itemstack2 = inv->getStackInSlot(i);
        if (!itemstack2.isEmpty())
        {
            if (itemstack2.getItem() == Items::BANNER)
            {
                if (!itemstack1.isEmpty())
                {
                    return false;
                }

                itemstack1 = itemstack2;
            }
            else
            {
                if (itemstack2.getItem() != Items::SHIELD)
                {
                    return false;
                }

                if (!itemstack.isEmpty())
                {
                    return false;
                }

                if (itemstack2.getSubCompound("BlockEntityTag") != nullptr)
                {
                    return false;
                }

                itemstack = itemstack2;
            }
        }
    }

    if (!itemstack.isEmpty() && !itemstack1.isEmpty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

ItemStack Decoration::getCraftingResult(InventoryCrafting *inv)
{
    ItemStack itemstack  = ItemStack::EMPTY;
    ItemStack itemstack1 = ItemStack::EMPTY;

    for (int i = 0; i < inv->getSizeInventory(); ++i)
    {
        ItemStack itemstack2 = inv->getStackInSlot(i);
        if (!itemstack2.isEmpty())
        {
            if (itemstack2.getItem() == Items::BANNER)
            {
                itemstack = itemstack2;
            }
            else if (itemstack2.getItem() == Items::SHIELD)
            {
                itemstack1 = itemstack2.copy();
            }
        }
    }

    if (itemstack1.isEmpty())
    {
        return itemstack1;
    }
    else
    {
        NBTTagCompound *nbttagcompound  = itemstack.getSubCompound("BlockEntityTag");
        NBTTagCompound *nbttagcompound1 = nbttagcompound == nullptr ? new NBTTagCompound() : nbttagcompound->copy();
        nbttagcompound1->setInteger("Base", itemstack.getMetadata() & 15);
        itemstack1.setTagInfo("BlockEntityTag", nbttagcompound1);
        return itemstack1;
    }
}

ItemStack Decoration::getRecipeOutput()
{
    return ItemStack::EMPTY;
}

std::vector<ItemStack> Decoration::getRemainingItems(InventoryCrafting *inv)
{
    std::vector<ItemStack> nonnulllist(inv->getSizeInventory(), ItemStack::EMPTY);

    for (auto i = 0; i < nonnulllist.size(); ++i)
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (itemstack.getItem()->hasContainerItem())
        {
            nonnulllist[i] = ItemStack(itemstack.getItem()->getContainerItem());
        }
    }

    return nonnulllist;
}

bool Decoration::isDynamic()
{
    return true;
}

bool Decoration::canFit(int32_t width, int32_t height)
{
    return width * height >= 2;
}
} // namespace ShieldRecipes
