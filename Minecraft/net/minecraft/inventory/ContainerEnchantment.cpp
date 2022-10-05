#include "ContainerEnchantment.h"

#include "../item/EnumDyeColor.h"
#include "../item/ItemEnchantedBook.h"
#include "../stats/StatList.h"
#include "IContainerListener.h"
#include "IInventory.h"
#include "SoundCategory.h"
#include "Util.h"
#include "World.h"

class InventoryBasic
{
    int getInventoryStackLimit()
    {
        return 64;
    }

    void markDirty()
    {
        super.markDirty();
        ContainerEnchantment.this.onCraftMatrixChanged(this);
    }
};

class EnchantSlot : public Slot
{
  public:
    EnchantSlot(IInventory *inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition);

    bool isItemValid(ItemStack stack) override
    {
        return true;
    }

    int32_t getSlotStackLimit() const override
    {
        return 1;
    }
};

class TributSlot : public Slot
{
  public:
    TributSlot(IInventory *inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition);

    bool isItemValid(ItemStack stack)
    {
        return stack.getItem() == Items::DYE && EnumDyeColor::byDyeDamage(stack.getMetadata()) == EnumDyeColor::BLUE;
    }
};

EnchantSlot::EnchantSlot(IInventory *inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition)
    : Slot(inventoryIn, index, xPosition, yPosition)
{
}

TributSlot::TributSlot(IInventory *inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition)
    : Slot(inventoryIn, index, xPosition, yPosition)
{
}

ContainerEnchantment::ContainerEnchantment(InventoryPlayer *playerInv, World *worldIn)
    : ContainerEnchantment(playerInv, worldIn, BlockPos::ORIGIN)
{
}

ContainerEnchantment::ContainerEnchantment(InventoryPlayer *playerInv, World *worldIn, BlockPos pos)
    : rand(), enchantLevels(3, 0), enchantClue({-1, -1, -1}), worldClue({-1, -1, -1}), world(worldIn), position(pos),
      xpSeed(playerInv->player->getXPSeed())
{
    tableInventory = new InventoryBasic("Enchant", true, 2);
    addSlotToContainer(EnchantSlot(tableInventory, 0, 15, 47));
    addSlotToContainer(Slot(tableInventory, 1, 35, 47));

    for (auto k = 0; k < 3; ++k)
    {
        for (auto j = 0; j < 9; ++j)
        {
            addSlotToContainer(Slot(playerInv, j + k * 9 + 9, 8 + j * 18, 84 + k * 18));
        }
    }

    for (auto k = 0; k < 9; ++k)
    {
        addSlotToContainer(Slot(playerInv, k, 8 + k * 18, 142));
    }
}

void ContainerEnchantment::addListener(IContainerListener *listener)
{
    Container::addListener(listener);
    broadcastData(listener);
}

void ContainerEnchantment::detectAndSendChanges()
{
    Container::detectAndSendChanges();

    for (auto i = 0; i < listeners.size(); ++i)
    {
        IContainerListener *icontainerlistener = listeners[i];
        broadcastData(icontainerlistener);
    }
}

void ContainerEnchantment::updateProgressBar(int32_t id, int32_t data)
{
    if (id >= 0 && id <= 2)
    {
        enchantLevels[id] = data;
    }
    else if (id == 3)
    {
        xpSeed = data;
    }
    else if (id >= 4 && id <= 6)
    {
        enchantClue[id - 4] = data;
    }
    else if (id >= 7 && id <= 9)
    {
        worldClue[id - 7] = data;
    }
    else
    {
        Container::updateProgressBar(id, data);
    }
}

void ContainerEnchantment::onCraftMatrixChanged(IInventory *inventoryIn)
{
    if (inventoryIn == tableInventory)
    {
        ItemStack itemstack = inventoryIn->getStackInSlot(0);
        if (!itemstack.isEmpty() && itemstack.isItemEnchantable())
        {
            if (!world->isRemote)
            {
                auto l = 0;

                for (auto j = -1; j <= 1; ++j)
                {
                    for (auto k = -1; k <= 1; ++k)
                    {
                        if ((j != 0 || k != 0) && world->isAirBlock(position.add(k, 0, j)) &&
                            world->isAirBlock(position.add(k, 1, j)))
                        {
                            if (world->getBlockState(position.add(k * 2, 0, j * 2)).getBlock() == Blocks::BOOKSHELF)
                            {
                                ++l;
                            }

                            if (world->getBlockState(position.add(k * 2, 1, j * 2)).getBlock() == Blocks::BOOKSHELF)
                            {
                                ++l;
                            }

                            if (k != 0 && j != 0)
                            {
                                if (world->getBlockState(position.add(k * 2, 0, j)).getBlock() == Blocks::BOOKSHELF)
                                {
                                    ++l;
                                }

                                if (world->getBlockState(position.add(k * 2, 1, j)).getBlock() == Blocks::BOOKSHELF)
                                {
                                    ++l;
                                }

                                if (world->getBlockState(position.add(k, 0, j * 2)).getBlock() == Blocks::BOOKSHELF)
                                {
                                    ++l;
                                }

                                if (world->getBlockState(position.add(k, 1, j * 2)).getBlock() == Blocks::BOOKSHELF)
                                {
                                    ++l;
                                }
                            }
                        }
                    }
                }

                rand.seed(xpSeed);

                for (auto j = 0; j < 3; ++j)
                {
                    enchantLevels[j] = EnchantmentHelper::calcItemStackEnchantability(rand, j, l, itemstack);
                    enchantClue[j]   = -1;
                    worldClue[j]     = -1;
                    if (enchantLevels[j] < j + 1)
                    {
                        enchantLevels[j] = 0;
                    }
                }

                for (auto j = 0; j < 3; ++j)
                {
                    if (enchantLevels[j] > 0)
                    {
                        auto list = getEnchantmentList(itemstack, j, enchantLevels[j]);
                        if (!list.empty())
                        {
                            EnchantmentData enchantmentdata = list.get(rand(list.size()));
                            enchantClue[j] = Enchantment::getEnchantmentID(enchantmentdata.enchantment);
                            worldClue[j]   = enchantmentdata.enchantmentLevel;
                        }
                    }
                }

                setectAndSendChanges();
            }
        }
        else
        {
            for (auto l = 0; l < 3; ++l)
            {
                enchantLevels[l] = 0;
                enchantClue[l]   = -1;
                worldClue[l]     = -1;
            }
        }
    }
}

bool ContainerEnchantment::enchantItem(EntityPlayer *playerIn, int32_t id)
{
    ItemStack itemstack  = tableInventory->getStackInSlot(0);
    ItemStack itemstack1 = tableInventory->getStackInSlot(1);
    int32_t i            = id + 1;
    if ((itemstack1.isEmpty() || itemstack1.getCount() < i) && !playerIn->capabilities.isCreativeMode)
    {
        return false;
    }
    else if (enchantLevels[id] <= 0 || itemstack.isEmpty() ||
             (playerIn->experienceLevel < i || playerIn->experienceLevel < enchantLevels[id]) &&
                 !playerIn->capabilities.isCreativeMode)
    {
        return false;
    }
    else
    {
        if (!world->isRemote)
        {
            List list = getEnchantmentList(itemstack, id, enchantLevels[id]);
            if (!list.isEmpty())
            {
                playerIn->onEnchant(itemstack, i);
                bool flag = itemstack.getItem() == Items::BOOK;
                if (flag)
                {
                    itemstack = ItemStack(Items::ENCHANTED_BOOK);
                    tableInventory->setInventorySlotContents(0, itemstack);
                }

                for (auto j = 0; j < list.size(); ++j)
                {
                    EnchantmentData enchantmentdata = list[j];
                    if (flag)
                    {
                        ItemEnchantedBook::addEnchantment(itemstack, enchantmentdata);
                    }
                    else
                    {
                        itemstack.addEnchantment(enchantmentdata.enchantment, enchantmentdata.enchantmentLevel);
                    }
                }

                if (!playerIn->capabilities.isCreativeMode)
                {
                    itemstack1.shrink(i);
                    if (itemstack1.isEmpty())
                    {
                        tableInventory->setInventorySlotContents(1, ItemStack::EMPTY);
                    }
                }

                playerIn->addStat(StatList::ITEM_ENCHANTED);
                if (Util:: instanceof <EntityPlayerMP>(playerIn))
                {
                    CriteriaTriggers::ENCHANTED_ITEM.trigger((EntityPlayerMP *)playerIn, itemstack, i);
                }

                tableInventory->markDirty();
                xpSeed = playerIn->getXPSeed();
                onCraftMatrixChanged(tableInventory);
                world->playSound(nullptr, position, SoundEvents::BLOCK_ENCHANTMENT_TABLE_USE, SoundCategory::BLOCKS,
                                 1.0F, world->rand.nextFloat() * 0.1F + 0.9F);
            }
        }

        return true;
    }
}

int32_t ContainerEnchantment::getLapisAmount() const
{
    const ItemStack itemstack = tableInventory->getStackInSlot(1);
    return itemstack.isEmpty() ? 0 : itemstack.getCount();
}

void ContainerEnchantment::onContainerClosed(EntityPlayer *playerIn)
{
    Container::onContainerClosed(playerIn);
    if (!world->isRemote)
    {
        clearContainer(playerIn, playerIn->world, tableInventory);
    }
}

bool ContainerEnchantment::canInteractWith(EntityPlayer *playerIn)
{
    if (world->getBlockState(position)->getBlock() != Blocks::ENCHANTING_TABLE)
    {
        return false;
    }
    else
    {
        return playerIn->getDistanceSq(position.getx() + 0.5, position.gety() + 0.5, position.getz() + 0.5) <= 64.0;
    }
}

ItemStack ContainerEnchantment::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot           = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack())
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack            = itemstack1.copy();
        if (index == 0)
        {
            if (!mergeItemStack(itemstack1, 2, 38, true))
            {
                return ItemStack::EMPTY;
            }
        }
        else if (index == 1)
        {
            if (!mergeItemStack(itemstack1, 2, 38, true))
            {
                return ItemStack::EMPTY;
            }
        }
        else if (itemstack1.getItem() == Items::DYE &&
                 EnumDyeColor::byDyeDamage(itemstack1.getMetadata()) == EnumDyeColor::BLUE)
        {
            if (!mergeItemStack(itemstack1, 1, 2, true))
            {
                return ItemStack::EMPTY;
            }
        }
        else
        {
            if (inventorySlots[0]->getHasStack() || !inventorySlots[0]->isItemValid(itemstack1))
            {
                return ItemStack::EMPTY;
            }

            if (itemstack1.hasTagCompound() && itemstack1.getCount() == 1)
            {
                inventorySlots[0]->putStack(itemstack1.copy());
                itemstack1.setCount(0);
            }
            else if (!itemstack1.isEmpty())
            {
                inventorySlots[0]->putStack(ItemStack(itemstack1.getItem(), 1, itemstack1.getMetadata()));
                itemstack1.shrink(1);
            }
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

void ContainerEnchantment::broadcastData(IContainerListener *crafting)
{
    crafting->sendWindowProperty(*this, 0, enchantLevels[0]);
    crafting->sendWindowProperty(*this, 1, enchantLevels[1]);
    crafting->sendWindowProperty(*this, 2, enchantLevels[2]);
    crafting->sendWindowProperty(*this, 3, xpSeed & -16);
    crafting->sendWindowProperty(*this, 4, enchantClue[0]);
    crafting->sendWindowProperty(*this, 5, enchantClue[1]);
    crafting->sendWindowProperty(*this, 6, enchantClue[2]);
    crafting->sendWindowProperty(*this, 7, worldClue[0]);
    crafting->sendWindowProperty(*this, 8, worldClue[1]);
    crafting->sendWindowProperty(*this, 9, worldClue[2]);
}

std::vector<> ContainerEnchantment::getEnchantmentList(ItemStack stack, int32_t enchantSlot, int32_t level)
{
    rand.seed(xpSeed + enchantSlot);
    std::vector<> list = EnchantmentHelper::buildEnchantmentList(rand, stack, level, false);
    if (stack.getItem() == Items::BOOK && list.size() > 1)
    {
        list.remove(rand(list.size()));
    }

    return list;
}
