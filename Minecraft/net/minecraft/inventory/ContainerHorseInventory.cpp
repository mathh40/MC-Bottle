#include "ContainerHorseInventory.h"
#include "IInventory.h"
#include "Util.h"

class SaddleSlot : public Slot
{
public:
    SaddleSlot(const AbstractHorse* horse,IInventory* inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition);


    bool isItemValid(ItemStack stack) override
    {
        return stack.getItem() == Items::SADDLE && !getHasStack() && horse->canBeSaddled();
    }

    bool isEnabled() override
    {
        return horse->canBeSaddled();
    }
private:
    const AbstractHorse* horse;
};

class ArmorSlot : public Slot
{
public:
    ArmorSlot(const AbstractHorse* horse,IInventory* inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition);


    bool isItemValid(ItemStack stack) override
    {
        return horse->isArmor(stack);
    }

    bool isEnabled() override
    {
        return horse->wearsArmor();
    }

    static int32_t getSlotStackLimit()
    {
        return 1;
    }
private:
    const AbstractHorse* horse;
};

SaddleSlot::SaddleSlot(const AbstractHorse * horse, IInventory *inventoryIn, int32_t index, int32_t xPosition,
    int32_t yPosition)
    :Slot(inventoryIn, index, xPosition, yPosition),horse(horse)
{
}

ArmorSlot::ArmorSlot(const AbstractHorse *horse, IInventory *inventoryIn, int32_t index, int32_t xPosition,
    int32_t yPosition)
    :Slot(inventoryIn, index, xPosition, yPosition),horse(horse)
{
}

ContainerHorseInventory::ContainerHorseInventory(IInventory *playerInventory, IInventory *horseInventoryIn,
                                                 const AbstractHorse *horse, EntityPlayer *player)
        :horseInventory(horseInventoryIn),horse(horse)
{
    horseInventoryIn->openInventory(player);
    addSlotToContainer(SaddleSlot(horse, horseInventoryIn, 0, 8, 18));
    addSlotToContainer(ArmorSlot(horse, horseInventoryIn, 1, 8, 36));

    if (Util::instanceof<AbstractChestHorse>(horse) && ((AbstractChestHorse)horse).hasChest()) 
    {
        for(auto j1 = 0; j1 < 3; ++j1) 
        {
            for(auto k1 = 0; k1 < ((AbstractChestHorse)horse).getInventoryColumns(); ++k1) 
            {
                addSlotToContainer(Slot(horseInventoryIn, 2 + k1 + j1 * ((AbstractChestHorse)horse).getInventoryColumns(), 80 + k1 * 18, 18 + j1 * 18));
            }
        }
    }

    for(auto j1 = 0; j1 < 3; ++j1) 
    {
        for(auto k1 = 0; k1 < 9; ++k1) 
        {
            addSlotToContainer(Slot(playerInventory, k1 + j1 * 9 + 9, 8 + k1 * 18, 102 + j1 * 18 + -18));
        }
    }

    for(auto j1 = 0; j1 < 9; ++j1) 
    {
        addSlotToContainer(Slot(playerInventory, j1, 8 + j1 * 18, 142));
    }
}

bool ContainerHorseInventory::canInteractWith(EntityPlayer *playerIn)
{
    return horseInventory->isUsableByPlayer(playerIn) && horse->isEntityAlive() && horse->getDistance(playerIn) < 8.0F;
}

ItemStack ContainerHorseInventory::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack()) 
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack = itemstack1.copy();
        if (index < horseInventory->getSizeInventory()) 
        {
            if (!mergeItemStack(itemstack1, horseInventory->getSizeInventory(), inventorySlots.size(), true)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (getSlot(1).isItemValid(itemstack1) && !getSlot(1).getHasStack()) 
        {
            if (!mergeItemStack(itemstack1, 1, 2, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (getSlot(0).isItemValid(itemstack1)) {
            if (!mergeItemStack(itemstack1, 0, 1, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (horseInventory->getSizeInventory() <= 2 || !mergeItemStack(itemstack1, 2, horseInventory->getSizeInventory(), false)) 
        {
            return ItemStack::EMPTY;
        }

        if (itemstack1.isEmpty()) 
        {
            slot->putStack(ItemStack::EMPTY);
        }
        else 
        {
            slot->onSlotChanged();
        }
    }

    return itemstack;
}

void ContainerHorseInventory::onContainerClosed(EntityPlayer *playerIn)
{
    Container::onContainerClosed(playerIn);
    horseInventory->closeInventory(playerIn);
}
