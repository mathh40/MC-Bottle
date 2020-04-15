#pragma once
#include "Item.h"

class Packet;
class ItemMapBase :public Item
{
public:
    bool isMap() override;
    Packet* createMapDataPacket(ItemStack stack, World* worldIn, EntityPlayer* player);
};
