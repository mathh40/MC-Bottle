#include "SlotFurnaceOutput.h"

#include "../item/ItemStack.h"
#include "../item/crafting/FurnaceRecipes.h"
#include "math/MathHelper.h"

SlotFurnaceOutput::SlotFurnaceOutput(EntityPlayer *player, IInventory *inventoryIn, int32_t slotIndex,
                                     int32_t xPosition, int32_t yPosition)
    : Slot(inventoryIn, slotIndex, xPosition, yPosition), player(player)
{
}

bool SlotFurnaceOutput::isItemValid(ItemStack stack)
{
    return false;
}

ItemStack SlotFurnaceOutput::decrStackSize(int32_t amount)
{
    if (getHasStack())
    {
        removeCount += MathHelper::min(amount, getStack().getCount());
    }

    return Slot::decrStackSize(amount);
}

ItemStack SlotFurnaceOutput::onTake(EntityPlayer *thePlayer, ItemStack stack)
{
    onCrafting(stack);
    Slot::onTake(thePlayer, stack);
    return stack;
}

void SlotFurnaceOutput::onCrafting(ItemStack stack, int32_t amount)
{
    removeCount += amount;
    onCrafting(stack);
}

void SlotFurnaceOutput::onCrafting(ItemStack stack)
{
    stack.onCrafting(player->world, player, removeCount);
    if (!player->world.isRemote)
    {
        auto i = removeCount;
        auto f = FurnaceRecipes::instance().getSmeltingExperience(stack);
        auto j = 0;
        if (f == 0.0F)
        {
            i = 0;
        }
        else if (f < 1.0F)
        {
            pcg32 random;
            j = MathHelper::floor(i * f);
            if (j < MathHelper::ceil(i * f) && random() < i * f - j)
            {
                ++j;
            }

            i = j;
        }

        while (i > 0)
        {
            j = EntityXPOrb::getXPSplit(i);
            i -= j;
            player->world.spawnEntity(
                new EntityXPOrb(player->world, player->posX, player->posY + 0.5, player->posZ + 0.5, j));
        }
    }

    removeCount = 0;
}
