#pragma once
#include "../../inventory/IInventory.h"

class RecipeItemHelper;
class InventoryPlayer : public IInventory
{
  public:
    InventoryPlayer(EntityPlayer *playerIn);
    ItemStack getCurrentItem();
    int32_t getHotbarSize();
    int32_t getFirstEmptyStack();
    void setPickedItemStack(ItemStack stack);
    void pickItem(int32_t index);
    static constexpr bool isHotbar(int32_t index);
    int32_t getSlotFor(ItemStack stack);
    int32_t findSlotMatchingUnusedItem(ItemStack p_194014_1_);
    int32_t getBestHotbarSlot();
    void changeCurrentItem(int32_t direction);
    int32_t clearMatchingItems(Item *itemIn, int32_t metadataIn, int32_t removeCount, NBTTagCompound *itemNBT);
    int32_t storeItemStack(ItemStack itemStackIn);
    void decrementAnimations();
    bool addItemStackToInventory(ItemStack itemStackIn);
    bool add(int32_t p_191971_1_, ItemStack p_191971_2_);
    void placeItemBackInInventory(World *p_191975_1_, ItemStack p_191975_2_);
    ItemStack decrStackSize(int32_t index, int32_t count) override;
    void deleteStack(ItemStack stack);
    ItemStack removeStackFromSlot(int32_t index) override;
    void setInventorySlotContents(int32_t index, ItemStack stack) override;
    float getDestroySpeed(IBlockState *state);
    NBTTagList *writeToNBT(NBTTagList *nbtTagListIn);
    void readFromNBT(NBTTagList *nbtTagListIn);
    int32_t getSizeInventory() const override;
    bool isEmpty() const;
    ItemStack getStackInSlot(int32_t index) override;
    std::string getName() const override;
    bool hasCustomName() const override;
    ITextComponent *getDisplayName() const override;
    int32_t getInventoryStackLimit() const override;
    bool canHarvestBlock(IBlockState *state);
    ItemStack armorItemInSlot(int32_t slotIn);
    void damageArmor(float damage);
    void dropAllItems();
    void markDirty() override;
    int32_t getTimesChanged() const;
    void setItemStack(ItemStack itemStackIn);
    ItemStack getItemStack() const;
    bool isUsableByPlayer(EntityPlayer *player) override;
    bool hasItemStack(ItemStack itemStackIn);
    void openInventory(EntityPlayer *player) override;
    void closeInventory(EntityPlayer *player) override;
    bool isItemValidForSlot(int32_t index, ItemStack stack) override;
    void copyInventory(InventoryPlayer *playerInventory);
    int32_t getField(int32_t id) override;
    void setField(int32_t id, int32_t value) override;
    int32_t getFieldCount() override;
    void clear() override;
    void fillStackedContents(const RecipeItemHelper &helper, bool p_194016_2_);

    std::vector<ItemStack> mainInventory;
    std::vector<ItemStack> armorInventory;
    std::vector<ItemStack> offHandInventory;
    int32_t currentItem = 0;
    EntityPlayer *player;

  private:
    int32_t storePartialItemStack(ItemStack itemStackIn);
    int32_t addResource(int32_t p_191973_1_, ItemStack p_191973_2_);

    std::array<std::vector<ItemStack> *, 3> allInventories;
    ItemStack itemStack;
    int32_t timesChanged;
};
