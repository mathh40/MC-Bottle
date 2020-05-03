#pragma once
#include "Item.h"

class ItemWritableBook :public Item
{
public:
    ItemWritableBook();
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
    static bool isNBTValid(NBTTagCompound* nbt);

};