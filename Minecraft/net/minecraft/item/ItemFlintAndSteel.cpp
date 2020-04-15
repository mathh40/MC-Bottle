#include "ItemFlintAndSteel.h"

#include "EnumFacing.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "Util.h"
#include "../block/material/Material.h"
#include "../world/gen/ChunkGeneratorFlat.h"

ItemFlintAndSteel::ItemFlintAndSteel()
{
    maxStackSize = 1;
    setMaxDamage(64);
    setCreativeTab(CreativeTabs::TOOLS);
}

EnumActionResult ItemFlintAndSteel::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
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
            worldIn->playSound(player, pos, SoundEvents::ITEM_FLINTANDSTEEL_USE, SoundCategory::BLOCKS, 1.0F, MathHelper::nextFloat(itemRand) * 0.4F + 0.8F);
            worldIn->setBlockState(pos, Blocks::FIRE::getDefaultState(), 11);
        }

        if (Util::instanceof< EntityPlayerMP>(player)) 
        {
            CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP*)player, pos, itemstack);
        }

        itemstack.damageItem(1, player);
        return EnumActionResult::SUCCESS;
    }
}
