#pragma once
#include "ItemMapBase.h"
#include "../world/storage/MapData.h"

class ItemMap :public ItemMapBase
{
public:
    static ItemStack setupNewMap(World* worldIn, double worldX, double worldZ, uint8_t scale, bool trackingPosition, bool unlimitedTracking);
    static MapData loadMapData(int32_t mapId, World* worldIn);
    MapData getMapData(ItemStack stack, World* worldIn);
    void updateMapData(World* worldIn, Entity* viewer, MapData data);
    static void renderBiomePreviewMap(World* worldIn, ItemStack map);
    void onUpdate(ItemStack stack, World* worldIn, Entity* entityIn, int32_t itemSlot, bool isSelected) override;
    Packet createMapDataPacket(ItemStack stack, World* worldIn, EntityPlayer* player);
    void onCreated(ItemStack stack, World* worldIn, EntityPlayer* playerIn);
    static int32_t getColor(ItemStack p_190907_0_);
protected:
    ItemMap();
    static void scaleMap(ItemStack p_185063_0_, World* p_185063_1_, int32_t p_185063_2_);
    static void enableMapTracking(ItemStack p_185064_0_, World* p_185064_1_);
    void addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn) override;
private:
};
