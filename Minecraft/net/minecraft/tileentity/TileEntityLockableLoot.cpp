#include "TileEntityLockableLoot.h"

#include "../world/WorldServer.h"

TileEntityLockableLoot::TileEntityLockableLoot() : lootTable("", "")
{
}

void TileEntityLockableLoot::fillWithLoot(EntityPlayer *player)
{
    if (lootTable != nullptr)
    {
        LootTable loottable = world->getLootTableManager().getLootTableFromLocation(lootTable);
        lootTable           = nullptr;
        pcg32 random;
        if (lootTableSeed != 0L)
        {
            random.seed(lootTableSeed);
        }

        LootContext.Builder lootcontext$builder = new LootContext.Builder((WorldServer *)world);
        if (player != nullptr)
        {
            lootcontext$builder.withLuck(player->getLuck());
        }

        loottable.fillInventory(this, random, lootcontext$builder.build());
    }
}

ResourceLocation TileEntityLockableLoot::getLootTable() const
{
    return lootTable;
}

void TileEntityLockableLoot::setLootTable(ResourceLocation p_189404_1_, int64_t p_189404_2_)
{
    lootTable     = p_189404_1_;
    lootTableSeed = p_189404_2_;
}

bool TileEntityLockableLoot::hasCustomName() const
{
    return !customName.empty();
}

void TileEntityLockableLoot::setCustomName(std::string_view p_190575_1_)
{
    customName = p_190575_1_;
}

ItemStack TileEntityLockableLoot::getStackInSlot(int32_t index)
{
    fillWithLoot(nullptr);
    return getItems().get(index);
}

ItemStack TileEntityLockableLoot::decrStackSize(int32_t index, int32_t count)
{
    fillWithLoot(nullptr);
    ItemStack itemstack = ItemStackHelper.getAndSplit(getItems(), index, count);
    if (!itemstack.isEmpty())
    {
        markDirty();
    }

    return itemstack;
}

ItemStack TileEntityLockableLoot::removeStackFromSlot(int32_t index)
{
    fillWithLoot(nullptr);
    return ItemStackHelper.getAndRemove(getItems(), index);
}

void TileEntityLockableLoot::setInventorySlotContents(int32_t index, ItemStack stack)
{
    fillWithLoot(nullptr);
    getItems().set(index, stack);
    if (stack.getCount() > getInventoryStackLimit())
    {
        stack.setCount(getInventoryStackLimit());
    }

    markDirty();
}

bool TileEntityLockableLoot::isUsableByPlayer(EntityPlayer *player)
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

void TileEntityLockableLoot::openInventory(EntityPlayer *player)
{
}

void TileEntityLockableLoot::closeInventory(EntityPlayer *player)
{
}

bool TileEntityLockableLoot::isItemValidForSlot(int32_t index, ItemStack stack)
{
    return true;
}

int32_t TileEntityLockableLoot::getField(int32_t id) const
{
    return 0;
}

void TileEntityLockableLoot::setField(int32_t id, int32_t value)
{
}

int32_t TileEntityLockableLoot::getFieldCount() const
{
    return 0;
}

void TileEntityLockableLoot::clear()
{
    fillWithLoot(nullptr);
    getItems().clear();
}

bool TileEntityLockableLoot::checkLootAndRead(NBTTagCompound *compound)
{
    if (compound->hasKey("LootTable", 8))
    {
        lootTable     = ResourceLocation(compound->getString("LootTable"));
        lootTableSeed = compound->getLong("LootTableSeed");
        return true;
    }
    else
    {
        return false;
    }
}

bool TileEntityLockableLoot::checkLootAndWrite(NBTTagCompound *compound)
{
    if (lootTable != nullptr)
    {
        compound->setString("LootTable", lootTable.to_string());
        if (lootTableSeed != 0L)
        {
            compound->setLong("LootTableSeed", lootTableSeed);
        }

        return true;
    }
    else
    {
        return false;
    }
}
