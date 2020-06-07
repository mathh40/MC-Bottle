#include "ContainerRepair.h"



#include "IContainerListener.h"
#include "StringUtils.h"
#include "../../../../spdlog/include/spdlog/spdlog.h"
#include "../block/BlockAnvil.h"
#include "../item/ItemEnchantedBook.h"
#include "../world/World.h"
#include "math/MathHelper.h"

std::shared_ptr<spdlog::logger> ContainerRepair::LOGGER = spdlog::get("Minecraft")->clone("ContainerRepair");

class InventoryRepair : public InventoryBasic
{
public:
    InventoryRepair(ContainerRepair* repair);
    void markDirty() override;
private:
    ContainerRepair* container;
};

InventoryRepair::InventoryRepair(ContainerRepair* repair)
    :InventoryBasic("Repair", true, 2),container(repair)
{
}

void InventoryRepair::markDirty()
{
    InventoryBasic::markDirty();
    container->onCraftMatrixChanged(*this);
}

class ResultSlot : Slot
{
public:
    bool isItemValid(ItemStack stack) override
    {
        return false;
    }

    bool canTakeStack(EntityPlayer* playerIn) override
    {
        return (playerIn->capabilities.isCreativeMode || playerIn->experienceLevel >= container->maximumCost) && container->maximumCost > 0 && getHasStack();
    }

    ItemStack onTake(EntityPlayer* thePlayer, ItemStack stack) override
    {
        if (!thePlayer->capabilities.isCreativeMode) 
        {
            thePlayer->addExperienceLevel(container->maximumCost);
        }

        container->inputSlots->setInventorySlotContents(0, ItemStack::EMPTY);
        if (container->materialCost > 0) 
        {
            ItemStack itemstack = container->inputSlots->getStackInSlot(1);
            if (!itemstack.isEmpty() && itemstack.getCount() > container->materialCost) 
            {
                itemstack.shrink(container->materialCost);
                container->inputSlots->setInventorySlotContents(1, itemstack);
            }
            else 
            {
                container->inputSlots->setInventorySlotContents(1, ItemStack::EMPTY);
            }
        }
        else 
        {
            container->inputSlots->setInventorySlotContents(1, ItemStack::EMPTY);
        }

        container->maximumCost = 0;
        IBlockState* iblockstate = worldIn->getBlockState(blockPosIn);
        if (!thePlayer->capabilities.isCreativeMode && !worldIn->isRemote && iblockstate->getBlock() == Blocks::ANVIL && thePlayer->getRNG().nextFloat() < 0.12F) 
        {
            int32_t l = iblockstate->getValue(BlockAnvil::DAMAGE);
            ++l;
            if (l > 2) 
            {
                worldIn->setBlockToAir(blockPosIn);
                worldIn->playEvent(1029, blockPosIn, 0);
            }
            else 
            {
                worldIn->setBlockState(blockPosIn, iblockstate->withProperty(BlockAnvil::DAMAGE, l), 2);
                worldIn->playEvent(1030, blockPosIn, 0);
            }
        }
        else if (!worldIn->isRemote) 
        {
            worldIn->playEvent(1030, blockPosIn, 0);
        }

        return stack;
    }
    ResultSlot(ContainerRepair* repair, IInventory* inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition);
private:
    ContainerRepair* container;
};

ResultSlot::ResultSlot(ContainerRepair* repair,IInventory *inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition)
    :Slot(inventoryIn, index, xPosition, yPosition),container(repair)
{
}


ContainerRepair::ContainerRepair(InventoryPlayer *playerInventory, World *worldIn, EntityPlayer *player)
    :ContainerRepair(playerInventory, worldIn, BlockPos::ORIGIN, player)
{
}

ContainerRepair::ContainerRepair(InventoryPlayer *playerInventory, const World *worldIn, const BlockPos &blockPosIn,
    EntityPlayer *player)
        :outputSlot(new InventoryCraftResult()),inputSlots(new InventoryRepair(this)),pos(blockPosIn),world(worldIn)
        ,player(player)
{
    addSlotToContainer(Slot(inputSlots, 0, 27, 47));
    addSlotToContainer(Slot(inputSlots, 1, 76, 47));
    addSlotToContainer(ResultSlot(outputSlot, 2, 134, 47));

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

void ContainerRepair::onCraftMatrixChanged(IInventory *inventoryIn)
{
    Container::onCraftMatrixChanged(inventoryIn);
    if (inventoryIn == inputSlots) 
    {
        updateRepairOutput();
    }
}

void ContainerRepair::updateRepairOutput()
{
    ItemStack itemstack = inputSlots->getStackInSlot(0);
    maximumCost = 1;
    if (itemstack.isEmpty()) 
    {
        outputSlot->setInventorySlotContents(0, ItemStack::EMPTY);
        maximumCost = 0;
    }
    else 
    {
        ItemStack itemstack1 = itemstack.copy();
        ItemStack itemstack2 = inputSlots->getStackInSlot(1);
        auto map = EnchantmentHelper::getEnchantments(itemstack1);
        int32_t j = 0;
        int32_t i = 0;
        j = j + itemstack.getRepairCost() + (itemstack2.isEmpty() ? 0 : itemstack2.getRepairCost());
        materialCost = 0;
        if (!itemstack2.isEmpty()) 
        {
            bool flag = itemstack2.getItem() == Items::ENCHANTED_BOOK && !ItemEnchantedBook::getEnchantments(itemstack2)->isEmpty();
            if (itemstack1.isItemStackDamageable() && itemstack1.getItem()->getIsRepairable(itemstack, itemstack2)) 
            {
                auto l = MathHelper::min(itemstack1.getItemDamage(), itemstack1.getMaxDamage() / 4);
                if (l <= 0) 
                {
                    outputSlot->setInventorySlotContents(0, ItemStack::EMPTY);
                    maximumCost = 0;
                    return;
                }
                auto i1 = 0;
                for(;l > 0 && i1 < itemstack2.getCount(); ++i1) 
                {
                    auto j1 = itemstack1.getItemDamage() - l;
                    itemstack1.setItemDamage(j1);
                    ++i;
                    l = MathHelper::min(itemstack1.getItemDamage(), itemstack1.getMaxDamage() / 4);
                }

                materialCost = i1;
            }
            else 
            {
                if (!flag && (itemstack1.getItem() != itemstack2.getItem() || !itemstack1.isItemStackDamageable())) 
                {
                    outputSlot->setInventorySlotContents(0, ItemStack::EMPTY);
                    maximumCost = 0;
                    return;
                }

                if (itemstack1.isItemStackDamageable() && !flag) {
                    auto l = itemstack.getMaxDamage() - itemstack.getItemDamage();
                    auto i1 = itemstack2.getMaxDamage() - itemstack2.getItemDamage();
                    auto j1 = i1 + itemstack1.getMaxDamage() * 12 / 100;
                    auto k1 = l + j1;
                    auto l1 = itemstack1.getMaxDamage() - k1;
                    if (l1 < 0) 
                    {
                        l1 = 0;
                    }

                    if (l1 < itemstack1.getMetadata()) 
                    {
                        itemstack1.setItemDamage(l1);
                        i += 2;
                    }
                }

                auto map1 = EnchantmentHelper::getEnchantments(itemstack2);
                bool flag2 = false;
                bool flag3 = false;
                Iterator var24 = map1.keySet().iterator();

            
                while(true) 
                {
                    Enchantment enchantment1;
                    do 
                    {
                        if (!var24.hasNext()) 
                        {
                            if (flag3 && !flag2) 
                            {
                                outputSlot->setInventorySlotContents(0, ItemStack::EMPTY);
                                maximumCost = 0;
                                return;
                            }
                            goto label168;
                        }

                        enchantment1 = (Enchantment)var24.next();
                    }
                    while(enchantment1 == nullptr);

                    auto i2 = map.containsKey(enchantment1) ? map.get(enchantment1) : 0;
                    auto j2 = map1.get(enchantment1);
                    j2 = i2 == j2 ? j2 + 1 : MathHelper::max(j2, i2);
                    bool flag1 = enchantment1.canApply(itemstack);
                    if (player->capabilities.isCreativeMode || itemstack.getItem() == Items::ENCHANTED_BOOK) 
                    {
                        flag1 = true;
                    }

                    Iterator var17 = map.keySet().iterator();

                    while(var17.hasNext()) 
                    {
                        Enchantment enchantment = (Enchantment)var17.next();
                        if (enchantment != enchantment1 && !enchantment1.isCompatibleWith(enchantment)) 
                        {
                            flag1 = false;
                            ++i;
                        }
                    }

                    if (!flag1) 
                    {
                        flag3 = true;
                    }
                    else 
                    {
                        flag2 = true;
                        if (j2 > enchantment1.getMaxLevel()) 
                        {
                            j2 = enchantment1.getMaxLevel();
                        }

                        map.put(enchantment1, j2);
                        auto k3 = 0;
                        switch(enchantment1.getRarity())
                        {
                        case EnumRarity::COMMON:
                            k3 = 1;
                            break;
                        case EnumRarity::UNCOMMON:
                            k3 = 2;
                            break;
                        case EnumRarity::RARE:
                            k3 = 4;
                            break;
                        case EnumRarity::VERY_RARE:
                            k3 = 8;
                        }

                        if (flag) 
                        {
                            k3 = MathHelper::max(1, k3 / 2);
                        }

                        i += k3 * j2;
                        if (itemstack.getCount() > 1) 
                        {
                            i = 40;
                        }
                    }
                }           
            }
        }
        label168:
        auto k = 0;
        if (StringUtils::isBlank(repairedItemName)) 
        {
            if (itemstack.hasDisplayName()) 
            {
                k = 1;
                i += k;
                itemstack1.clearCustomName();
            }
        }
        else if (!(repairedItemName == itemstack.getDisplayName()))
        {
            k = 1;
            i += k;
            itemstack1.setStackDisplayName(repairedItemName);
        }

        maximumCost = j + i;
        if (i <= 0) 
        {
            itemstack1 = ItemStack::EMPTY;
        }

        if (k == i && k > 0 && maximumCost >= 40) 
        {
            maximumCost = 39;
        }

        if (maximumCost >= 40 && !player->capabilities.isCreativeMode) 
        {
            itemstack1 = ItemStack::EMPTY;
        }

        if (!itemstack1.isEmpty()) 
        {
            auto k2 = itemstack1.getRepairCost();
            if (!itemstack2.isEmpty() && k2 < itemstack2.getRepairCost()) 
            {
                k2 = itemstack2.getRepairCost();
            }

            if (k != i || k == 0) 
            {
                k2 = k2 * 2 + 1;
            }

            itemstack1.setRepairCost(k2);
            EnchantmentHelper::setEnchantments(map, itemstack1);
        }

        outputSlot->setInventorySlotContents(0, itemstack1);
        detectAndSendChanges();
    }
}

void ContainerRepair::addListener(IContainerListener *listener)
{
    Container::addListener(listener);
    listener->sendWindowProperty(*this, 0, maximumCost);
}

void ContainerRepair::updateProgressBar(int32_t id, int32_t data)
{
    if (id == 0) 
    {
        maximumCost = data;
    }
}

void ContainerRepair::onContainerClosed(EntityPlayer *playerIn)
{
    Container::onContainerClosed(playerIn);
    if (!world->isRemote) 
    {
        clearContainer(playerIn, world, inputSlots);
    }
}

bool ContainerRepair::canInteractWith(EntityPlayer *playerIn)
{
    if (world->getBlockState(pos)->getBlock() != Blocks::ANVIL) 
    {
        return false;
    }
    else 
    {
        return playerIn->getDistanceSq(pos.getx() + 0.5, pos.gety() + 0.5, pos.getz() + 0.5) <= 64.0;
    }
}

ItemStack ContainerRepair::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack()) 
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack = itemstack1.copy();
        if (index == 2) 
        {
            if (!mergeItemStack(itemstack1, 3, 39, true)) 
            {
                return ItemStack::EMPTY;
            }

            slot->onSlotChange(itemstack1, itemstack);
        }
        else if (index != 0 && index != 1) 
        {
            if (index >= 3 && index < 39 && !mergeItemStack(itemstack1, 0, 2, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (!mergeItemStack(itemstack1, 3, 39, false)) 
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

        slot->onTake(playerIn, itemstack1);
    }

    return itemstack;
}

void ContainerRepair::updateItemName(std::string_view newName)
{
    repairedItemName = newName;
    if (getSlot(2).getHasStack()) 
    {
        ItemStack itemstack = getSlot(2).getStack();
        if (StringUtils::isBlank(newName)) 
        {
            itemstack.clearCustomName();
        }
        else 
        {
            itemstack.setStackDisplayName(repairedItemName);
        }
    }

    updateRepairOutput();
}
