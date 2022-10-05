#include "TileEntityBrewingStand.h"

#include "datafix/FixTypes.h"

std::array<int32_t, 1> TileEntityBrewingStand::SLOTS_FOR_UP   = {3};
std::array<int32_t, 4> TileEntityBrewingStand::SLOTS_FOR_DOWN = {0, 1, 2, 3};
std::array<int32_t, 4> TileEntityBrewingStand::OUTPUT_SLOTS   = {0, 1, 2, 4};

TileEntityBrewingStand::TileEntityBrewingStand() : brewingItemStacks(5, ItemStack::EMPTY)
{
}

std::string TileEntityBrewingStand::getName() const
{
    return hasCustomName() ? customName : "container.brewing";
}

bool TileEntityBrewingStand::hasCustomName() const
{
    return !customName.empty();
}

void TileEntityBrewingStand::setName(std::string_view name)
{
    customName = name;
}

int32_t TileEntityBrewingStand::getSizeInventory() const
{
    return brewingItemStacks.size();
}

bool TileEntityBrewingStand::isEmpty() const
{
    bool empty = false;

    for (auto stack : brewingItemStacks)
    {
        empty = stack.isEmpty();
    }
    return empty;
}

void TileEntityBrewingStand::update()
{
    ItemStack itemstack = brewingItemStacks[4];
    if (fuel <= 0 && itemstack.getItem() == Items::BLAZE_POWDER)
    {
        fuel = 20;
        itemstack.shrink(1);
        markDirty();
    }

    bool flag            = canBrew();
    bool flag1           = brewTime > 0;
    ItemStack itemstack1 = brewingItemStacks[3];
    if (flag1)
    {
        --brewTime;
        bool flag2 = brewTime == 0;
        if (flag2 && flag)
        {
            brewPotions();
            markDirty();
        }
        else if (!flag)
        {
            brewTime = 0;
            markDirty();
        }
        else if (ingredientID != itemstack1.getItem())
        {
            brewTime = 0;
            markDirty();
        }
    }
    else if (flag && fuel > 0)
    {
        --fuel;
        brewTime     = 400;
        ingredientID = itemstack1.getItem();
        markDirty();
    }

    if (!world->isRemote)
    {
        auto aboolean = createFilledSlotsArray();
        if (!aboolean != filledSlots)
        {
            filledSlots              = aboolean;
            IBlockState *iblockstate = world->getBlockState(getPos());
            if (!(iblockstate.getBlock() instanceof BlockBrewingStand))
            {
                return;
            }

            for (int i = 0; i < BlockBrewingStand.HAS_BOTTLE.length; ++i)
            {
                iblockstate = iblockstate.withProperty(BlockBrewingStand.HAS_BOTTLE[i], aboolean[i]);
            }

            world->setBlockState(pos, iblockstate, 2);
        }
    }
}

std::array<bool, 3> TileEntityBrewingStand::createFilledSlotsArray()
{
    std::array<bool, 3> aboolean;

    for (auto i : aboolean)
    {
        if (!i.isEmpty())
        {
            i = true;
        }
    }

    return aboolean;
}

void TileEntityBrewingStand::registerFixesBrewingStand(DataFixer fixer)
{
    fixer.registerWalker(FixTypes::BLOCK_ENTITY,
                         ItemStackDataLists(TileEntityBrewingStand.class, new String[]{"Items"}));
}

void TileEntityBrewingStand::readFromNBT(NBTTagCompound *compound)
{
    TileEntityLockable::readFromNBT(compound);
    brewingItemStacks = std::vector<ItemStack>(5, ItemStack::EMPTY);
    ItemStackHelper::loadAllItems(compound, brewingItemStacks);
    brewTime = compound->getShort("BrewTime");
    if (compound->hasKey("CustomName", 8))
    {
        customName = compound->getString("CustomName");
    }

    fuel = compound->getByte("Fuel");
}

NBTTagCompound *TileEntityBrewingStand::writeToNBT(NBTTagCompound *compound)
{
    TileEntityLockable::writeToNBT(compound);
    compound->setShort("BrewTime", (short)brewTime);
    ItemStackHelper.saveAllItems(compound, brewingItemStacks);
    if (hasCustomName())
    {
        compound->setString("CustomName", customName);
    }

    compound->setByte("Fuel", (fuel));
    return compound;
}

ItemStack TileEntityBrewingStand::getStackInSlot(int32_t index)
{
    return index >= 0 && index < brewingItemStacks.size() ? brewingItemStacks[index] : ItemStack::EMPTY;
}

ItemStack TileEntityBrewingStand::decrStackSize(int32_t index, int32_t count)
{
    return ItemStackHelper.getAndSplit(brewingItemStacks, index, count);
}

ItemStack TileEntityBrewingStand::removeStackFromSlot(int32_t index)
{
    return ItemStackHelper.getAndRemove(brewingItemStacks, index);
}

void TileEntityBrewingStand::setInventorySlotContents(int32_t index, ItemStack stack)
{
    if (index >= 0 && index < brewingItemStacks.size())
    {
        brewingItemStacks[index] = stack;
    }
}

int32_t TileEntityBrewingStand::getInventoryStackLimit() const
{
    return 64;
}

bool TileEntityBrewingStand::isUsableByPlayer(EntityPlayer *player)
{
    if (world->getTileEntity(pos) != this)
    {
        return false;
    }
    else
    {
        return player->getDistanceSq((double)pos.getx() + 0.5, (double)pos.gety() + 0.5, (double)pos.getz() + 0.5) <=
               64.0;
    }
}

void TileEntityBrewingStand::openInventory(EntityPlayer *player)
{
}

void TileEntityBrewingStand::closeInventory(EntityPlayer *player)
{
}

bool TileEntityBrewingStand::isItemValidForSlot(int32_t index, ItemStack stack)
{
    if (index == 3)
    {
        return PotionHelper.isReagent(stack);
    }
    else
    {
        Item item = stack.getItem();
        if (index == 4)
        {
            return item == Items::BLAZE_POWDER;
        }
        else
        {
            return (item == Items::POTIONITEM || item == Items::SPLASH_POTION || item == Items::LINGERING_POTION ||
                    item == Items::GLASS_BOTTLE) &&
                   getStackInSlot(index).isEmpty();
        }
    }
}

std::vector<int32_t> TileEntityBrewingStand::getSlotsForFace(EnumFacing side)
{
    if (side == EnumFacing::UP)
    {
        return std::vector<int32_t>(SLOTS_FOR_UP.begin(), SLOTS_FOR_UP.end());
    }
    else
    {
        auto temp = side == EnumFacing::DOWN ? SLOTS_FOR_DOWN : OUTPUT_SLOTS;
        return std::vector<int32_t>(temp.begin(), temp.end());
    }
}

bool TileEntityBrewingStand::canInsertItem(int32_t index, ItemStack itemStackIn, EnumFacing direction)
{
    return isItemValidForSlot(index, itemStackIn);
}

bool TileEntityBrewingStand::canExtractItem(int32_t index, ItemStack stack, EnumFacing direction)
{
    if (index == 3)
    {
        return stack.getItem() == Items::GLASS_BOTTLE;
    }
    else
    {
        return true;
    }
}

std::string TileEntityBrewingStand::getGuiID() const
{
    return "minecraft:brewing_stand";
}

Container TileEntityBrewingStand::createContainer(InventoryPlayer *playerInventory, EntityPlayer *playerIn)
{
    return ContainerBrewingStand(playerInventory, this);
}

int32_t TileEntityBrewingStand::getField(int32_t id) const
{
    switch (id)
    {
    case 0:
        return brewTime;
    case 1:
        return fuel;
    default:
        return 0;
    }
}

void TileEntityBrewingStand::setField(int32_t id, int32_t value)
{
    switch (id)
    {
    case 0:
        brewTime = value;
        break;
    case 1:
        fuel = value;
    }
}

int32_t TileEntityBrewingStand::getFieldCount() const
{
    return 2;
}

void TileEntityBrewingStand::clear()
{
    brewingItemStacks.clear();
}

bool TileEntityBrewingStand::canBrew()
{
    ItemStack itemstack = brewingItemStacks[3];
    if (itemstack.isEmpty())
    {
        return false;
    }
    else if (!PotionHelper.isReagent(itemstack))
    {
        return false;
    }
    else
    {
        for (int i = 0; i < 3; ++i)
        {
            ItemStack itemstack1 = brewingItemStacks[i];
            if (!itemstack1.isEmpty() && PotionHelper.hasConversions(itemstack1, itemstack))
            {
                return true;
            }
        }

        return false;
    }
}

void TileEntityBrewingStand::brewPotions()
{
    ItemStack itemstack = brewingItemStacks[3];

    for (int i = 0; i < 3; ++i)
    {
        brewingItemStacks[i] = PotionHelper.doReaction(itemstack, brewingItemStacks[i]);
    }

    itemstack.shrink(1);
    BlockPos blockpos = getPos();
    if (itemstack.getItem().hasContainerItem())
    {
        ItemStack itemstack1 = ItemStack(itemstack.getItem().getContainerItem());
        if (itemstack.isEmpty())
        {
            itemstack = itemstack1;
        }
        else
        {
            InventoryHelper::spawnItemStack(world, (double)blockpos.getx(), (double)blockpos.gety(),
                                            (double)blockpos.getz(), itemstack1);
        }
    }

    brewingItemStacks[3] = itemstack;
    world->playEvent(1035, blockpos, 0);
}
