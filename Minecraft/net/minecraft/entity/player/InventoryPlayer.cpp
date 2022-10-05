#include "InventoryPlayer.h"

#include "../../inventory/InventoryHelper.h"
#include "../../inventory/ItemStackHelper.h"
#include "../../item/ItemArmor.h"
#include "../EntityLeashKnot.h"
#include "ReportedException.h"
#include "text/TextComponentString.h"
#include "text/TextComponentTranslation.h"

InventoryPlayer::InventoryPlayer(EntityPlayer *playerIn)
    : mainInventory(36, ItemStack::EMPTY), armorInventory(4, ItemStack::EMPTY), offHandInventory(1, ItemStack::EMPTY),
      allInventories({&mainInventory, &armorInventory, &offHandInventory}), itemStack(ItemStack::EMPTY),
      player(playerIn)
{
}

ItemStack InventoryPlayer::getCurrentItem()
{
    return isHotbar(currentItem) ? mainInventory[currentItem] : ItemStack::EMPTY;
}

int32_t InventoryPlayer::getHotbarSize()
{
    return 9;
}

int32_t InventoryPlayer::getFirstEmptyStack()
{
    for (int i = 0; i < mainInventory.size(); ++i)
    {
        if (mainInventory[i].isEmpty())
        {
            return i;
        }
    }

    return -1;
}

void InventoryPlayer::setPickedItemStack(ItemStack stack)
{
    auto i = getSlotFor(stack);
    if (isHotbar(i))
    {
        currentItem = i;
    }
    else if (i == -1)
    {
        currentItem = getBestHotbarSlot();
        if (!mainInventory[currentItem].isEmpty())
        {
            auto j = getFirstEmptyStack();
            if (j != -1)
            {
                mainInventory[j] = mainInventory[currentItem];
            }
        }

        mainInventory[currentItem] = stack;
    }
    else
    {
        pickItem(i);
    }
}

void InventoryPlayer::pickItem(int32_t index)
{
    currentItem                = getBestHotbarSlot();
    ItemStack itemstack        = mainInventory[currentItem];
    mainInventory[currentItem] = mainInventory[index];
    mainInventory[index]       = itemstack;
}

constexpr bool InventoryPlayer::isHotbar(int32_t index)
{
    return index >= 0 && index < 9;
}

int32_t InventoryPlayer::getSlotFor(ItemStack stack)
{
    for (auto i = 0; i < mainInventory.size(); ++i)
    {
        if (!mainInventory[i].isEmpty() && InventoryHelper::stackEqualExact(stack, mainInventory[i]))
        {
            return i;
        }
    }

    return -1;
}

int32_t InventoryPlayer::findSlotMatchingUnusedItem(ItemStack p_194014_1_)
{
    for (auto i = 0; i < mainInventory.size(); ++i)
    {
        ItemStack itemstack = mainInventory[i];
        if (!mainInventory[i].isEmpty() && InventoryHelper::stackEqualExact(p_194014_1_, mainInventory[i]) &&
            !mainInventory[i].isItemDamaged() && !itemstack.isItemEnchanted() && !itemstack.hasDisplayName())
        {
            return i;
        }
    }

    return -1;
}

int32_t InventoryPlayer::getBestHotbarSlot()
{
    for (auto k = 0; k < 9; ++k)
    {
        auto l = (currentItem + k) % 9;
        if (mainInventory[l].isEmpty())
        {
            return l;
        }
    }

    for (auto k = 0; k < 9; ++k)
    {
        auto l = (currentItem + k) % 9;
        if (!mainInventory[l].isItemEnchanted())
        {
            return l;
        }
    }

    return currentItem;
}

void InventoryPlayer::changeCurrentItem(int32_t direction)
{
    if (direction > 0)
    {
        direction = 1;
    }

    if (direction < 0)
    {
        direction = -1;
    }

    for (currentItem -= direction; currentItem < 0; currentItem += 9)
    {
    }

    while (currentItem >= 9)
    {
        currentItem -= 9;
    }
}

int32_t InventoryPlayer::clearMatchingItems(Item *itemIn, int32_t metadataIn, int32_t removeCount,
                                            NBTTagCompound *itemNBT)
{
    auto i = 0;
    for (auto l = 0; l < getSizeInventory(); ++l)
    {
        ItemStack itemstack = getStackInSlot(l);
        if (!itemstack.isEmpty() && (itemIn == nullptr || itemstack.getItem() == itemIn) &&
            (metadataIn <= -1 || itemstack.getMetadata() == metadataIn) &&
            (itemNBT == nullptr || NBTUtil::areNBTEquals(itemNBT, itemstack.getTagCompound(), true)))
        {
            int k = removeCount <= 0 ? itemstack.getCount() : MathHelper::min(removeCount - i, itemstack.getCount());
            i += k;
            if (removeCount != 0)
            {
                itemstack.shrink(k);
                if (itemstack.isEmpty())
                {
                    setInventorySlotContents(l, ItemStack::EMPTY);
                }

                if (removeCount > 0 && i >= removeCount)
                {
                    return i;
                }
            }
        }
    }

    if (!itemStack.isEmpty())
    {
        if (itemIn != nullptr && itemStack.getItem() != itemIn)
        {
            return i;
        }

        if (metadataIn > -1 && itemStack.getMetadata() != metadataIn)
        {
            return i;
        }

        if (itemNBT != nullptr && !NBTUtil::areNBTEquals(itemNBT, itemStack.getTagCompound(), true))
        {
            return i;
        }

        auto l = removeCount <= 0 ? itemStack.getCount() : MathHelper::min(removeCount - i, itemStack.getCount());
        i += l;
        if (removeCount != 0)
        {
            itemStack.shrink(l);
            if (itemStack.isEmpty())
            {
                itemStack = ItemStack::EMPTY;
            }

            if (removeCount > 0 && i >= removeCount)
            {
                return i;
            }
        }
    }

    return i;
}

int32_t InventoryPlayer::storeItemStack(ItemStack itemStackIn)
{
    if (InventoryHelper::canMergeStacks(getStackInSlot(currentItem), itemStackIn, this))
    {
        return currentItem;
    }
    else if (InventoryHelper::canMergeStacks(getStackInSlot(40), itemStackIn, this))
    {
        return 40;
    }
    else
    {
        for (auto i = 0; i < mainInventory.size(); ++i)
        {
            if (InventoryHelper::canMergeStacks(mainInventory[i], itemStackIn, this))
            {
                return i;
            }
        }

        return -1;
    }
}

void InventoryPlayer::decrementAnimations()
{
    for (auto nonnulllist : allInventories)
    {
        for (auto i = 0; i < nonnulllist->size(); ++i)
        {
            if (!((*nonnulllist)[i]).isEmpty())
            {
                (*nonnulllist)[i].updateAnimation(player->world, player, i, currentItem == i);
            }
        }
    }
}

bool InventoryPlayer::addItemStackToInventory(ItemStack itemStackIn)
{
    return add(-1, itemStackIn);
}

bool InventoryPlayer::add(int32_t p_191971_1_, ItemStack p_191971_2_)
{
    if (p_191971_2_.isEmpty())
    {
        return false;
    }
    else
    {
        try
        {
            if (p_191971_2_.isItemDamaged())
            {
                if (p_191971_1_ == -1)
                {
                    p_191971_1_ = getFirstEmptyStack();
                }

                if (p_191971_1_ >= 0)
                {
                    mainInventory[p_191971_1_] = p_191971_2_.copy();
                    mainInventory[p_191971_1_].setAnimationsToGo(5);
                    p_191971_2_.setCount(0);
                    return true;
                }
                else if (player->capabilities.isCreativeMode)
                {
                    p_191971_2_.setCount(0);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                int i;
                do
                {
                    i = p_191971_2_.getCount();
                    if (p_191971_1_ == -1)
                    {
                        p_191971_2_.setCount(storePartialItemStack(p_191971_2_));
                    }
                    else
                    {
                        p_191971_2_.setCount(addResource(p_191971_1_, p_191971_2_));
                    }
                } while (!p_191971_2_.isEmpty() && p_191971_2_.getCount() < i);

                if (p_191971_2_.getCount() == i && player->capabilities.isCreativeMode)
                {
                    p_191971_2_.setCount(0);
                    return true;
                }
                else
                {
                    return p_191971_2_.getCount() < i;
                }
            }
        }
        catch (std::exception &var6)
        {
            CrashReport crashreport                 = CrashReport.makeCrashReport(var6, "Adding item to inventory");
            CrashReportCategory crashreportcategory = crashreport.makeCategory("Item being added");
            crashreportcategory.addCrashSection("Item ID", Item.getIdFromItem(p_191971_2_.getItem()));
            crashreportcategory.addCrashSection("Item data", p_191971_2_.getMetadata());
            crashreportcategory.addDetail("Item name", new ICrashReportDetail() {
               public std::string call() throws Exception {
                  return p_191971_2_.getDisplayName();
        }
    });
    throw ReportedException(crashreport);
}
}
}

void InventoryPlayer::placeItemBackInInventory(World *p_191975_1_, ItemStack p_191975_2_)
{
    if (!p_191975_1_->isRemote)
    {
        while (!p_191975_2_.isEmpty())
        {
            int i = storeItemStack(p_191975_2_);
            if (i == -1)
            {
                i = getFirstEmptyStack();
            }

            if (i == -1)
            {
                player->dropItem(p_191975_2_, false);
                break;
            }

            int j = p_191975_2_.getMaxStackSize() - getStackInSlot(i).getCount();
            if (add(i, p_191975_2_.splitStack(j)))
            {
                ((EntityPlayerMP *)player)->connection.sendPacket(new SPacketSetSlot(-2, i, getStackInSlot(i)));
            }
        }
    }
}

ItemStack InventoryPlayer::decrStackSize(int32_t index, int32_t count)
{
    std::vector<ItemStack> *list = nullptr;

    for (auto nonnulllist : allInventories)
    {
        if (index < nonnulllist->size())
        {
            list = nonnulllist;
            break;
        }
        index -= nonnulllist->size();
    }

    return list != nullptr && !((*list)[index]).isEmpty() ? ItemStackHelper::getAndSplit(*list, index, count)
                                                          : ItemStack::EMPTY;
}

void InventoryPlayer::deleteStack(ItemStack stack)
{

    while (true)
    {
        for (auto nonnulllist : allInventories)
        {

            for (int i = 0; i < nonnulllist->size(); ++i)
            {
                if ((*nonnulllist)[i] == stack)
                {
                    (*nonnulllist)[i] = ItemStack::EMPTY;
                    break;
                }
            }
        }

        return;
    }
}

ItemStack InventoryPlayer::removeStackFromSlot(int32_t index)
{
    std::vector<ItemStack> *nonnulllist = nullptr;

    for (auto nonnulllist1 : allInventories)
    {
        if (index < nonnulllist1->size())
        {
            nonnulllist = nonnulllist1;
            break;
        }
        index -= nonnulllist1->size();
    }

    if (nonnulllist != nullptr && !((*nonnulllist)[index].isEmpty()))
    {
        ItemStack itemstack   = (*nonnulllist)[index];
        (*nonnulllist)[index] = ItemStack::EMPTY;
        return itemstack;
    }
    else
    {
        return ItemStack::EMPTY;
    }
}

void InventoryPlayer::setInventorySlotContents(int32_t index, ItemStack stack)
{
    std::vector<ItemStack> *nonnulllist = nullptr;

    for (auto nonnulllist1 : allInventories)
    {
        if (index < nonnulllist1->size())
        {
            nonnulllist = nonnulllist1;
            break;
        }
        index -= nonnulllist1->size();
    }

    if (nonnulllist != nullptr)
    {
        (*nonnulllist)[index] = stack;
    }
}

float InventoryPlayer::getDestroySpeed(IBlockState *state)
{
    float f = 1.0F;
    if (!(mainInventory[currentItem]).isEmpty())
    {
        f *= (mainInventory[currentItem]).getDestroySpeed(state);
    }

    return f;
}

NBTTagList *InventoryPlayer::writeToNBT(NBTTagList *nbtTagListIn)
{
    NBTTagCompound *nbttagcompound2;
    for (auto k = 0; k < mainInventory.size(); ++k)
    {
        if (!(mainInventory[k]).isEmpty())
        {
            nbttagcompound2 = new NBTTagCompound();
            nbttagcompound2->setByte("Slot", k);
            (mainInventory[k]).writeToNBT(nbttagcompound2);
            nbtTagListIn->appendTag(nbttagcompound2);
        }
    }

    for (auto k = 0; k < armorInventory.size(); ++k)
    {
        if (!(armorInventory[k]).isEmpty())
        {
            nbttagcompound2 = new NBTTagCompound();
            nbttagcompound2->setByte("Slot", (k + 100));
            (armorInventory[k]).writeToNBT(nbttagcompound2);
            nbtTagListIn->appendTag(nbttagcompound2);
        }
    }

    for (auto k = 0; k < offHandInventory.size(); ++k)
    {
        if (!(offHandInventory[k]).isEmpty())
        {
            nbttagcompound2 = new NBTTagCompound();
            nbttagcompound2->setByte("Slot", (k + 150));
            (offHandInventory[k]).writeToNBT(nbttagcompound2);
            nbtTagListIn->appendTag(nbttagcompound2);
        }
    }

    return nbtTagListIn;
}

void InventoryPlayer::readFromNBT(NBTTagList *nbtTagListIn)
{
    mainInventory.clear();
    armorInventory.clear();
    offHandInventory.clear();

    for (auto i = 0; i < nbtTagListIn->tagCount(); ++i)
    {
        NBTTagCompound *nbttagcompound = nbtTagListIn->getCompoundTagAt(i);
        auto j                         = nbttagcompound->getByte("Slot") & 255;
        ItemStack itemstack            = ItemStack(nbttagcompound);
        if (!itemstack.isEmpty())
        {
            if (j >= 0 && j < mainInventory.size())
            {
                mainInventory[j] = itemstack;
            }
            else if (j >= 100 && j < armorInventory.size() + 100)
            {
                armorInventory[j - 100] = itemstack;
            }
            else if (j >= 150 && j < offHandInventory.size() + 150)
            {
                offHandInventory[j - 150] = itemstack;
            }
        }
    }
}

int32_t InventoryPlayer::getSizeInventory() const
{
    return mainInventory.size() + armorInventory.size() + offHandInventory.size();
}

bool InventoryPlayer::isEmpty() const
{

    bool empty = false;
    empty      = InventoryHelper::isInventoryEmpty(mainInventory);
    empty      = InventoryHelper::isInventoryEmpty(armorInventory);
    empty      = InventoryHelper::isInventoryEmpty(offHandInventory);

    return false;
}

ItemStack InventoryPlayer::getStackInSlot(int32_t index)
{
    std::vector<ItemStack> *list = nullptr;

    for (auto nonnulllist : allInventories)
    {
        if (index < nonnulllist->size())
        {
            list = nonnulllist;
            break;
        }
        index -= nonnulllist->size();
    }

    return list == nullptr ? ItemStack::EMPTY : list[index];
}

std::string InventoryPlayer::getName() const
{
    return "container.inventory";
}

bool InventoryPlayer::hasCustomName() const
{
    return false;
}

ITextComponent *InventoryPlayer::getDisplayName() const
{
    return (ITextComponent *)(hasCustomName() ? new TextComponentString(getName())
                                              : new TextComponentTranslation(getName(), {}));
}

int32_t InventoryPlayer::getInventoryStackLimit() const
{
    return 64;
}

bool InventoryPlayer::canHarvestBlock(IBlockState *state)
{
    if (state->getMaterial().isToolNotRequired())
    {
        return true;
    }
    else
    {
        ItemStack itemstack = getStackInSlot(currentItem);
        return !itemstack.isEmpty() ? itemstack.canHarvestBlock(state) : false;
    }
}

ItemStack InventoryPlayer::armorItemInSlot(int32_t slotIn)
{
    return armorInventory[slotIn];
}

void InventoryPlayer::damageArmor(float damage)
{
    damage /= 4.0F;
    if (damage < 1.0F)
    {
        damage = 1.0F;
    }

    for (auto itemstack : armorInventory)
    {
        if (Util:: instanceof <ItemArmor>(itemstack.getItem()))
        {
            itemstack.damageItem(damage, player);
        }
    }
}

void InventoryPlayer::dropAllItems()
{
    for (auto list : allInventories)
    {

        for (auto &itemstack : list)
        {
            if (!itemstack.isEmpty())
            {
                player->dropItem(itemstack, true, false);
                itemstack = ItemStack::EMPTY;
            }
        }
    }
}

void InventoryPlayer::markDirty()
{
    ++timesChanged;
}

int32_t InventoryPlayer::getTimesChanged() const
{
    return timesChanged;
}

void InventoryPlayer::setItemStack(ItemStack itemStackIn)
{
    itemStack = itemStackIn;
}

ItemStack InventoryPlayer::getItemStack() const
{
    return itemStack;
}

bool InventoryPlayer::isUsableByPlayer(EntityPlayer *player)
{
    if (player->isDead)
    {
        return false;
    }
    else
    {
        return player->getDistanceSq(player) <= 64.0;
    }
}

bool InventoryPlayer::hasItemStack(ItemStack itemStackIn)
{
    for (auto list : allInventories)
    {
        for (auto itemstack : list)
        {
            if (!itemstack.isEmpty() && itemstack.isItemEqual(itemStackIn))
            {
                return true;
            }
        }
    }

    return false;
}

void InventoryPlayer::openInventory(EntityPlayer *player)
{
}

void InventoryPlayer::closeInventory(EntityPlayer *player)
{
}

bool InventoryPlayer::isItemValidForSlot(int32_t index, ItemStack stack)
{
    return true;
}

void InventoryPlayer::copyInventory(InventoryPlayer *playerInventory)
{
    for (int i = 0; i < getSizeInventory(); ++i)
    {
        setInventorySlotContents(i, playerInventory->getStackInSlot(i));
    }

    currentItem = playerInventory->currentItem;
}

int32_t InventoryPlayer::getField(int32_t id)
{
    return 0;
}

void InventoryPlayer::setField(int32_t id, int32_t value)
{
}

int32_t InventoryPlayer::getFieldCount()
{
    return 0;
}

void InventoryPlayer::clear()
{
    for (auto list : allInventories)
    {
        list->clear();
    }
}

void InventoryPlayer::fillStackedContents(const RecipeItemHelper &helper, bool p_194016_2_)
{

    for (auto itemstack : mainInventory)
    {
        helper.accountStack(itemstack);
    }

    if (p_194016_2_)
    {
        helper.accountStack(offHandInventory[0]);
    }
}

int32_t InventoryPlayer::storePartialItemStack(ItemStack itemStackIn)
{
    auto i = storeItemStack(itemStackIn);
    if (i == -1)
    {
        i = getFirstEmptyStack();
    }

    return i == -1 ? itemStackIn.getCount() : addResource(i, itemStackIn);
}

int32_t InventoryPlayer::addResource(int32_t p_191973_1_, ItemStack p_191973_2_)
{
    Item *item          = p_191973_2_.getItem();
    int i               = p_191973_2_.getCount();
    ItemStack itemstack = getStackInSlot(p_191973_1_);
    if (itemstack.isEmpty())
    {
        itemstack = ItemStack(item, 0, p_191973_2_.getMetadata());
        if (p_191973_2_.hasTagCompound())
        {
            itemstack.setTagCompound(p_191973_2_.getTagCompound().copy());
        }

        setInventorySlotContents(p_191973_1_, itemstack);
    }

    auto j = i;
    if (i > itemstack.getMaxStackSize() - itemstack.getCount())
    {
        j = itemstack.getMaxStackSize() - itemstack.getCount();
    }

    if (j > getInventoryStackLimit() - itemstack.getCount())
    {
        j = getInventoryStackLimit() - itemstack.getCount();
    }

    if (j == 0)
    {
        return i;
    }
    else
    {
        i -= j;
        itemstack.grow(j);
        itemstack.setAnimationsToGo(5);
        return i;
    }
}
