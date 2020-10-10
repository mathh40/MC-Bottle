#include "EntityMinecartHopper.h"

#include "DamageSource.h"
#include "../../inventory/ContainerHopper.h"

EntityMinecartHopper::EntityMinecartHopper(World *worldIn)
    :EntityMinecartContainer(worldIn),lastPosition(BlockPos::ORIGIN){
}

EntityMinecartHopper::EntityMinecartHopper(World *worldIn, double x, double y, double z)
    :EntityMinecartContainer(worldIn, x, y, z),lastPosition(BlockPos::ORIGIN){
}

EntityMinecart::Type EntityMinecartHopper::getType()
{
    return EntityMinecart::Type::HOPPER;
}

IBlockState * EntityMinecartHopper::getDefaultDisplayTile() {
    return Blocks::HOPPER->getDefaultState();
}

int32_t EntityMinecartHopper::getDefaultDisplayTileOffset() const{
    return 1;
}

int32_t EntityMinecartHopper::getSizeInventory() const{
    return 5;
}

bool EntityMinecartHopper::processInitialInteract(EntityPlayer *player, EnumHand hand) {
    if (!world->isRemote) {
         player->displayGUIChest(this);
      }

      return true;
}

void EntityMinecartHopper::onActivatorRailPass(int32_t x, int32_t y, int32_t z, bool receivingPower) {
    bool flag = !receivingPower;
      if (flag != getBlocked()) {
         setBlocked(flag);
      }
}

bool EntityMinecartHopper::getBlocked() const {
    return isBlocked;
}

void EntityMinecartHopper::setBlocked(bool p_96110_1_) {
    isBlocked = p_96110_1_;
}

World * EntityMinecartHopper::getWorld() {
    return world;
}

double EntityMinecartHopper::getXPos() {
    return posX;
}

double EntityMinecartHopper::getYPos() {
    return posY + 0.5;
}

double EntityMinecartHopper::getZPos() {
    return posZ;
}

void EntityMinecartHopper::onUpdate() {
    EntityMinecartContainer::onUpdate();
      if (!world->isRemote && isEntityAlive() && getBlocked()) {
         BlockPos blockpos = BlockPos(this);
         if (blockpos == lastPosition) {
            --transferTicker;
         } else {
            setTransferTicker(0);
         }

         if (!canTransfer()) {
            setTransferTicker(0);
            if (captureDroppedItems()) {
               setTransferTicker(4);
               markDirty();
            }
         }
      }
}

bool EntityMinecartHopper::captureDroppedItems() {
    if (TileEntityHopper::pullItems(this)) {
         return true;
      } else {
         auto list = world->getEntitiesWithinAABB<EntityItem>(getEntityBoundingBox().grow(0.25, 0.0, 0.25), EntitySelectors::IS_ALIVE);
         if (!list.empty()) {
            TileEntityHopper::putDropInInventoryAllSlots(nullptr, (IInventory*)this, list[0]);
         }

         return false;
      }
}

void EntityMinecartHopper::killMinecart(DamageSource::DamageSource source) {
    EntityMinecartContainer::killMinecart(source);
    if (world->getGameRules().getBoolean("doEntityDrops")) {
        dropItemWithOffset(Item::getItemFromBlock(Blocks::HOPPER), 1, 0.0F);
    }
}

void EntityMinecartHopper::registerFixesMinecartHopper(DataFixer fixer) {
    EntityMinecartContainer::addDataFixers(fixer, EntityMinecartHopper.class);
}

void EntityMinecartHopper::setTransferTicker(int32_t p_98042_1_) {
    transferTicker = p_98042_1_;
}

bool EntityMinecartHopper::canTransfer() const {
    return transferTicker > 0;
}

std::string EntityMinecartHopper::getGuiID() const {
    return "minecraft:hopper";
}

Container * EntityMinecartHopper::createContainer(InventoryPlayer *playerInventory, EntityPlayer *playerIn) {
    return new ContainerHopper(playerInventory, (IInventory*)this, playerIn);
}

void EntityMinecartHopper::writeEntityToNBT(NBTTagCompound *compound) {
    EntityMinecartContainer::writeEntityToNBT(compound);
    compound->setInteger("TransferCooldown", transferTicker);
    compound->setBoolean("Enabled", isBlocked);
}

void EntityMinecartHopper::readEntityFromNBT(NBTTagCompound *compound) {
    EntityMinecartContainer::readEntityFromNBT(compound);
    transferTicker = compound->getInteger("TransferCooldown");
    isBlocked = compound->hasKey("Enabled") ? compound->getBoolean("Enabled") : true;
}
