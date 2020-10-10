#include "EntityMinecartContainer.h"


#include "DamageSource.h"
#include "WorldServer.h"
#include "../../inventory/InventoryHelper.h"
#include "../../inventory/ItemStackHelper.h"
#include "datafix/DataFixer.h"

EntityMinecartContainer::EntityMinecartContainer(World *worldIn)
    :EntityMinecart(worldIn),minecartContainerItems(36, ItemStack::EMPTY),dropContentsWhenDead(true){
}

EntityMinecartContainer::EntityMinecartContainer(World *worldIn, double x, double y, double z)
    :EntityMinecart(worldIn, x, y, z),minecartContainerItems(36, ItemStack::EMPTY),dropContentsWhenDead(true){
}

void EntityMinecartContainer::killMinecart(DamageSource::DamageSource source) {
    EntityMinecart::killMinecart(source);
    if (world->getGameRules().getBoolean("doEntityDrops")) {
        InventoryHelper::dropInventoryItems(world, this, (IInventory*)this);
    }
}

bool EntityMinecartContainer::isEmpty() const{
    const auto itemstack = minecartContainerItems.begin();

      do {
         if (itemstack == minecartContainerItems.end()) {
            return true;
         }
      } while(itemstack->isEmpty());

      return false;
}

ItemStack EntityMinecartContainer::getStackInSlot(int32_t index) {
    addLoot(nullptr);
    return minecartContainerItems[index];
}

ItemStack EntityMinecartContainer::decrStackSize(int32_t index, int32_t count) {
    addLoot(nullptr);
    return ItemStackHelper::getAndSplit(minecartContainerItems, index, count);
}

ItemStack EntityMinecartContainer::removeStackFromSlot(int32_t index) {
    addLoot(nullptr);
    ItemStack itemstack = minecartContainerItems[index];
    if (itemstack.isEmpty()) {
        return ItemStack::EMPTY;
    } else {
        minecartContainerItems[index] = ItemStack::EMPTY;
        return itemstack;
    }
}

void EntityMinecartContainer::setInventorySlotContents(int32_t index, ItemStack stack) {
    addLoot(nullptr);
    minecartContainerItems[index] =  stack;
    if (!stack.isEmpty() && stack.getCount() > getInventoryStackLimit()) {
        stack.setCount(getInventoryStackLimit());
    }
}

void EntityMinecartContainer::markDirty() {
}

bool EntityMinecartContainer::isUsableByPlayer(EntityPlayer *player) {
    if (isDead) {
        return false;
    } else {
        return player->getDistanceSq(this) <= 64.0;
    }
}

void EntityMinecartContainer::openInventory(EntityPlayer *player) {

}

void EntityMinecartContainer::closeInventory(EntityPlayer *player) {
}

bool EntityMinecartContainer::isItemValidForSlot(int32_t index, ItemStack stack) {
    return true;
}

int32_t EntityMinecartContainer::getInventoryStackLimit() const {
    return 64;
}

Entity * EntityMinecartContainer::changeDimension(int32_t dimensionIn) {
    dropContentsWhenDead = false;
    return EntityMinecart::changeDimension(dimensionIn);
}

void EntityMinecartContainer::setDead() {
    if (dropContentsWhenDead) {
         InventoryHelper::dropInventoryItems(world, this, this);
      }

      EntityMinecart::setDead();
}

void EntityMinecartContainer::setDropItemsWhenDead(bool dropWhenDead) {
    dropContentsWhenDead = dropWhenDead;
}

void EntityMinecartContainer::addDataFixers(DataFixer p_190574_0_, Class p_190574_1_) {
    EntityMinecart::registerFixesMinecart(p_190574_0_, p_190574_1_);
    p_190574_0_.registerWalker(FixTypes::ENTITY, new ItemStackDataLists(p_190574_1_, {"Items"}));
}

bool EntityMinecartContainer::processInitialInteract(EntityPlayer *player, EnumHand hand) {
    if (!world->isRemote) {
         player->displayGUIChest(this);
      }

      return true;
}

int32_t EntityMinecartContainer::getField(int32_t id) {
    return 0;
}

void EntityMinecartContainer::setField(int32_t id, int32_t value) {
}

int32_t EntityMinecartContainer::getFieldCount() {
    return 0;
}

bool EntityMinecartContainer::isLocked() {
    return false;
}

void EntityMinecartContainer::setLockCode(LockCode &code) {

}

LockCode EntityMinecartContainer::getLockCode() {
    return LockCode::EMPTY_CODE;
}

void EntityMinecartContainer::addLoot(EntityPlayer *player) {
    if (lootTable.has_value()) {
         pcg32 loottable = world->getLootTableManager().getLootTableFromLocation(lootTable);
         lootTable = std::nullopt;
         pcg32 random;
         if (lootTableSeed == 0L) {
            random = pcg32();
         } else {
            random = pcg32(lootTableSeed);
         }

         LootContext::Builder lootcontext$builder = LootContext::Builder((WorldServer*)world);
         if (player != nullptr) {
            lootcontext$builder.withLuck(player->getLuck());
         }

         loottable.fillInventory(this, random, lootcontext$builder.build());
      }
}

void EntityMinecartContainer::clear() {
    addLoot(nullptr);
    minecartContainerItems.clear();
}

void EntityMinecartContainer::setLootTable(ResourceLocation lootTableIn, int64_t lootTableSeedIn) {
    lootTable = lootTableIn;
    lootTableSeed = lootTableSeedIn;
}

std::optional<ResourceLocation> EntityMinecartContainer::getLootTable() {
    return lootTable;
}

void EntityMinecartContainer::writeEntityToNBT(NBTTagCompound *compound) {
    EntityMinecart::writeEntityToNBT(compound);
      if (lootTable.has_value()) {
         compound->setString("LootTable", lootTable->to_string());
         if (lootTableSeed != 0L) {
            compound->setLong("LootTableSeed", lootTableSeed);
         }
      } else {
         ItemStackHelper::saveAllItems(compound, minecartContainerItems);
      }
}

void EntityMinecartContainer::readEntityFromNBT(NBTTagCompound *compound) {
    EntityMinecart::readEntityFromNBT(compound);
      minecartContainerItems = decltype(minecartContainerItems)(getSizeInventory(), ItemStack::EMPTY);
      if (compound->hasKey("LootTable", 8)) {
         lootTable = ResourceLocation(compound->getString("LootTable"));
         lootTableSeed = compound->getLong("LootTableSeed");
      } else {
         ItemStackHelper::loadAllItems(compound, minecartContainerItems);
      }
}

void EntityMinecartContainer::applyDrag() {
    float f = 0.98F;
    if (!lootTable.has_value()) {
        auto i = 15 - Container::calcRedstoneFromInventory(this);
        f += (float)i * 0.001F;
    }

    motionX *= (double)f;
    motionY *= 0.0;
    motionZ *= (double)f;
}
