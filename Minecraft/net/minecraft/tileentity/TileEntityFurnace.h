#include "../inventory/ISidedInventory.h"
#include "ITickable.h"
#include "TileEntityLockable.h"

class InventoryPlayer;
class DataFixer;
class Container;

class TileEntityFurnace : public TileEntityLockable, ITickable, ISidedInventory
{
  public:
    TileEntityFurnace();
    int32_t getSizeInventory();
    bool isEmpty();
    ItemStack getStackInSlot(int32_t index);
    ItemStack decrStackSize(int32_t index, int32_t count);
    ItemStack removeStackFromSlot(int32_t index);
    void setInventorySlotContents(int32_t index, ItemStack stack);
    std::string getName() const override;
    bool hasCustomName() const override;
    void setCustomInventoryName(std::string_view p_145951_1_);
    static void registerFixesFurnace(DataFixer fixer);
    void readFromNBT(NBTTagCompound *compound) override;
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    int32_t getInventoryStackLimit() const;
    bool isBurning() const;
    bool isBurning(IInventory *inventory);
    void update() override;
    int32_t getCookTime(ItemStack stack) const;
    void smeltItem();
    int32_t getItemBurnTime(ItemStack stack);
    static bool isItemFuel(ItemStack stack);
    bool isUsableByPlayer(EntityPlayer *player);
    void openInventory(EntityPlayer *player);
    void closeInventory(EntityPlayer *player);
    bool isItemValidForSlot(int32_t index, ItemStack stack);
    std::vector<int32_t> getSlotsForFace(EnumFacing side);
    bool canInsertItem(int32_t index, ItemStack itemStackIn, EnumFacing direction);
    bool canExtractItem(int32_t index, ItemStack stack, EnumFacing direction);
    std::string getGuiID() const;
    Container *createContainer(InventoryPlayer *playerInventory, EntityPlayer *playerIn);
    int32_t getField(int32_t id) const;
    void setField(int32_t id, int32_t value);
    int32_t getFieldCount() const;
    void clear();

  private:
    static const int32_t SLOTS_TOP[1];
    static const int32_t SLOTS_BOTTOM[2];
    static const int32_t SLOTS_SIDES[1];
    std::vector<ItemStack> furnaceItemStacks;
    int16_t furnaceBurnTime;
    int32_t currentItemBurnTime;
    int16_t cookTime;
    int16_t totalCookTime;
    std::string furnaceCustomName;

    bool canSmelt();
};
