#include "RecipeTippedArrow.h"

#include "../../potion/PotionUtils.h"

bool RecipeTippedArrow::matches(InventoryCrafting *inv, World *worldIn)
{
    if (inv->getWidth() == 3 && inv->getHeight() == 3)
    {
        for (auto i = 0; i < inv->getWidth(); ++i)
        {
            for (auto j = 0; j < inv->getHeight(); ++j)
            {
                ItemStack itemstack = inv->getStackInRowAndColumn(i, j);
                if (itemstack.isEmpty())
                {
                    return false;
                }

                Item *item = itemstack.getItem();
                if (i == 1 && j == 1)
                {
                    if (item != Items::LINGERING_POTION)
                    {
                        return false;
                    }
                }
                else if (item != Items::ARROW)
                {
                    return false;
                }
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

ItemStack RecipeTippedArrow::getCraftingResult(InventoryCrafting *inv)
{
    ItemStack itemstack = inv->getStackInRowAndColumn(1, 1);
    if (itemstack.getItem() != Items::LINGERING_POTION)
    {
        return ItemStack::EMPTY;
    }
    else
    {
        ItemStack itemstack1 = ItemStack(Items::TIPPED_ARROW, 8);
        PotionUtils::addPotionToItemStack(itemstack1, PotionUtils::getPotionFromItem(itemstack));
        PotionUtils::appendEffects(itemstack1, PotionUtils::getFullEffectsFromItem(itemstack));
        return itemstack1;
    }
}

ItemStack RecipeTippedArrow::getRecipeOutput()
{
    return ItemStack::EMPTY;
}

std::vector<ItemStack> RecipeTippedArrow::getRemainingItems(InventoryCrafting *inv)
{
    return std::vector<ItemStack>(inv->getSizeInventory(), ItemStack::EMPTY);
}

bool RecipeTippedArrow::isDynamic()
{
    return true;
}

bool RecipeTippedArrow::canFit(int32_t width, int32_t height)
{
    return width >= 2 && height >= 2;
}
