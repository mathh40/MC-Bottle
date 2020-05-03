#include "ItemSpade.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "material/Material.h"

std::unordered_set<Block*> ItemSpade::EFFECTIVE_ON = {Blocks::CLAY, Blocks::DIRT, Blocks::FARMLAND, Blocks::GRASS, Blocks::GRAVEL, Blocks::MYCELIUM, Blocks::SAND, Blocks::SNOW, Blocks::SNOW_LAYER, Blocks::SOUL_SAND, Blocks::GRASS_PATH, Blocks::CONCRETE_POWDER};

ItemSpade::ItemSpade(ToolMaterial material)
    :ItemTool(1.5F, -3.0F, material, EFFECTIVE_ON)
{
}

bool ItemSpade::canHarvestBlock(IBlockState *blockIn)
{
    Block* block = blockIn->getBlock();
    if (block == Blocks::SNOW_LAYER) 
    {
        return true;
    }
    else 
    {
        return block == Blocks::SNOW;
    }
}

EnumActionResult ItemSpade::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    ItemStack itemstack = player->getHeldItem(hand);
    if (!player->canPlayerEdit(pos.offset(facing), facing, itemstack)) 
    {
        return EnumActionResult::FAIL;
    }
    else 
    {
        IBlockState* iblockstate = worldIn->getBlockState(pos);
        Block* block = iblockstate->getBlock();
        if (facing != EnumFacing::DOWN && worldIn->getBlockState(pos.up()).getMaterial() == Material::AIR && block == Blocks::GRASS) 
        {
            IBlockState* iblockstate1 = Blocks::GRASS_PATH.getDefaultState();
            worldIn->playSound(player, pos, SoundEvents::ITEM_SHOVEL_FLATTEN, SoundCategory::BLOCKS, 1.0F, 1.0F);
            if (!worldIn->isRemote) 
            {
                worldIn->setBlockState(pos, iblockstate1, 11);
                itemstack.damageItem(1, player);
            }

            return EnumActionResult::SUCCESS;
        }
        else 
        {
            return EnumActionResult::PASS;
        }
    }
}
