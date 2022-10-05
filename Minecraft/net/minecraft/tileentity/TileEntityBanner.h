#pragma once
#include "BannerPattern.h"
#include "TileEntity.h"

class TileEntityBanner : public TileEntity, public IWorldNameable
{
  public:
    TileEntityBanner();
    void setItemValues(ItemStack stack, bool p_175112_2_);
    std::string getName() const;
    bool hasCustomName() const;
    ITextComponent *getDisplayName() override;
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    void readFromNBT(NBTTagCompound *compound) override;
    SPacketUpdateTileEntity getUpdatePacket() override;
    NBTTagCompound *getUpdateTag() override;

    static int32_t getPatterns(ItemStack stack);
    static void removeBannerData(ItemStack stack);
    std::vector<BannerPattern> getPatternList();
    std::vector<EnumDyeColor> getColorList();
    std::string getPatternResourceLocation();
    ItemStack getItem();
    static EnumDyeColor getColor(ItemStack p_190616_0_);

  private:
    void initializeBannerData();

    std::string name;
    EnumDyeColor baseColor;
    NBTTagList *patterns;
    bool patternDataSet;
    std::vector<BannerPattern> patternList;
    std::vector<EnumDyeColor> colorList;
    std::string patternResourceLocation;
};
