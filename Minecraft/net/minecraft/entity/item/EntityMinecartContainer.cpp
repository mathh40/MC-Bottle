#include "EntityMinecartContainer.h"


#include "DamageSource.h"
#include "../../inventory/InventoryHelper.h"

EntityMinecartContainer::EntityMinecartContainer(World *worldIn)
    :EntityMinecart(worldIn),minecartContainerItems(36, ItemStack::EMPTY),dropContentsWhenDead(true){
}

EntityMinecartContainer::EntityMinecartContainer(World *worldIn, double x, double y, double z)
    :EntityMinecart(worldIn, x, y, z),minecartContainerItems(36, ItemStack::EMPTY),dropContentsWhenDead(true){
}

void EntityMinecartContainer::killMinecart(DamageSource::DamageSource source) {
    EntityMinecart::killMinecart(source);
    if (world->getGameRules().getBoolean("doEntityDrops")) {
        InventoryHelper::dropInventoryItems(world, this, (IInventory*)this);
    }
}

bool EntityMinecartContainer::isEmpty() {

}
