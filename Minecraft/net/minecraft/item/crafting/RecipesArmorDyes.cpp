#include "RecipesArmorDyes.h"

#include "../EnumDyeColor.h"
#include "../ItemArmor.h"
#include "Util.h"
#include "math/MathHelper.h"

bool RecipesArmorDyes::matches(InventoryCrafting *inv, World *worldIn)
{
    ItemStack itemstack = ItemStack::EMPTY;
    std::vector<ItemStack> list;

    for (auto i = 0; i < inv->getSizeInventory(); ++i)
    {
        ItemStack itemstack1 = inv->getStackInSlot(i);
        if (!itemstack1.isEmpty())
        {
            if (Util:: instanceof <ItemArmor>(itemstack1.getItem()))
            {
                ItemArmor *itemarmor = itemstack1.getItem();
                if (itemarmor->getArmorMaterial() != ArmorMaterial::LEATHER || !itemstack.isEmpty())
                {
                    return false;
                }

                itemstack = itemstack1;
            }
            else
            {
                if (itemstack1.getItem() != Items::DYE)
                {
                    return false;
                }

                list.emplace_back(itemstack1);
            }
        }
    }

    return !itemstack.isEmpty() && !list.empty();
}

ItemStack RecipesArmorDyes::getCraftingResult(InventoryCrafting *inv)
{
    ItemStack itemstack = ItemStack::EMPTY;
    std::array<int32_t, 3> aint;
    ItemArmor *itemarmor = nullptr;

    int32_t i = 0;
    int32_t j = 0;

    for (auto i1 = 0; i1 < inv->getSizeInventory(); ++i1)
    {
        ItemStack itemstack1 = inv->getStackInSlot(i1);
        if (!itemstack1.isEmpty())
        {
            if (Util:: instanceof <ItemArmor>(itemstack1.getItem()))
            {
                itemarmor = itemstack1.getItem();
                if (itemarmor->getArmorMaterial() != ArmorMaterial::LEATHER || !itemstack.isEmpty())
                {
                    return ItemStack::EMPTY;
                }

                itemstack = itemstack1.copy();
                itemstack.setCount(1);
                if (itemarmor->hasColor(itemstack1))
                {
                    auto l   = itemarmor->getColor(itemstack);
                    auto f   = (float)(l >> 16 & 255) / 255.0F;
                    auto f1  = (float)(l >> 8 & 255) / 255.0F;
                    float f2 = (float)(l & 255) / 255.0F;
                    auto i   = (int)((float)i + MathHelper::max(f, MathHelper::max(f1, f2)) * 255.0F);
                    aint[0]  = (int)((float)aint[0] + f * 255.0F);
                    aint[1]  = (int)((float)aint[1] + f1 * 255.0F);
                    aint[2]  = (int)((float)aint[2] + f2 * 255.0F);
                    ++j;
                }
            }
            else
            {
                if (itemstack1.getItem() != Items::DYE)
                {
                    return ItemStack::EMPTY;
                }

                auto afloat = EnumDyeColor::byDyeDamage(itemstack1.getMetadata())->getColorComponentValues();
                auto l1     = (int)(afloat[0] * 255.0F);
                auto i2     = (int)(afloat[1] * 255.0F);
                auto j2     = (int)(afloat[2] * 255.0F);
                i += MathHelper::max(l1, MathHelper::max(i2, j2));
                aint[0] += l1;
                aint[1] += i2;
                aint[2] += j2;
                ++j;
            }
        }
    }

    if (itemarmor == nullptr)
    {
        return ItemStack::EMPTY;
    }
    else
    {
        auto i1 = aint[0] / j;
        auto j1 = aint[1] / j;
        auto l  = aint[2] / j;
        auto f  = (float)i / (float)j;
        auto f1 = (float)MathHelper::max(i1, MathHelper::max(j1, l));
        i1      = (int)((float)i1 * f / f1);
        j1      = (int)((float)j1 * f / f1);
        l       = (int)((float)l * f / f1);
        auto j2 = (i1 << 8) + j1;
        j2      = (j2 << 8) + l;
        itemarmor->setColor(itemstack, j2);
        return itemstack;
    }
}

ItemStack RecipesArmorDyes::getRecipeOutput()
{
    return ItemStack::EMPTY;
}

std::vector<ItemStack> RecipesArmorDyes::getRemainingItems(InventoryCrafting *inv)
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

bool RecipesArmorDyes::isDynamic()
{
    return true;
}

bool RecipesArmorDyes::canFit(int32_t width, int32_t height)
{
    return width * height >= 2;
}
