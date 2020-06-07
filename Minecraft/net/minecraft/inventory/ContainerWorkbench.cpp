#include "ContainerWorkbench.h"

#include "World.h"

ContainerWorkbench::ContainerWorkbench(InventoryPlayer *playerInventory, World *worldIn, const BlockPos &posIn)
    :world(worldIn),pos(posIn),player(playerInventory->player),craftMatrix(new InventoryCrafting(this, 3, 3)),craftResult(new InventoryCraftResult())
{
    addSlotToContainer(SlotCrafting(playerInventory->player, craftMatrix, craftResult, 0, 124, 35));

    for(auto l = 0; l < 3; ++l) 
    {
        for(auto i1 = 0; i1 < 3; ++i1) 
        {
            addSlotToContainer(Slot(craftMatrix, i1 + l * 3, 30 + i1 * 18, 17 + l * 18));
        }
    }

    for(auto l = 0; l < 3; ++l) 
    {
        for(auto i1 = 0; i1 < 9; ++i1) 
        {
            addSlotToContainer(Slot(playerInventory, i1 + l * 9 + 9, 8 + i1 * 18, 84 + l * 18));
        }
    }

    for(auto l = 0; l < 9; ++l) 
    {
        addSlotToContainer(Slot(playerInventory, l, 8 + l * 18, 142));
    }
}

void ContainerWorkbench::onCraftMatrixChanged(IInventory *inventoryIn)
{
    slotChangedCraftingGrid(world, player, craftMatrix, craftResult);
}

void ContainerWorkbench::onContainerClosed(EntityPlayer *playerIn)
{
    Container::onContainerClosed(playerIn);
    if (!world->isRemote) 
    {
        clearContainer(playerIn, world, craftMatrix);
    }
}

bool ContainerWorkbench::canInteractWith(EntityPlayer *playerIn)
{
    if (world->getBlockState(pos)->getBlock() != Blocks::CRAFTING_TABLE) 
    {
        return false;
    }
    else 
    {
        return playerIn->getDistanceSq(pos.getx() + 0.5, pos.gety() + 0.5, pos.getz() + 0.5) <= 64.0;
    }
}

ItemStack ContainerWorkbench::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack()) 
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack = itemstack1.copy();
        if (index == 0) 
        {
            itemstack1.getItem()->onCreated(itemstack1, world, playerIn);
            if (!mergeItemStack(itemstack1, 10, 46, true)) 
            {
                return ItemStack::EMPTY;
            }

            slot->onSlotChange(itemstack1, itemstack);
        }
        else if (index >= 10 && index < 37) 
        {
            if (!mergeItemStack(itemstack1, 37, 46, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (index >= 37 && index < 46) 
        {
            if (!mergeItemStack(itemstack1, 10, 37, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (!mergeItemStack(itemstack1, 10, 46, false)) 
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

        if (itemstack1.getCount() == itemstack.getCount())
        {
            return ItemStack::EMPTY;
        }

        ItemStack itemstack2 = slot->onTake(playerIn, itemstack1);
        if (index == 0) 
        {
            playerIn->dropItem(itemstack2, false);
        }
    }

    return itemstack;
}
