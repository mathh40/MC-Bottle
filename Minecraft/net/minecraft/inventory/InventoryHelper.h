#include <span>
class ItemStack;
class Entity;
class IInventory;
class BlockPos;
class World;

namespace InventoryHelper
{
constexpr

    void
    dropInventoryItems(World *worldIn, BlockPos pos, IInventory *inventory);
void dropInventoryItems(World *worldIn, Entity *entityAt, IInventory *inventory);
void dropInventoryItems(World *worldIn, double x, double y, double z, IInventory *inventory);
void spawnItemStack(World *worldIn, double x, double y, double z, ItemStack stack);
bool canMergeStacks(ItemStack stack1, ItemStack stack2, IInventory *inventory);
bool stackEqualExact(ItemStack stack1, ItemStack stack2);
bool isInventoryEmpty(std::span<const ItemStack> inventory);

} // namespace InventoryHelper
