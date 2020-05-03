#include "ItemWritableBook.h"


#include "ItemStack.h"
#include "../nbt/NBTTagList.h"
#include "../stats/StatList.h"

ItemWritableBook::ItemWritableBook()
{
    setMaxStackSize(1);
}

ActionResult ItemWritableBook::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    playerIn->openBook(itemstack, handIn);
    playerIn->addStat(StatList::getObjectUseStats(this));
    return ActionResult(EnumActionResult::SUCCESS, itemstack);
}

bool ItemWritableBook::isNBTValid(NBTTagCompound *nbt)
{
    if (nbt == nullptr) 
    {
        return false;
    }
    else if (!nbt->hasKey("pages", 9)) 
    {
        return false;
    }
    else 
    {
        NBTTagList* nbttaglist = nbt->getTagList("pages", 8);

        for(auto i = 0; i < nbttaglist->tagCount(); ++i) 
        {
            auto s = nbttaglist->getStringTagAt(i);
            if (s.size() > 32767) 
            {
                return false;
            }
        }

        return true;
    }
}
