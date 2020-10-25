#include "ContainerPlayer.h"
#include "InventoryCraftResult.h"
#include "SlotCrafting.h"
#include "../entity/EntityLiving.h"
#include "../item/ItemArmor.h"

class EquipmentSlot : public Slot
{
public:
    EquipmentSlot(const EntityEquipmentSlot* entityequipmentslot,IInventory* inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition)
        :Slot(inventoryIn, index, xPosition, yPosition),entityequipmentslot(entityequipmentslot)
    {
    }

    int32_t getSlotStackLimit() const override
    {
        return 1;
    }

    bool isItemValid(ItemStack stack) override
    {
        return entityequipmentslot == EntityLiving::getSlotForItemStack(stack);
    }

    bool canTakeStack(EntityPlayer* playerIn) override
    {
        ItemStack itemstack = getStack();
        return !itemstack.isEmpty() && !playerIn->isCreative() && EnchantmentHelper::hasBindingCurse(itemstack) ? false : Slot::canTakeStack(playerIn);
    }

    std::string getSlotTexture() override
    {
        return ItemArmor::EMPTY_SLOT_NAMES[entityequipmentslot->getIndex()];
    }
private:
    const EntityEquipmentSlot* entityequipmentslot;
};

class ShieldSlot : public Slot
{
public:
    ShieldSlot(IInventory* inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition)
        :Slot(inventoryIn, index, xPosition, yPosition)
    {
    }

    std::string getSlotTexture() override
    {
        return "minecraft:items/empty_armor_slot_shield";
    }
};


ContainerPlayer::ContainerPlayer(InventoryPlayer *playerInventory, bool localWorld, EntityPlayer *playerIn)
    :isLocalWorld(localWorld),player(playerIn)
{
    addSlotToContainer(SlotCrafting(playerInventory->player, craftMatrix, craftResult, 0, 154, 28));

    for(auto i1 = 0; i1 < 2; ++i1) 
    {
        for(auto j1 = 0; j1 < 2; ++j1) 
        {
            addSlotToContainer(Slot(craftMatrix, j1 + i1 * 2, 98 + j1 * 18, 18 + i1 * 18));
        }
    }

    for(auto i1 = 0; i1 < 4; ++i1) 
    {
        addSlotToContainer(EquipmentSlot(VALID_EQUIPMENT_SLOTS[i1],playerInventory, 36 + (3 - i1), 8, 8 + i1 * 18));
    }

    for(auto i1 = 0; i1 < 3; ++i1) 
    {
        for(auto j1 = 0; j1 < 9; ++j1) 
        {
            addSlotToContainer(Slot(playerInventory, j1 + (i1 + 1) * 9, 8 + j1 * 18, 84 + i1 * 18));
        }
    }

    for(auto i1 = 0; i1 < 9; ++i1) 
    {
        addSlotToContainer(Slot(playerInventory, i1, 8 + i1 * 18, 142));
    }

    addSlotToContainer(ShieldSlot(playerInventory, 40, 77, 62));
}

void ContainerPlayer::onCraftMatrixChanged(IInventory *inventoryIn)
{
    slotChangedCraftingGrid(player->world, player, craftMatrix, craftResult);
}

void ContainerPlayer::onContainerClosed(EntityPlayer *playerIn)
{
    Container::onContainerClosed(playerIn);
    craftResult->clear();
    if (!playerIn->world.isRemote) 
    {
        clearContainer(playerIn, playerIn->world, craftMatrix);
    }
}

bool ContainerPlayer::canInteractWith(EntityPlayer *playerIn)
{
    return true;
}

ItemStack ContainerPlayer::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack()) 
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack = itemstack1.copy();
        auto entityequipmentslot = EntityLiving::getSlotForItemStack(itemstack);
        if (index == 0) 
        {
            if (!mergeItemStack(itemstack1, 9, 45, true)) 
            {
                return ItemStack::EMPTY;
            }

            slot->onSlotChange(itemstack1, itemstack);
        }
        else if (index >= 1 && index < 5) 
        {
            if (!mergeItemStack(itemstack1, 9, 45, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (index >= 5 && index < 9) 
        {
            if (!mergeItemStack(itemstack1, 9, 45, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (entityequipmentslot.getSlotType() == EntityEquipmentSlot::Type::ARMOR && !(inventorySlots[8 - entityequipmentslot.getIndex()])->getHasStack()) 
        {
            auto i = 8 - entityequipmentslot.getIndex();
            if (!mergeItemStack(itemstack1, i, i + 1, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (entityequipmentslot == EntityEquipmentSlot::OFFHAND && !(inventorySlots[45]->getHasStack())) 
        {
            if (!mergeItemStack(itemstack1, 45, 46, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (index >= 9 && index < 36) 
        {
            if (!mergeItemStack(itemstack1, 36, 45, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (index >= 36 && index < 45) 
        {
            if (!mergeItemStack(itemstack1, 9, 36, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (!mergeItemStack(itemstack1, 9, 45, false)) 
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

bool ContainerPlayer::canMergeSlot(ItemStack stack, Slot slotIn)
{
    return slotIn.inventory != craftResult && Container::canMergeSlot(stack, slotIn);
}
