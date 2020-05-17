#include "RecipesBanners.h"

#include "../EnumDyeColor.h"
#include "../ItemBanner.h"
#include "../../tileentity/TileEntityBanner.h"

bool RecipesBanners::RecipeDuplicatePattern::matches(InventoryCrafting *inv, World *worldIn)
{
    ItemStack itemstack = ItemStack::EMPTY;
    ItemStack itemstack1 = ItemStack::EMPTY;

    for(auto i = 0; i < inv->getSizeInventory(); ++i) 
    {
        ItemStack itemstack2 = inv->getStackInSlot(i);
        if (!itemstack2.isEmpty()) 
        {
            if (itemstack2.getItem() != Items::BANNER) 
            {
                return false;
            }

            if (!itemstack.isEmpty() && !itemstack1.isEmpty()) 
            {
                return false;
            }

            EnumDyeColor enumdyecolor = ItemBanner::getBaseColor(itemstack2);
            bool flag = TileEntityBanner::getPatterns(itemstack2) > 0;
            if (!itemstack.isEmpty()) 
            {
                if (flag) 
                {
                    return false;
                }

                if (enumdyecolor != ItemBanner::getBaseColor(itemstack)) 
                {
                    return false;
                }

                itemstack1 = itemstack2;
            }
            else if (!itemstack1.isEmpty()) 
            {
                if (!flag) 
                {
                    return false;
                }

                if (enumdyecolor != ItemBanner::getBaseColor(itemstack1)) 
                {
                    return false;
                }

                itemstack = itemstack2;
            }
            else if (flag) 
            {
                itemstack = itemstack2;
            }
            else 
            {
                itemstack1 = itemstack2;
            }
        }
    }

    return !itemstack.isEmpty() && !itemstack1.isEmpty();
}

ItemStack RecipesBanners::RecipeDuplicatePattern::getCraftingResult(InventoryCrafting *inv)
{
    for(auto i = 0; i < inv->getSizeInventory(); ++i) 
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (!itemstack.isEmpty() && TileEntityBanner::getPatterns(itemstack) > 0) 
        {
            ItemStack itemstack1 = itemstack.copy();
            itemstack1.setCount(1);
            return itemstack1;
        }
    }

    return ItemStack::EMPTY;
}

ItemStack RecipesBanners::RecipeDuplicatePattern::getRecipeOutput()
{
    return ItemStack::EMPTY;
}

std::vector<ItemStack> RecipesBanners::RecipeDuplicatePattern::getRemainingItems(InventoryCrafting *inv)
{
    std::vector<ItemStack> nonnulllist(inv->getSizeInventory(), ItemStack::EMPTY);

    for(int i = 0; i < nonnulllist.size(); ++i) 
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (!itemstack.isEmpty()) 
        {
            if (itemstack.getItem()->hasContainerItem()) 
            {
                nonnulllist[i] = ItemStack(itemstack.getItem()->getContainerItem());
            }
            else if (itemstack.hasTagCompound() && TileEntityBanner::getPatterns(itemstack) > 0) 
            {
                ItemStack itemstack1 = itemstack.copy();
                itemstack1.setCount(1);
                nonnulllist[i] = itemstack1;
            }
        }
    }

    return nonnulllist;
}

bool RecipesBanners::RecipeDuplicatePattern::isDynamic()
{
    return true;
}

bool RecipesBanners::RecipeDuplicatePattern::canFit(int32_t width, int32_t height)
{
    return width * height >= 2;
}

bool RecipesBanners::RecipeAddPattern::matches(InventoryCrafting *inv, World *worldIn)
{
    bool flag = false;

    for(int i = 0; i < inv->getSizeInventory(); ++i) 
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (itemstack.getItem() == Items::BANNER) 
        {
            if (flag) 
            {
                return false;
            }

            if (TileEntityBanner::getPatterns(itemstack) >= 6) 
            {
                return false;
            }

            flag = true;
        }
    }

    if (!flag) 
    {
        return false;
    }
    else 
    {
        return matchPatterns(inv) != nullptr;
    }
}

ItemStack RecipesBanners::RecipeAddPattern::getCraftingResult(InventoryCrafting *inv)
{
    ItemStack itemstack = ItemStack::EMPTY;

    for(int i = 0; i < inv->getSizeInventory(); ++i) 
    {
        ItemStack itemstack1 = inv->getStackInSlot(i);
        if (!itemstack1.isEmpty() && itemstack1.getItem() == Items::BANNER) 
        {
            itemstack = itemstack1.copy();
            itemstack.setCount(1);
            break;
        }
    }

    BannerPattern bannerpattern = matchPatterns(inv);
    if (bannerpattern != nullptr) 
    {
        int k = 0;

        for(int j = 0; j < inv->getSizeInventory(); ++j) 
        {
            ItemStack itemstack2 = inv->getStackInSlot(j);
            if (itemstack2.getItem() == Items::DYE) 
            {
                k = itemstack2.getMetadata();
                break;
            }
        }

        NBTTagCompound* nbttagcompound1 = itemstack.getOrCreateSubCompound("BlockEntityTag");
        NBTTagList nbttaglist;
        if (nbttagcompound1->hasKey("Patterns", 9)) 
        {
            nbttaglist = nbttagcompound1->getTagList("Patterns", 10);
        }
        else 
        {
            nbttaglist = new NBTTagList();
            nbttagcompound1->setTag("Patterns", nbttaglist);
        }

        NBTTagCompound* nbttagcompound = new NBTTagCompound();
        nbttagcompound->setString("Pattern", bannerpattern.getHashname());
        nbttagcompound->setInteger("Color", k);
        nbttaglist.appendTag(nbttagcompound);
    }

    return itemstack;
}

ItemStack RecipesBanners::RecipeAddPattern::getRecipeOutput()
{
    return ItemStack::EMPTY;
}

std::vector<ItemStack> RecipesBanners::RecipeAddPattern::getRemainingItems(InventoryCrafting *inv)
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

bool RecipesBanners::RecipeAddPattern::isDynamic()
{
    return true;
}

bool RecipesBanners::RecipeAddPattern::canFit(int32_t width, int32_t height)
{
    return width >= 3 && height >= 3;
}

std::optional<BannerPattern> RecipesBanners::RecipeAddPattern::matchPatterns(InventoryCrafting* p_190933_1_)
{
    BannerPattern[] var2 = BannerPattern::values();
    int var3 = var2.size();

    for(int var4 = 0; var4 < var3; ++var4) 
    {
        BannerPattern bannerpattern = var2[var4];
        if (bannerpattern.hasPattern()) 
        {
            boolean flag = true;
            int l;
            if (bannerpattern.hasPatternItem()) 
            {
                boolean flag1 = false;
                boolean flag2 = false;

                for(l = 0; l < p_190933_1_->getSizeInventory() && flag; ++l) 
                {
                    ItemStack itemstack = p_190933_1_->getStackInSlot(l);
                    if (!itemstack.isEmpty() && itemstack.getItem() != Items::BANNER) 
                    {
                        if (itemstack.getItem() == Items::DYE) 
                        {
                            if (flag2) 
                            {
                                flag = false;
                                break;
                            }

                            flag2 = true;
                        }
                        else 
                        {
                            if (flag1 || !itemstack.isItemEqual(bannerpattern.getPatternItem())) 
                            {
                                flag = false;
                                break;
                            }

                            flag1 = true;
                        }
                    }
                }

                if (!flag1 || !flag2) 
                {
                    flag = false;
                }
            }
            else if (p_190933_1_->getSizeInventory() == bannerpattern.getPatterns().size() * bannerpattern.getPatterns()[0].length()) 
            {
                int j = -1;

                for(int k = 0; k < p_190933_1_->getSizeInventory() && flag; ++k) 
                {
                    l = k / 3;
                    int i1 = k % 3;
                    ItemStack itemstack1 = p_190933_1_->getStackInSlot(k);
                    if (!itemstack1.isEmpty() && itemstack1.getItem() != Items::BANNER) 
                    {
                        if (itemstack1.getItem() != Items::DYE) 
                        {
                            flag = false;
                            break;
                        }

                        if (j != -1 && j != itemstack1.getMetadata()) 
                        {
                            flag = false;
                            break;
                        }

                        if (bannerpattern.getPatterns()[l][i1] == ' ') 
                        {
                            flag = false;
                            break;
                        }

                        j = itemstack1.getMetadata();
                    }
                    else if (bannerpattern.getPatterns()[l][i1] != ' ') 
                    {
                        flag = false;
                        break;
                    }
                }
            }
            else 
            {
                flag = false;
            }

            if (flag) 
            {
                return bannerpattern;
            }
        }
    }

    return std::nullopt;
}
