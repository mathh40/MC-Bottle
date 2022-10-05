#include "SlotCrafting.h"

#include "../item/ItemStack.h"
#include "../item/crafting/CraftingManager.h"
#include "../item/crafting/IRecipe.h"
#include "InventoryCraftResult.h"
#include "InventoryCrafting.h"
#include "math/MathHelper.h"

SlotCrafting::SlotCrafting(EntityPlayer *player, InventoryCrafting *craftingInventory, IInventory *inventoryIn,
                           int32_t slotIndex, int32_t xPosition, int32_t yPosition)
    : Slot(inventoryIn, slotIndex, xPosition, yPosition), player(player), craftMatrix(craftingInventory)
{
}

bool SlotCrafting::isItemValid(ItemStack stack)
{
    return false;
}

ItemStack SlotCrafting::decrStackSize(int32_t amount)
{
    if (getHasStack())
    {
        amountCrafted += MathHelper::min(amount, getStack().getCount());
    }

    return Slot::decrStackSize(amount);
}

ItemStack SlotCrafting::onTake(EntityPlayer *thePlayer, ItemStack stack)
{
    onCrafting(stack);
    auto nonnulllist = CraftingManager::getRemainingItems(craftMatrix, thePlayer->world);

    for (auto i = 0; i < nonnulllist.size(); ++i)
    {
        ItemStack itemstack  = craftMatrix->getStackInSlot(i);
        ItemStack itemstack1 = nonnulllist[i];
        if (!itemstack.isEmpty())
        {
            craftMatrix->decrStackSize(i, 1);
            itemstack = craftMatrix->getStackInSlot(i);
        }

        if (!itemstack1.isEmpty())
        {
            if (itemstack.isEmpty())
            {
                craftMatrix->setInventorySlotContents(i, itemstack1);
            }
            else if (ItemStack::areItemsEqual(itemstack, itemstack1) &&
                     ItemStack::areItemStackTagsEqual(itemstack, itemstack1))
            {
                itemstack1.grow(itemstack.getCount());
                craftMatrix->setInventorySlotContents(i, itemstack1);
            }
            else if (!player->inventory.addItemStackToInventory(itemstack1))
            {
                player->dropItem(itemstack1, false);
            }
        }
    }

    return stack;
}

void SlotCrafting::onCrafting(ItemStack stack, int32_t amount)
{
    amountCrafted += amount;
    onCrafting(stack);
}

void SlotCrafting::onSwapCraft(int32_t p_190900_1_)
{
    amountCrafted += p_190900_1_;
}

void SlotCrafting::onCrafting(ItemStack stack)
{
    if (amountCrafted > 0)
    {
        stack.onCrafting(player->world, player, amountCrafted);
    }

    amountCrafted                              = 0;
    InventoryCraftResult *inventorycraftresult = static_cast<InventoryCraftResult *>(inventory);
    IRecipe *irecipe                           = inventorycraftresult->getRecipeUsed();
    if (irecipe != nullptr && !irecipe->isDynamic())
    {
        player->unlockRecipes({irecipe});
        inventorycraftresult->setRecipeUsed(nullptr);
    }
}
