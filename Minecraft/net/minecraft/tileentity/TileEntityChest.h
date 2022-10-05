#include "ITickable.h"
#include "TileEntityLockableLoot.h"

class DataFixer;

class TileEntityChest : public TileEntityLockableLoot, ITickable
{
  public:
    TileEntityChest();
    TileEntityChest(BlockChest::Type typeIn);

    int32_t getSizeInventory() const;
    bool isEmpty();
    std::string getName() const override;
    static void registerFixesChest(DataFixer fixer);
    void readFromNBT(NBTTagCompound *compound) override;
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    int32_t getInventoryStackLimit() const;
    void updateContainingBlockInfo() override;
    void checkForAdjacentChests();
    void update() override;
    bool receiveClientEvent(int32_t id, int32_t type) override;
    void openInventory(EntityPlayer *player) override;
    void closeInventory(EntityPlayer *player) override;
    void invalidate() override;
    BlockChest::Type getChestType();
    std::string getGuiID() const;
    Container createContainer(InventoryPlayer *playerInventory, EntityPlayer *playerIn);

    bool adjacentChestChecked;
    TileEntityChest *adjacentChestZNeg;
    TileEntityChest *adjacentChestXPos;
    TileEntityChest *adjacentChestXNeg;
    TileEntityChest *adjacentChestZPos;
    float lidAngle;
    float prevLidAngle;
    int32_t numPlayersUsing;

  protected:
    TileEntityChest *getAdjacentChest(EnumFacing side);
    std::vector<ItemStack> getItems() override;

  private:
    void setNeighbor(TileEntityChest *chestTe, EnumFacing side);
    bool isChestAt(BlockPos posIn);

    std::vector<ItemStack> chestContents;
    int32_t ticksSinceSync;
    BlockChest::Type cachedChestType;
};
