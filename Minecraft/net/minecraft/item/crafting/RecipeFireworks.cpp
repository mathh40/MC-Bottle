#include "RecipeFireworks.h"

#include "../ItemDye.h"

RecipeFireworks::RecipeFireworks() : resultItem(ItemStack::EMPTY)
{
}

bool RecipeFireworks::matches(InventoryCrafting *inv, World *worldIn)
{
    resultItem = ItemStack::EMPTY;
    int32_t i  = 0;
    int32_t j  = 0;
    int32_t k  = 0;
    int32_t l  = 0;
    int32_t i1 = 0;
    int32_t j1 = 0;

    for (int32_t k1 = 0; k1 < inv->getSizeInventory(); ++k1)
    {
        ItemStack itemstack = inv->getStackInSlot(k1);
        if (!itemstack.isEmpty())
        {
            if (itemstack.getItem() == Items::GUNPOWDER)
            {
                ++j;
            }
            else if (itemstack.getItem() == Items::FIREWORK_CHARGE)
            {
                ++l;
            }
            else if (itemstack.getItem() == Items::DYE)
            {
                ++k;
            }
            else if (itemstack.getItem() == Items::PAPER)
            {
                ++i;
            }
            else if (itemstack.getItem() == Items::GLOWSTONE_DUST)
            {
                ++i1;
            }
            else if (itemstack.getItem() == Items::DIAMOND)
            {
                ++i1;
            }
            else if (itemstack.getItem() == Items::FIRE_CHARGE)
            {
                ++j1;
            }
            else if (itemstack.getItem() == Items::FEATHER)
            {
                ++j1;
            }
            else if (itemstack.getItem() == Items::GOLD_NUGGET)
            {
                ++j1;
            }
            else
            {
                if (itemstack.getItem() != Items::SKULL)
                {
                    return false;
                }

                ++j1;
            }
        }
    }

    i1 = i1 + k + j1;
    if (j <= 3 && i <= 1)
    {
        NBTTagCompound *nbttagcompound;
        NBTTagCompound *nbttagcompound2;
        if (j >= 1 && i == 1 && i1 == 0)
        {
            resultItem     = ItemStack(Items::FIREWORKS, 3);
            nbttagcompound = new NBTTagCompound();
            if (l > 0)
            {
                NBTTagList *nbttaglist = new NBTTagList();

                for (auto j2 = 0; j2 < inv->getSizeInventory(); ++j2)
                {
                    ItemStack itemstack3 = inv->getStackInSlot(j2);
                    if (itemstack3.getItem() == Items::FIREWORK_CHARGE && itemstack3.hasTagCompound() &&
                        itemstack3.getTagCompound()->hasKey("Explosion", 10))
                    {
                        nbttaglist->appendTag(itemstack3.getTagCompound()->getCompoundTag("Explosion"));
                    }
                }

                nbttagcompound->setTag("Explosions", nbttaglist);
            }

            nbttagcompound->setByte("Flight", j);
            nbttagcompound2 = new NBTTagCompound();
            nbttagcompound2->setTag("Fireworks", nbttagcompound);
            resultItem.setTagCompound(nbttagcompound2);
            return true;
        }
        else if (j == 1 && i == 0 && l == 0 && k > 0 && j1 <= 1)
        {
            resultItem      = new ItemStack(Items::FIREWORK_CHARGE);
            nbttagcompound  = new NBTTagCompound();
            nbttagcompound2 = new NBTTagCompound();
            unsigned char b0;
            std::vector<int32_t> list;

            for (int l1 = 0; l1 < inv->getSizeInventory(); ++l1)
            {
                ItemStack itemstack2 = inv->getStackInSlot(l1);
                if (!itemstack2.isEmpty())
                {
                    if (itemstack2.getItem() == Items::DYE)
                    {
                        list.emplace_back(ItemDye::DYE_COLORS[itemstack2.getMetadata() & 15]);
                    }
                    else if (itemstack2.getItem() == Items::GLOWSTONE_DUST)
                    {
                        nbttagcompound2->setBoolean("Flicker", true);
                    }
                    else if (itemstack2.getItem() == Items::DIAMOND)
                    {
                        nbttagcompound2->setBoolean("Trail", true);
                    }
                    else if (itemstack2.getItem() == Items::FIRE_CHARGE)
                    {
                        b0 = 1;
                    }
                    else if (itemstack2.getItem() == Items::FEATHER)
                    {
                        b0 = 4;
                    }
                    else if (itemstack2.getItem() == Items::GOLD_NUGGET)
                    {
                        b0 = 2;
                    }
                    else if (itemstack2.getItem() == Items::SKULL)
                    {
                        b0 = 3;
                    }
                }
            }

            std::vector<int32_t> aint1(list.size());

            for (auto l2 = 0; l2 < aint1.size(); ++l2)
            {
                aint1[l2] = list[l2];
            }

            nbttagcompound2->setIntArray("Colors", aint1);
            nbttagcompound2->setByte("Type", b0);
            nbttagcompound->setTag("Explosion", nbttagcompound2);
            resultItem.setTagCompound(nbttagcompound);
            return true;
        }
        else if (j == 0 && i == 0 && l == 1 && k > 0 && k == i1)
        {
            std::vector<int32_t> list1;

            for (auto i2 = 0; i2 < inv->getSizeInventory(); ++i2)
            {
                ItemStack itemstack1 = inv->getStackInSlot(i2);
                if (!itemstack1.isEmpty())
                {
                    if (itemstack1.getItem() == Items::DYE)
                    {
                        list1.emplace_back(ItemDye::DYE_COLORS[itemstack1.getMetadata() & 15]);
                    }
                    else if (itemstack1.getItem() == Items::FIREWORK_CHARGE)
                    {
                        resultItem = itemstack1.copy();
                        resultItem.setCount(1);
                    }
                }
            }

            std::vector<int32_t> aint(list1.size());

            for (auto j2 = 0; j2 < aint.size(); ++j2)
            {
                aint[j2] = list1[j2];
            }

            if (!resultItem.isEmpty() && resultItem.hasTagCompound())
            {
                NBTTagCompound *nbttagcompound4 = resultItem.getTagCompound()->getCompoundTag("Explosion");
                if (nbttagcompound4 == nullptr)
                {
                    return false;
                }
                else
                {
                    nbttagcompound4->setIntArray("FadeColors", aint);
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

ItemStack RecipeFireworks::getCraftingResult(InventoryCrafting *inv)
{
    return resultItem.copy();
}

ItemStack RecipeFireworks::getRecipeOutput()
{
    return resultItem;
}

std::vector<ItemStack> RecipeFireworks::getRemainingItems(InventoryCrafting *inv)
{
    std::vector<ItemStack> nonnulllist(inv->getSizeInventory(), ItemStack::EMPTY);

    for (int i = 0; i < nonnulllist.size(); ++i)
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (itemstack.getItem()->hasContainerItem())
        {
            nonnulllist[i] = ItemStack(itemstack.getItem()->getContainerItem());
        }
    }

    return nonnulllist;
}

bool RecipeFireworks::isDynamic()
{
    return true;
}

bool RecipeFireworks::canFit(int32_t width, int32_t height)
{
    return width * height >= 1;
}
