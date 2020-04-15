#include "ItemFireball.h"

#include "EnumFacing.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "../block/material/Material.h"
#include "../world/gen/ChunkGeneratorFlat.h"

ItemFireball::ItemFireball()
{
    setCreativeTab(CreativeTabs::MISC);
}

EnumActionResult ItemFireball::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    if (worldIn->isRemote) 
    {
        return EnumActionResult::SUCCESS;
    }
    else 
    {
        pos = pos.offset(facing);
        ItemStack itemstack = player->getHeldItem(hand);
        if (!player->canPlayerEdit(pos, facing, itemstack)) 
        {
            return EnumActionResult::FAIL;
        }
        else 
        {
            if (worldIn->getBlockState(pos).getMaterial() == Material::AIR) 
            {
                worldIn->playSound(nullptr, pos, SoundEvents::ITEM_FIRECHARGE_USE, SoundCategory::BLOCKS, 1.0F, (MathHelper::nextFloat(itemRand) - MathHelper::nextFloat(itemRand)) * 0.2F + 1.0F);
                worldIn->setBlockState(pos, Blocks::FIRE::getDefaultState());
            }

            if (!player->capabilities.isCreativeMode) 
            {
                itemstack.shrink(1);
            }

            return EnumActionResult::SUCCESS;
        }
    }
}
