class ItemStack;
class Entity;
class IInventory;
class BlockPos;
class World;

namespace InventoryHelper
{
    void dropInventoryItems(World* worldIn, BlockPos pos, IInventory* inventory);
    void dropInventoryItems(World* worldIn, Entity* entityAt, IInventory* inventory);
    void dropInventoryItems(World* worldIn, double x, double y, double z, IInventory* inventory);
    void spawnItemStack(World* worldIn, double x, double y, double z, ItemStack stack);
}
