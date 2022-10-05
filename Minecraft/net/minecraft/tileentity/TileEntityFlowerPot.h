#include "TileEntity.h"

class DataFixer;

class TileEntityFlowerPot : public TileEntity
{
  public:
    TileEntityFlowerPot() = default;
    TileEntityFlowerPot(const Item &potItem, int32_t potData);

    static void registerFixesFlowerPot(DataFixer fixer);

    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    void readFromNBT(NBTTagCompound *compound) override;
    std::optional<SPacketUpdateTileEntity> getUpdatePacket() override;
    NBTTagCompound *getUpdateTag() override;
    void setItemStack(ItemStack stack);
    ItemStack getFlowerItemStack();
    Item getFlowerPotItem() const;
    int32_t getFlowerPotData() const;

  private:
    Item flowerPotItem;
    int32_t flowerPotData;
};
