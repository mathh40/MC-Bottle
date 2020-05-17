#include "RecipesMapExtending.h"

#include "storage/MapData.h"

RecipesMapExtending::RecipesMapExtending()
    :ShapedRecipes("", 3, 3, {Ingredient::EMPTY, Ingredient::fromItems(Items::PAPER), Ingredient::fromItems(Items::PAPER), Ingredient::fromItems(Items::PAPER), Ingredient::fromItems(Items::PAPER), Ingredient::fromItem(Items::FILLED_MAP), Ingredient::fromItems(Items::PAPER), Ingredient::fromItems(Items::PAPER), Ingredient::fromItems(Items::PAPER), Ingredient::fromItems(Items::PAPER)}, ItemStack(Items::MAP))
{
}

bool RecipesMapExtending::matches(InventoryCrafting *inv, World *worldIn)
{
    if (!ShapedRecipes::matches(inv, worldIn)) 
    {
        return false;
    }
    else 
    {
        ItemStack itemstack = ItemStack::EMPTY;

        for(int i = 0; i < inv->getSizeInventory() && itemstack.isEmpty(); ++i) 
        {
            ItemStack itemstack1 = inv->getStackInSlot(i);
            if (itemstack1.getItem() == Items::FILLED_MAP) 
            {
                itemstack = itemstack1;
            }
        }

        if (itemstack.isEmpty()) 
        {
            return false;
        }
        else 
        {
            MapData mapdata = Items::FILLED_MAP.getMapData(itemstack, worldIn);
            if (mapdata == nullptr) 
            {
                return false;
            }
            else if (isExplorationMap(mapdata)) 
            {
                return false;
            }
            else 
            {
                return mapdata.scale < 4;
            }
        }
    }
}

ItemStack RecipesMapExtending::getCraftingResult(InventoryCrafting *inv)
{
    ItemStack itemstack = ItemStack::EMPTY;

    for(auto i = 0; i < inv->getSizeInventory() && itemstack.isEmpty(); ++i) 
    {
        ItemStack itemstack1 = inv->getStackInSlot(i);
        if (itemstack1.getItem() == Items::FILLED_MAP) 
        {
            itemstack = itemstack1;
        }
    }

    itemstack = itemstack.copy();
    itemstack.setCount(1);
    if (itemstack.getTagCompound() == nullptr) 
    {
        itemstack.setTagCompound(new NBTTagCompound());
    }

    itemstack.getTagCompound()->setInteger("map_scale_direction", 1);
    return itemstack;
}

bool RecipesMapExtending::isDynamic()
{
    return true;
}

bool RecipesMapExtending::isExplorationMap(MapData p_190934_1_)
{
    if (!p_190934_1_.mapDecorations.empty()) 
    {
        for(auto& entry : p_190934_1_.mapDecorations)
        {
            if (entry.second.getType() == MapDecoration::Type::MANSION || entry.second.getType() == MapDecoration::Type::MONUMENT) 
            {
                return true;
            }
        }
    }

    return false;
}
