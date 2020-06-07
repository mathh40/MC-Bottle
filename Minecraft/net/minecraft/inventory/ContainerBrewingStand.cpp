#include "ContainerBrewingStand.h"

#include "IContainerListener.h"
#include "IInventory.h"
#include "Util.h"
#include "../potion/PotionHelper.h"
#include "../potion/PotionType.h"
#include "../potion/PotionUtils.h"

PotionSlot::PotionSlot(IInventory *p_i47598_1_, int32_t p_i47598_2_, int32_t p_i47598_3_, int32_t p_i47598_4_)
    :Slot(p_i47598_1_, p_i47598_2_, p_i47598_3_, p_i47598_4_)
{
}

bool PotionSlot::isItemValid(ItemStack stack)
{
    return canHoldPotion(stack);
}

int32_t PotionSlot::getSlotStackLimit() const
{
    return 1;
}

ItemStack PotionSlot::onTake(EntityPlayer *thePlayer, ItemStack stack)
{
    PotionType potiontype = PotionUtils::getPotionFromItem(stack);
    if (Util::instanceof< EntityPlayerMP>(thePlayer)) 
    {
        CriteriaTriggers::BREWED_POTION.trigger((EntityPlayerMP*)thePlayer, potiontype);
    }

    Slot::onTake(thePlayer, stack);
    return stack;
}

bool PotionSlot::canHoldPotion(ItemStack stack)
{
    Item* item = stack.getItem();
    return item == Items::POTIONITEM || item == Items::SPLASH_POTION || item == Items::LINGERING_POTION || item == Items::GLASS_BOTTLE;
}

IngredientSlot::IngredientSlot(IInventory *iInventoryIn, int32_t index, int32_t xPosition, int32_t yPosition)
:Slot(iInventoryIn, index, xPosition, yPosition)
{
}

bool IngredientSlot::isItemValid(ItemStack stack)
{
    return PotionHelper::isReagent(stack);
}

int32_t IngredientSlot::getSlotStackLimit() const
{
    return 64;
}

FuelSlot::FuelSlot(IInventory *iInventoryIn, int32_t index, int32_t xPosition, int32_t yPosition)
    :Slot(iInventoryIn, index, xPosition, yPosition)
{
}

bool FuelSlot::isItemValid(ItemStack stack)
{
    return isValidBrewingFuel(stack);
}

bool FuelSlot::isValidBrewingFuel(ItemStack itemStackIn)
{
    return itemStackIn.getItem() == Items::BLAZE_POWDER;
}

int32_t FuelSlot::getSlotStackLimit() const
{
    return 64;
}

ContainerBrewingStand::ContainerBrewingStand(InventoryPlayer *playerInventory, IInventory *tileBrewingStandIn)
    :tileBrewingStand(tileBrewingStandIn)
{
    addSlotToContainer(PotionSlot(tileBrewingStandIn, 0, 56, 51));
    addSlotToContainer(PotionSlot(tileBrewingStandIn, 1, 79, 58));
    addSlotToContainer(PotionSlot(tileBrewingStandIn, 2, 102, 51));
    slot = addSlotToContainer(IngredientSlot(tileBrewingStandIn, 3, 79, 17));
    addSlotToContainer(FuelSlot(tileBrewingStandIn, 4, 17, 17));

    for(auto k = 0; k < 3; ++k) 
    {
        for(auto j = 0; j < 9; ++j) 
        {
            addSlotToContainer(Slot(playerInventory, j + k * 9 + 9, 8 + j * 18, 84 + k * 18));
        }
    }

    for(auto k = 0; k < 9; ++k) 
    {
        addSlotToContainer(Slot(playerInventory, k, 8 + k * 18, 142));
    }
}

void ContainerBrewingStand::addListener(IContainerListener *listener)
{
    Container::addListener(listener);
    listener->sendAllWindowProperties(*this, tileBrewingStand);
}

void ContainerBrewingStand::detectAndSendChanges()
{
    Container::detectAndSendChanges();

    for(auto i = 0; i < listeners.size(); ++i) 
    {
        IContainerListener* icontainerlistener = listeners[i];
        if (prevBrewTime != tileBrewingStand->getField(0)) 
        {
            icontainerlistener->sendWindowProperty(*this, 0, tileBrewingStand->getField(0));
        }

        if (prevFuel != tileBrewingStand->getField(1)) 
        {
            icontainerlistener->sendWindowProperty(*this, 1, tileBrewingStand->getField(1));
        }
    }

    prevBrewTime = tileBrewingStand->getField(0);
    prevFuel = tileBrewingStand->getField(1);
}

void ContainerBrewingStand::updateProgressBar(int32_t id, int32_t data)
{
    tileBrewingStand->setField(id, data);
}

bool ContainerBrewingStand::canInteractWith(EntityPlayer *playerIn)
{
    return tileBrewingStand->isUsableByPlayer(playerIn);
}

ItemStack ContainerBrewingStand::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack()) 
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack = itemstack1.copy();
        if ((index < 0 || index > 2) && index != 3 && index != 4) 
        {
            if (slot->isItemValid(itemstack1)) 
            {
                if (!mergeItemStack(itemstack1, 3, 4, false)) 
                {
                    return ItemStack::EMPTY;
                }
            }
            else if (PotionSlot::canHoldPotion(itemstack) && itemstack.getCount() == 1) 
            {
                if (!mergeItemStack(itemstack1, 0, 3, false)) 
                {
                    return ItemStack::EMPTY;
                }
            }
            else if (FuelSlot::isValidBrewingFuel(itemstack)) 
            {
                if (!mergeItemStack(itemstack1, 4, 5, false)) 
                {
                    return ItemStack::EMPTY;
                }
            }
            else if (index >= 5 && index < 32) 
            {
                if (!mergeItemStack(itemstack1, 32, 41, false)) 
                {
                    return ItemStack::EMPTY;
                }
            }
            else if (index >= 32 && index < 41) 
            {
                if (!mergeItemStack(itemstack1, 5, 32, false)) 
                {
                    return ItemStack::EMPTY;
                }
            }
            else if (!mergeItemStack(itemstack1, 5, 41, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else 
        {
            if (!mergeItemStack(itemstack1, 5, 41, true)) 
            {
                return ItemStack::EMPTY;
            }

            slot->onSlotChange(itemstack1, itemstack);
        }

        if (itemstack1.isEmpty()) 
        {
            slot->putStack(ItemStack::EMPTY);
        }
        else 
        {
            slot->onSlotChanged();
        }

        if (itemstack1.getCount() == itemstack.getCount()) 
        {
            return ItemStack::EMPTY;
        }

        slot->onTake(playerIn, itemstack1);
    }

    return itemstack;
}
