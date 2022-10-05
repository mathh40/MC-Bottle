#include "EntityMinecartChest.h"

#include "../../inventory/ContainerChest.h"
#include "DamageSource.h"
#include "datafix/DataFixer.h"

EntityMinecartChest::EntityMinecartChest(World *worldIn) : EntityMinecartContainer(worldIn)
{
}

EntityMinecartChest::EntityMinecartChest(World *worldIn, double x, double y, double z)
    : EntityMinecartContainer(worldIn, x, y, z)
{
}

void EntityMinecartChest::registerFixesMinecartChest(DataFixer fixer)
{
    EntityMinecartContainer::addDataFixers(fixer, EntityMinecartChest.class);
}

void EntityMinecartChest::killMinecart(DamageSource::DamageSource source)
{
    EntityMinecartContainer::killMinecart(source);
    if (world->getGameRules().getBoolean("doEntityDrops"))
    {
        dropItemWithOffset(Item::getItemFromBlock(Blocks::CHEST), 1, 0.0F);
    }
}

int32_t EntityMinecartChest::getSizeInventory() const
{
    return 27;
}

EntityMinecart::Type EntityMinecartChest::getType()
{
    return EntityMinecart::Type::CHEST;
}

IBlockState *EntityMinecartChest::getDefaultDisplayTile()
{
    return Blocks::CHEST->getDefaultState()->withProperty(BlockChest::FACING, EnumFacing::NORTH);
}

int32_t EntityMinecartChest::getDefaultDisplayTileOffset() const
{
    return 8;
}

std::string EntityMinecartChest::getGuiID() const
{
    return "minecraft:chest";
}

Container *EntityMinecartChest::createContainer(InventoryPlayer *playerInventory, EntityPlayer *playerIn)
{
    addLoot(playerIn);
    return new ContainerChest(playerInventory, this, playerIn);
}
