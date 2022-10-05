#include "TileEntityLockable.h"

class TileEntityLockableLoot : public TileEntityLockable, ILootContainer
{
  public:
    TileEntityLockableLoot();

    void fillWithLoot(EntityPlayer *player);
    ResourceLocation getLootTable() const;
    void setLootTable(ResourceLocation p_189404_1_, int64_t p_189404_2_);
    bool hasCustomName() const override;
    void setCustomName(std::string_view p_190575_1_);
    ItemStack getStackInSlot(int32_t index);
    virtual ItemStack decrStackSize(int32_t index, int32_t count);
    ItemStack removeStackFromSlot(int32_t index);
    virtual void setInventorySlotContents(int32_t index, ItemStack stack);
    bool isUsableByPlayer(EntityPlayer *player);
    virtual void openInventory(EntityPlayer *player);
    virtual void closeInventory(EntityPlayer *player);
    bool isItemValidForSlot(int32_t index, ItemStack stack);
    int32_t getField(int32_t id) const;
    void setField(int32_t id, int32_t value);
    int32_t getFieldCount() const;
    virtual void clear();

  protected:
    virtual std::vector<ItemStack> &getItems() = 0;
    bool checkLootAndRead(NBTTagCompound *compound);
    bool checkLootAndWrite(NBTTagCompound *compound);

    ResourceLocation lootTable;
    int64_t lootTableSeed;
    std::string customName;

  private:
};
