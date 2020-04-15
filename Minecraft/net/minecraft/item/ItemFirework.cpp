#include "ItemFirework.h"

#include "EnumFacing.h"
#include "ItemStack.h"
#include "text/translation/I18n.h"

EnumActionResult ItemFirework::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                         EnumFacing facing, float hitX, float hitY, float hitZ)
{
    if (!worldIn->isRemote) 
    {
        ItemStack itemstack = player->getHeldItem(hand);
        EntityFireworkRocket* entityfireworkrocket = new EntityFireworkRocket(worldIn, (double)((float)pos.getx() + hitX), (double)((float)pos.gety() + hitY), (double)((float)pos.getz() + hitZ), itemstack);
        worldIn->spawnEntity(entityfireworkrocket);
        if (!player->capabilities.isCreativeMode) 
        {
            itemstack.shrink(1);
        }
    }

    return EnumActionResult::SUCCESS;
}

ActionResult ItemFirework::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    if (playerIn->isElytraFlying()) 
    {
        ItemStack itemstack = playerIn->getHeldItem(handIn);
        if (!worldIn->isRemote) 
        {
            EntityFireworkRocket* entityfireworkrocket = new EntityFireworkRocket(worldIn, itemstack, playerIn);
            worldIn->spawnEntity(entityfireworkrocket);
            if (!playerIn->capabilities.isCreativeMode) 
            {
                itemstack.shrink(1);
            }
        }

        return ActionResult(EnumActionResult::SUCCESS, playerIn->getHeldItem(handIn));
    }
    else 
    {
        return ActionResult(EnumActionResult::PASS, playerIn->getHeldItem(handIn));
    }
}

void ItemFirework::addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn)
{
    NBTTagCompound* nbttagcompound = stack.getSubCompound("Fireworks");
    if (nbttagcompound != nullptr) 
    {
        if (nbttagcompound->hasKey("Flight", 99)) 
        {
            tooltip.push_back(I18n::translateToLocal("item.fireworks.flight") + " " + nbttagcompound->getByte("Flight"));
        }

        NBTTagList* nbttaglist = nbttagcompound->getTagList("Explosions", 10);
        if (!nbttaglist->isEmpty()) 
        {
            for(auto i = 0; i < nbttaglist->tagCount(); ++i) 
            {
                NBTTagCompound* nbttagcompound1 = nbttaglist->getCompoundTagAt(i);
                std::vector<std::string> list;
                ItemFireworkCharge::addExplosionInfo(nbttagcompound1, list);
                if (!list.empty()) 
                {
                    for(int32_t j = 1; j < list.size(); ++j) 
                    {
                        list[j] = "  " + list[j];
                    }

                    tooltip.assign(list.begin(),list.end());
                }
            }
        }
    }
}
