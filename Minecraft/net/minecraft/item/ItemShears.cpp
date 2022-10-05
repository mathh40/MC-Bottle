#include "ItemShears.h"

#include "ItemStack.h"
#include "World.h"
#include "material/Material.h"

ItemShears::ItemShears()
{
    setMaxStackSize(1);
    setMaxDamage(238);
    setCreativeTab(CreativeTabs::TOOLS);
}

bool ItemShears::onBlockDestroyed(ItemStack stack, World *worldIn, IBlockState *state, BlockPos pos,
                                  EntityLivingBase *entityLiving)
{
    if (!worldIn->isRemote)
    {
        stack.damageItem(1, entityLiving);
    }

    Block *block = state->getBlock();
    return state->getMaterial() != Material::LEAVES && block != Blocks::WEB && block != Blocks::TALLGRASS &&
                   block != Blocks::VINE && block != Blocks::TRIPWIRE && block != Blocks::WOOL
               ? Item::onBlockDestroyed(stack, worldIn, state, pos, entityLiving)
               : true;
}

bool ItemShears::canHarvestBlock(IBlockState *blockIn)
{
    Block *block = blockIn->getBlock();
    return block == Blocks::WEB || block == Blocks::REDSTONE_WIRE || block == Blocks::TRIPWIRE;
}

float ItemShears::getDestroySpeed(ItemStack stack, IBlockState *state)
{
    Block *block = state->getBlock();
    if (block != Blocks::WEB && state->getMaterial() != Material::LEAVES)
    {
        return block == Blocks::WOOL ? 5.0F : Item::getDestroySpeed(stack, state);
    }
    else
    {
        return 15.0F;
    }
}
