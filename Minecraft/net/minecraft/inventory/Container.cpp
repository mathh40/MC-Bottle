#include "Container.h"

#include "IContainerListener.h"
#include "IInventory.h"
#include "Util.h"
#include "../../../../nlohmann_json/include/nlohmann/detail/value_t.hpp"
#include "../item/crafting/CraftingManager.h"
#include "../item/crafting/IRecipe.h"

constexpr int32_t UNENDLICH = -999;

void Container::addListener(IContainerListener *listener)
{  
    if (std::find(listeners.begin(),listeners.end(),listener) != listeners.end()) 
    {
        throw std::runtime_error("Listener already listening");
    }
    else 
    {
        listeners.emplace_back(listener);
        listener->sendAllContents(*this, getInventory());
        detectAndSendChanges();
    }
}

void Container::removeListener(IContainerListener *listener)
{
    auto ite = std::remove(listeners.begin(),listeners.end(),listener);
    listeners.erase(ite);
}

std::vector<Slot> Container::getInventory()
{
    std::vector<Slot> nonnulllist;

    for(int i = 0; i < inventorySlots.size(); ++i) 
    {
        nonnulllist.emplace_back(inventorySlots[i]->getStack());
    }

    return nonnulllist;
}

void Container::detectAndSendChanges()
{
    for(auto i = 0; i < inventorySlots.size(); ++i) 
    {
        ItemStack itemstack = inventorySlots[i]->getStack();
        ItemStack itemstack1 = inventoryItemStacks[i];
        if (!ItemStack::areItemStacksEqual(itemstack1, itemstack)) 
        {
            itemstack1 = itemstack.isEmpty() ? ItemStack::EMPTY : itemstack.copy();
            inventoryItemStacks[i] = itemstack1;

            for(auto j = 0; j < listeners.size(); ++j) 
            {
                listeners[j]->sendSlotContents(*this, i, itemstack1);
            }
        }
    }
}

bool Container::enchantItem(EntityPlayer *playerIn, int32_t id)
{
    return false;
}

std::optional<Slot> Container::getSlotFromInventory(const IInventory &inv, int32_t slotIn)
{
    for(auto i = 0; i < inventorySlots.size(); ++i) 
    {
        Slot slot = inventorySlots[i].value();
        if (slot.isHere(inv, slotIn)) 
        {
            return slot;
        }
    }

    return std::nullopt;
}

Slot Container::getSlot(int32_t slotId)
{
    return inventorySlots[slotId].value();
}

ItemStack Container::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    auto slot = inventorySlots[index];
    return slot.has_value() ? slot->getStack() : ItemStack::EMPTY;
}

ItemStack Container::slotClick(int32_t slotId, int32_t dragType, ClickType clickTypeIn, EntityPlayer *player)
{
    InventoryPlayer* inventoryplayer = player->inventory;
    if (clickTypeIn == ClickType::QUICK_CRAFT) 
    {
        auto j1 = dragEvent;
        dragEvent = getDragEvent(dragType);
        if ((j1 != 1 || dragEvent != 2) && j1 != dragEvent) 
        {
            resetDrag();
        }
        else if (inventoryplayer->getItemStack().isEmpty()) 
        {
            resetDrag();
        }
        else if (dragEvent == 0) 
        {
            dragMode = extractDragMode(dragType);
            if (isValidDragMode(dragMode, player)) 
            {
                dragEvent = 1;
                dragSlots.clear();
            }
            else 
            {
                resetDrag();
            }
        }
        else if (dragEvent == 1) 
        {
            auto slot7 = inventorySlots[slotId];
            auto itemstack = inventoryplayer->getItemStack();
            if (slot7.has_value() && canAddItemToSlot(slot7, itemstack, true) && slot7.isItemValid(itemstack) && (dragMode == 2 || itemstack.getCount() > dragSlots.size()) && canDragIntoSlot(slot7)) 
            {
                dragSlots.emplace(slot7);
            }
        }
        else if (dragEvent == 2) 
        {
            if (!dragSlots.empty()) 
            {
                auto itemstack = inventoryplayer->getItemStack().copy();
                auto k1 = inventoryplayer->getItemStack().getCount();
                auto var23 = dragSlots.begin();

                while(true) 
                {
                    Slot slot8;
                    ItemStack itemstack2 = ItemStack::EMPTY;
                    do 
                    {
                        do 
                        {
                            do 
                            {
                                do 
                                {
                                    if (!var23.hasNext()) 
                                    {
                                        itemstack2.setCount(k1);
                                        inventoryplayer->setItemStack(itemstack2);
                                        goto label359;
                                    }

                                    slot8 = (Slot)var23.next();
                                    itemstack2 = inventoryplayer->getItemStack();
                                }
                                while(slot8 == null);
                            }
                            while(!canAddItemToSlot(slot8, itemstack2, true));
                        }
                        while(!slot8.isItemValid(itemstack2));
                    }
                    while(dragMode != 2 && itemstack2.getCount() < dragSlots.size());

                    if (canDragIntoSlot(slot8)) 
                    {
                        ItemStack itemstack14 = itemstack2.copy();
                        int32_t j3 = slot8.getHasStack() ? slot8.getStack().getCount() : 0;
                        computeStackSize(dragSlots, dragMode, itemstack14, j3);
                        auto k3 = MathHelper::min(itemstack14.getMaxStackSize(), slot8.getItemStackLimit(itemstack14));
                        if (itemstack14.getCount() > k3) {
                            itemstack14.setCount(k3);
                        }

                        k1 -= itemstack14.getCount() - j3;
                        slot8.putStack(itemstack14);
                    }
                }
            }
        label359:
            resetDrag();
        }
        else 
        {
            resetDrag();
        }
    }
    else if (dragEvent != 0) 
    {
        resetDrag();
    }
    else 
    {
        if ((clickTypeIn == ClickType::PICKUP || clickTypeIn == ClickType::QUICK_MOVE) && (dragType == 0 || dragType == 1)) 
        {
            if (slotId == UNENDLICH) 
            {
                if (!inventoryplayer->getItemStack().isEmpty()) 
                {
                    if (dragType == 0) 
                    {
                        player->dropItem(inventoryplayer->getItemStack(), true);
                        inventoryplayer->setItemStack(ItemStack::EMPTY);
                    }

                    if (dragType == 1) 
                    {
                        player->dropItem(inventoryplayer->getItemStack().splitStack(1), true);
                    }
                }
            }
            else if (clickTypeIn == ClickType::QUICK_MOVE) 
            {
                if (slotId < 0) 
                {
                    return ItemStack::EMPTY;
                }

                slot6 = inventorySlots[slotId];
                if (slot6 == nullptr || !slot6.canTakeStack(player)) 
                {
                    return ItemStack::EMPTY;
                }

                for(ItemStack itemstack8 = transferStackInSlot(player, slotId); !itemstack8.isEmpty() && ItemStack::areItemsEqual(slot6.getStack(), itemstack8); itemstack8 = transferStackInSlot(player, slotId)) 
                {
                    itemstack = itemstack8.copy();
                }
            }
            else 
            {
                if (slotId < 0) 
                {
                    return ItemStack::EMPTY;
                }

                slot6 = (Slot)this.inventorySlots.get(slotId);
                if (slot6 != null) {
                    itemstack8 = slot6.getStack();
                    itemstack11 = inventoryplayer.getItemStack();
                    if (!itemstack8.isEmpty()) {
                        itemstack = itemstack8.copy();
                    }

                    if (itemstack8.isEmpty()) {
                        if (!itemstack11.isEmpty() && slot6.isItemValid(itemstack11)) {
                            k2 = dragType == 0 ? itemstack11.getCount() : 1;
                            if (k2 > slot6.getItemStackLimit(itemstack11)) {
                                k2 = slot6.getItemStackLimit(itemstack11);
                            }

                            slot6.putStack(itemstack11.splitStack(k2));
                        }
                    } else if (slot6.canTakeStack(player)) {
                        if (itemstack11.isEmpty()) {
                            if (itemstack8.isEmpty()) {
                                slot6.putStack(ItemStack.EMPTY);
                                inventoryplayer.setItemStack(ItemStack.EMPTY);
                            } else {
                                k2 = dragType == 0 ? itemstack8.getCount() : (itemstack8.getCount() + 1) / 2;
                                inventoryplayer.setItemStack(slot6.decrStackSize(k2));
                                if (itemstack8.isEmpty()) {
                                    slot6.putStack(ItemStack.EMPTY);
                                }

                                slot6.onTake(player, inventoryplayer.getItemStack());
                            }
                        } else if (slot6.isItemValid(itemstack11)) {
                            if (itemstack8.getItem() == itemstack11.getItem() && itemstack8.getMetadata() == itemstack11.getMetadata() && ItemStack.areItemStackTagsEqual(itemstack8, itemstack11)) {
                                k2 = dragType == 0 ? itemstack11.getCount() : 1;
                                if (k2 > slot6.getItemStackLimit(itemstack11) - itemstack8.getCount()) {
                                    k2 = slot6.getItemStackLimit(itemstack11) - itemstack8.getCount();
                                }

                                if (k2 > itemstack11.getMaxStackSize() - itemstack8.getCount()) {
                                    k2 = itemstack11.getMaxStackSize() - itemstack8.getCount();
                                }

                                itemstack11.shrink(k2);
                                itemstack8.grow(k2);
                            } else if (itemstack11.getCount() <= slot6.getItemStackLimit(itemstack11)) {
                                slot6.putStack(itemstack11);
                                inventoryplayer.setItemStack(itemstack8);
                            }
                        } else if (itemstack8.getItem() == itemstack11.getItem() && itemstack11.getMaxStackSize() > 1 && (!itemstack8.getHasSubtypes() || itemstack8.getMetadata() == itemstack11.getMetadata()) && ItemStack.areItemStackTagsEqual(itemstack8, itemstack11) && !itemstack8.isEmpty()) {
                            k2 = itemstack8.getCount();
                            if (k2 + itemstack11.getCount() <= itemstack11.getMaxStackSize()) {
                                itemstack11.grow(k2);
                                itemstack8 = slot6.decrStackSize(k2);
                                if (itemstack8.isEmpty()) {
                                    slot6.putStack(ItemStack.EMPTY);
                                }

                                slot6.onTake(player, inventoryplayer.getItemStack());
                            }
                        }
                    }

                    slot6.onSlotChanged();
                }
            }
        } else if (clickTypeIn == ClickType::SWAP && dragType >= 0 && dragType < 9) {
            slot6 = (Slot)inventorySlots.get(slotId);
            itemstack8 = inventoryplayer.getStackInSlot(dragType);
            itemstack11 = slot6.getStack();
            if (!itemstack8.isEmpty() || !itemstack11.isEmpty()) {
                if (itemstack8.isEmpty()) {
                    if (slot6.canTakeStack(player)) {
                        inventoryplayer.setInventorySlotContents(dragType, itemstack11);
                        slot6.onSwapCraft(itemstack11.getCount());
                        slot6.putStack(ItemStack::EMPTY);
                        slot6.onTake(player, itemstack11);
                    }
                } else if (itemstack11.isEmpty()) {
                    if (slot6.isItemValid(itemstack8)) {
                        k2 = slot6.getItemStackLimit(itemstack8);
                        if (itemstack8.getCount() > k2) {
                            slot6.putStack(itemstack8.splitStack(k2));
                        } else {
                            slot6.putStack(itemstack8);
                            inventoryplayer.setInventorySlotContents(dragType, ItemStack.EMPTY);
                        }
                    }
                } else if (slot6.canTakeStack(player) && slot6.isItemValid(itemstack8)) {
                    k2 = slot6.getItemStackLimit(itemstack8);
                    if (itemstack8.getCount() > k2) {
                        slot6.putStack(itemstack8.splitStack(k2));
                        slot6.onTake(player, itemstack11);
                        if (!inventoryplayer.addItemStackToInventory(itemstack11)) {
                            player.dropItem(itemstack11, true);
                        }
                    } else {
                        slot6.putStack(itemstack8);
                        inventoryplayer.setInventorySlotContents(dragType, itemstack11);
                        slot6.onTake(player, itemstack11);
                    }
                }
            }
        } else if (clickTypeIn == ClickType.CLONE && player.capabilities.isCreativeMode && inventoryplayer.getItemStack().isEmpty() && slotId >= 0) {
            slot6 = (Slot)inventorySlots.get(slotId);
            if (slot6 != null && slot6.getHasStack()) {
                itemstack8 = slot6.getStack().copy();
                itemstack8.setCount(itemstack8.getMaxStackSize());
                inventoryplayer.setItemStack(itemstack8);
            }
        } else if (clickTypeIn == ClickType.THROW && inventoryplayer.getItemStack().isEmpty() && slotId >= 0) {
            slot6 = (Slot)inventorySlots.get(slotId);
            if (slot6 != null && slot6.getHasStack() && slot6.canTakeStack(player)) {
                itemstack8 = slot6.decrStackSize(dragType == 0 ? 1 : slot6.getStack().getCount());
                slot6.onTake(player, itemstack8);
                player.dropItem(itemstack8, true);
            }
        } else if (clickTypeIn == ClickType.PICKUP_ALL && slotId >= 0) {
            slot6 = (Slot)inventorySlots.get(slotId);
            itemstack8 = inventoryplayer.getItemStack();
            if (!itemstack8.isEmpty() && (slot6 == null || !slot6.getHasStack() || !slot6.canTakeStack(player))) {
                k1 = dragType == 0 ? 0 : inventorySlots.size() - 1;
                k2 = dragType == 0 ? 1 : -1;

                for(int k = 0; k < 2; ++k) {
                    for(int l = k1; l >= 0 && l < inventorySlots.size() && itemstack8.getCount() < itemstack8.getMaxStackSize(); l += k2) {
                        Slot slot1 = (Slot)inventorySlots.get(l);
                        if (slot1.getHasStack() && canAddItemToSlot(slot1, itemstack8, true) && slot1.canTakeStack(player) && this.canMergeSlot(itemstack8, slot1)) {
                            ItemStack itemstack2 = slot1.getStack();
                            if (k != 0 || itemstack2.getCount() != itemstack2.getMaxStackSize()) {
                                k3 = Math.min(itemstack8.getMaxStackSize() - itemstack8.getCount(), itemstack2.getCount());
                                ItemStack itemstack3 = slot1.decrStackSize(k3);
                                itemstack8.grow(k3);
                                if (itemstack3.isEmpty()) {
                                    slot1.putStack(ItemStack.EMPTY);
                                }

                                slot1.onTake(player, itemstack3);
                            }
                        }
                    }
                }
            }

            detectAndSendChanges();
        }
    }

    return itemstack;
}

bool Container::canMergeSlot(ItemStack stack, Slot slotIn)
{
    return true;
}

void Container::onContainerClosed(EntityPlayer *playerIn)
{
    InventoryPlayer inventoryplayer = playerIn->inventory;
    if (!inventoryplayer.getItemStack().isEmpty()) 
    {
        playerIn->dropItem(inventoryplayer.getItemStack(), false);
        inventoryplayer.setItemStack(ItemStack::EMPTY);
    }
}

void Container::onCraftMatrixChanged(IInventory *inventoryIn)
{
    detectAndSendChanges();
}

void Container::putStackInSlot(int32_t slotID, ItemStack stack)
{
    getSlot(slotID).putStack(stack);;
}

void Container::setAll(std::vector<ItemStack> &p_190896_1_)
{
    for(auto i = 0; i < p_190896_1_.size(); ++i) 
    {
        getSlot(i).putStack(p_190896_1_[i]);
    }
}

void Container::updateProgressBar(int32_t id, int32_t data)
{
}

int16_t Container::getNextTransactionID(InventoryPlayer* invPlayer)
{
    ++transactionID;
    return transactionID;
}

bool Container::getCanCraft(EntityPlayer *player)
{
    return playerList.find(player) == playerList.end();
}

void Container::setCanCraft(EntityPlayer *player, bool canCraft)
{
    if (canCraft) 
    {
        playerList.erase(player);
    }
    else 
    {
        playerList.emplace(player);
    }
}

int32_t Container::extractDragMode(int32_t eventButton)
{
    return eventButton >> 2 & 3;
}

int32_t Container::getDragEvent(int32_t clickedButton)
{
    return clickedButton & 3;
}

int32_t Container::getQuickcraftMask(int32_t p_94534_0_, int32_t p_94534_1_)
{
    return p_94534_0_ & 3 | (p_94534_1_ & 3) << 2;
}

bool Container::isValidDragMode(int32_t dragModeIn, EntityPlayer *player)
{
    if (dragModeIn == 0) 
    {
        return true;
    }
    else if (dragModeIn == 1) 
    {
        return true;
    }
    else 
    {
        return dragModeIn == 2 && player->capabilities.isCreativeMode;
    }
}

bool Container::canAddItemToSlot(std::optional<Slot> slotIn, ItemStack stack, bool stackSizeMatters)
{
    bool flag = !slotIn.has_value() || !slotIn->getHasStack();
    if (!flag && stack.isItemEqual(slotIn->getStack()) && ItemStack::areItemStackTagsEqual(slotIn->getStack(), stack)) 
    {
        return slotIn->getStack().getCount() + (stackSizeMatters ? 0 : stack.getCount()) <= stack.getMaxStackSize();
    }
    else 
    {
        return flag;
    }
}

void Container::computeStackSize(std::unordered_set<Slot> &dragSlotsIn, int32_t dragModeIn, ItemStack stack,
    int32_t slotStackSize)
{
    switch(dragModeIn)
    {
    case 0:
        stack.setCount(MathHelper::floor((float)stack.getCount() / (float)dragSlotsIn.size()));
        break;
    case 1:
        stack.setCount(1);
        break;
    case 2:
        stack.setCount(stack.getItem()->getItemStackLimit());
    }

    stack.grow(slotStackSize);
}

bool Container::canDragIntoSlot(Slot slotIn)
{
    return true;
}

int32_t Container::calcRedstone(TileEntity *te) const
{
    return Util::instanceof<IInventory>(te)? calcRedstoneFromInventory((IInventory*)te) : 0;
}

int32_t Container::calcRedstoneFromInventory(IInventory *inv) 
{
    if (inv == nullptr) 
    {
        return 0;
    }
    else 
    {
        auto i = 0;
        auto f = 0.0F;

        for(auto j = 0; j < inv->getSizeInventory(); ++j) 
        {
            ItemStack itemstack = inv->getStackInSlot(j);
            if (!itemstack.isEmpty()) 
            {
                f += itemstack.getCount() / MathHelper::min(inv->getInventoryStackLimit(), itemstack.getMaxStackSize());
                ++i;
            }
        }

        f /= inv->getSizeInventory();
        return MathHelper::floor(f * 14.0F) + (i > 0 ? 1 : 0);
    }
}

Slot Container::addSlotToContainer(const Slot& slotIn)
{
    slotIn.slotNumber = inventorySlots.size();
    inventorySlots.emplace_back(slotIn);
    inventoryItemStacks.emplace_back(ItemStack::EMPTY);
    return slotIn;
}

void Container::clearContainer(EntityPlayer *playerIn, World *worldIn, IInventory *inventoryIn) const
{
    int32_t i = 0;
    if (!playerIn->isEntityAlive() || Util::instanceof <EntityPlayerMP>(playerIn) && ((EntityPlayerMP*)playerIn)->hasDisconnected()) 
    {
        for(i = 0; i < inventoryIn->getSizeInventory(); ++i) 
        {
            playerIn->dropItem(inventoryIn->removeStackFromSlot(i), false);
        }
    }
    else 
    {
        for(i = 0; i < inventoryIn->getSizeInventory(); ++i) 
        {
            playerIn->inventory.placeItemBackInInventory(worldIn, inventoryIn->removeStackFromSlot(i));
        }
    }
}

bool Container::mergeItemStack(ItemStack stack, int32_t startIndex, int32_t endIndex, bool reverseDirection)
{
    bool flag = false;
    int32_t i = startIndex;
    if (reverseDirection) 
    {
        i = endIndex - 1;
    }

    if (stack.isStackable()) 
    {
        while(!stack.isEmpty()) 
        {
            if (reverseDirection) 
            {
                if (i < startIndex) 
                {
                    break;
                }
            }
            else if (i >= endIndex) 
            {
                break;
            }

            Slot slot1 = inventorySlots[i].value();
            ItemStack itemstack = slot1.getStack();
            if (!itemstack.isEmpty() && itemstack.getItem() == stack.getItem() && (!stack.getHasSubtypes() || stack.getMetadata() == itemstack.getMetadata()) && ItemStack::areItemStackTagsEqual(stack, itemstack)) 
            {
                int32_t j = itemstack.getCount() + stack.getCount();
                if (j <= stack.getMaxStackSize()) 
                {
                    stack.setCount(0);
                    itemstack.setCount(j);
                    slot1.onSlotChanged();
                    flag = true;
                }
                else if (itemstack.getCount() < stack.getMaxStackSize()) 
                {
                    stack.shrink(stack.getMaxStackSize() - itemstack.getCount());
                    itemstack.setCount(stack.getMaxStackSize());
                    slot1.onSlotChanged();
                    flag = true;
                }
            }

            if (reverseDirection) 
            {
                --i;
            }
            else 
            {
                ++i;
            }
        }
    }

    if (!stack.isEmpty()) 
    {
        if (reverseDirection) 
        {
            i = endIndex - 1;
        }
        else 
        {
            i = startIndex;
        }

        while(true) 
        {
            if (reverseDirection) 
            {
                if (i < startIndex) 
                {
                    break;
                }
            }
            else if (i >= endIndex) 
            {
                break;
            }

            Slot slot1 = inventorySlots[i].value();
            ItemStack itemstack = slot1.getStack();
            if (itemstack.isEmpty() && slot1.isItemValid(stack)) 
            {
                if (stack.getCount() > slot1.getSlotStackLimit()) 
                {
                    slot1.putStack(stack.splitStack(slot1.getSlotStackLimit()));
                }
                else 
                {
                    slot1.putStack(stack.splitStack(stack.getCount()));
                }

                slot1.onSlotChanged();
                flag = true;
                break;
            }

            if (reverseDirection) 
            {
                --i;
            }
            else 
            {
                ++i;
            }
        }
    }

    return flag;
}

void Container::resetDrag()
{
    dragEvent = 0;
    dragSlots.clear();
}

void Container::slotChangedCraftingGrid(World *p_192389_1_, EntityPlayer *p_192389_2_, InventoryCrafting *p_192389_3_,
    InventoryCraftResult *p_192389_4_)
{
    if (!p_192389_1_->isRemote) 
    {
        EntityPlayerMP* entityplayermp = (EntityPlayerMP*)p_192389_2_;
        ItemStack itemstack = ItemStack::EMPTY;
        IRecipe* irecipe = CraftingManager::findMatchingRecipe(p_192389_3_, p_192389_1_);
        if (irecipe != nullptr && (irecipe->isDynamic() || !p_192389_1_->getGameRules().getBoolean("doLimitedCrafting") || entityplayermp->getRecipeBook().isUnlocked(irecipe))) 
        {
            p_192389_4_->setRecipeUsed(irecipe);
            itemstack = irecipe->getCraftingResult(p_192389_3_);
        }

        p_192389_4_->setInventorySlotContents(0, itemstack);
        entityplayermp->connection.sendPacket(SPacketSetSlot(windowId, 0, itemstack));
    }
}
Container::~Container() = default;
