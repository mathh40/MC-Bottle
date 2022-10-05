#include "TileEntity.h"

class TileEntityBed : public TileEntity
{
  public:
    TileEntityBed();
    void setItemValues(ItemStack p_193051_1_);
    void readFromNBT(NBTTagCompound *compound) override;
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    NBTTagCompound *getUpdateTag() override;
    std::optional<SPacketUpdateTileEntity> getUpdatePacket() override;
    EnumDyeColor getColor() const;
    void setColor(EnumDyeColor color);
    bool isHeadPiece();
    ItemStack getItemStack();

  private:
    EnumDyeColor color;
};
