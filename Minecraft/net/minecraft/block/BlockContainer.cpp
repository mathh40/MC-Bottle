#include "BlockContainer.h"
#include "../util/EnumFacing.h"
#include "../world/IWorldNameable.h"
#include "../util/Util.h"

BlockContainer::BlockContainer(Material materialIn) :
    Block(materialIn, materialIn.getMaterialMapColor()) {

}

BlockContainer::BlockContainer(Material materialIn, MapColor color) :
    Block(materialIn, color) {
    bTileEntity = true;
}

bool BlockContainer::isInvalidNeighbor(World worldIn, BlockPos pos, EnumFacing facing) {
    return worldIn.getBlockState(pos.offset(facing)).getMaterial() == Material.CACTUS;
}

bool BlockContainer::hasInvalidNeighbor(World worldIn, BlockPos pos) {
    return isInvalidNeighbor(worldIn, pos, EnumFacing::NORTH) || isInvalidNeighbor(worldIn, pos, EnumFacing::SOUTH) ||
           isInvalidNeighbor(worldIn, pos, EnumFacing::WEST) || isInvalidNeighbor(worldIn, pos, EnumFacing::EAST);
}

EnumBlockRenderType BlockContainer::getRenderType(IBlockState state) {
    return EnumBlockRenderType::INVISIBLE;
}

void BlockContainer::breakBlock(World worldIn, BlockPos pos, IBlockState state) {
    Block::breakBlock(worldIn, pos, state);
    worldIn.removeTileEntity(pos);
}

void BlockContainer::harvestBlock(World worldIn, EntityPlayer player, BlockPos pos, IBlockState state,
                                  std::optional<TileEntity> te, ItemStack stack) {
    if (Util::instanceof<IWorldNameable>(te) && ((IWorldNameable)te).hasCustomName()) {
        player.addStat(StatList.getBlockStats(this));
        player.addExhaustion(0.005F);
        if (worldIn.isRemote) {
            return;
        }

        int i = EnchantmentHelper.getEnchantmentLevel(Enchantments.FORTUNE, stack);
        Item item = getItemDropped(state, worldIn.rand, i);
        if (item == Items.AIR) {
            return;
        }

        ItemStack itemstack = ItemStack(item, quantityDropped(worldIn.rand));
        itemstack.setStackDisplayName(((IWorldNameable)te).getName());
        spawnAsEntity(worldIn, pos, itemstack);
    } else {
        Block::harvestBlock(worldIn, player, pos, state, std::nullopt, stack);
    }
}

bool BlockContainer::eventReceived(IBlockState state, World worldIn, BlockPos pos, int32_t id, int32_t param) {
    Block::eventReceived(state, worldIn, pos, id, param);
    auto tileentity = worldIn.getTileEntity(pos);
    return !tileentity ? false : tileentity.receiveClientEvent(id, param);
}
