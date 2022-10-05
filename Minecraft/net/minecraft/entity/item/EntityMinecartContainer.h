#pragma once
#include "EntityMinecart.h"
#include "storage/loot/ILootContainer.h"

class EntityMinecartContainer : public EntityMinecart, public ILockableContainer, public ILootContainer
{
  public:
    explicit EntityMinecartContainer(World *worldIn);
    EntityMinecartContainer(World *worldIn, double x, double y, double z);
    void killMinecart(DamageSource::DamageSource source) override;
    bool isEmpty() const override;
    ItemStack getStackInSlot(int32_t index) override;
    ItemStack decrStackSize(int32_t index, int32_t count) override;
    ItemStack removeStackFromSlot(int32_t index) override;
    void setInventorySlotContents(int32_t index, ItemStack stack) override;
    void markDirty() override;
    bool isUsableByPlayer(EntityPlayer *player) override;
    void openInventory(EntityPlayer *player) override;
    void closeInventory(EntityPlayer *player) override;
    bool isItemValidForSlot(int32_t index, ItemStack stack) override;
    int32_t getInventoryStackLimit() const override;
    Entity *changeDimension(int32_t dimensionIn) override;
    void setDead() override;
    void setDropItemsWhenDead(bool dropWhenDead) override;
    static void addDataFixers(DataFixer p_190574_0_, Class p_190574_1_);
    bool processInitialInteract(EntityPlayer *player, EnumHand hand) override;
    int32_t getField(int32_t id) override;
    void setField(int32_t id, int32_t value) override;
    int32_t getFieldCount() override;
    bool isLocked() override;
    void setLockCode(LockCode &code) override;
    LockCode getLockCode() override;
    void addLoot(EntityPlayer *player);
    void clear() override;
    void setLootTable(ResourceLocation lootTableIn, int64_t lootTableSeedIn);
    std::optional<ResourceLocation> getLootTable() override;

  protected:
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void applyDrag() override;

  private:
    std::vector<ItemStack> minecartContainerItems;
    bool dropContentsWhenDead;
    std::optional<ResourceLocation> lootTable;
    int64_t lootTableSeed;
};
