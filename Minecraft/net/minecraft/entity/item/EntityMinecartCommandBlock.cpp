#include "EntityMinecartCommandBlock.h"
#include "../../tileentity/TileEntityCommandBlock.h"
#include "datafix/DataFixer.h"
#include "text/TextComponentString.h"

DataParameter MinecartCommandBlockLogic::COMMAND = EntityDataManager::createKey(EntityMinecartCommandBlock.class, DataSerializers::STRING);
DataParameter MinecartCommandBlockLogic::LAST_OUTPUT = EntityDataManager::createKey(EntityMinecartCommandBlock.class, DataSerializers::TEXT_COMPONENT);

MinecartCommandBlockLogic::MinecartCommandBlockLogic(EntityMinecartCommandBlock *block)
    :block(block){
}

void MinecartCommandBlockLogic::updateCommand() {
    block->getDataManager().set(EntityMinecartCommandBlock::COMMAND, getCommand());
    block->getDataManager().set(EntityMinecartCommandBlock::LAST_OUTPUT, getLastOutput());
}

int32_t MinecartCommandBlockLogic::getCommandBlockType() {
    return 1;
}

void MinecartCommandBlockLogic::fillInInfo(ByteBuffer &buf) {
    buf.putInt(block->getEntityId());
}

BlockPos MinecartCommandBlockLogic::getPosition() const {
    return BlockPos(block->posX, block->posY + 0.5, block->posZ);
}

Vec3d MinecartCommandBlockLogic::getPositionVector() const {
    return Vec3d(block->posX, block->posY, block->posZ);
}

World *MinecartCommandBlockLogic::getEntityWorld() const {
    return block->world;
}

Entity *MinecartCommandBlockLogic::getCommandSenderEntity() const {
    return block;
}

MinecraftServer *MinecartCommandBlockLogic::getServer() const {
    return block->world->getMinecraftServer();
}

EntityMinecartCommandBlock::EntityMinecartCommandBlock(World *worldIn)
    :EntityMinecart(worldIn),commandBlockLogic(this){

}

EntityMinecartCommandBlock::EntityMinecartCommandBlock(World *worldIn, double x, double y, double z)
    :EntityMinecart(worldIn, x, y, z),commandBlockLogic(this){
}

void EntityMinecartCommandBlock::registerFixesMinecartCommand(DataFixer fixer) {
    EntityMinecart::registerFixesMinecart(fixer, EntityMinecartCommandBlock.class);
      fixer.registerWalker(FixTypes::ENTITY, new IDataWalker() {
         public NBTTagCompound process(IDataFixer fixer, NBTTagCompound* compound, int32_t versionIn) {
            if (TileEntity::getKey(TileEntityCommandBlock.class).equals(ResourceLocation(compound.getString("id")))) {
               compound.setString("id", "Control");
               fixer.process(FixTypes.BLOCK_ENTITY, compound, versionIn);
               compound.setString("id", "MinecartCommandBlock");
            }

            return compound;
         }
      });
}

EntityMinecart::Type EntityMinecartCommandBlock::getType() {
    return EntityMinecart::Type::COMMAND_BLOCK;
}

IBlockState * EntityMinecartCommandBlock::getDefaultDisplayTile() {
    return Blocks::COMMAND_BLOCK::getDefaultState();
}

CommandBlockBaseLogic * EntityMinecartCommandBlock::getCommandBlockLogic() {
    return &commandBlockLogic;
}

void EntityMinecartCommandBlock::onActivatorRailPass(int32_t x, int32_t y, int32_t z, bool receivingPower) {
    if (receivingPower && ticksExisted - activatorRailCooldown >= 4) {
         getCommandBlockLogic()->trigger(world);
         activatorRailCooldown = ticksExisted;
      }
}

bool EntityMinecartCommandBlock::processInitialInteract(EntityPlayer *player, EnumHand hand) {
    commandBlockLogic.tryOpenEditCommandBlock(player);
    return false;
}

void EntityMinecartCommandBlock::notifyDataManagerChange(DataParameter key) {
    EntityMinecart::notifyDataManagerChange(key);
      if (LAST_OUTPUT == key) {
         try {
            commandBlockLogic.setLastOutput((ITextComponent*)getDataManager().get(LAST_OUTPUT));
         } catch (std::exception& var3) {
         }
      } else if (COMMAND == key) {
         commandBlockLogic.setCommand(getDataManager().get(COMMAND));
      }
}

bool EntityMinecartCommandBlock::ignoreItemEntityData() {
    return true;
}

void EntityMinecartCommandBlock::entityInit() {
    EntityMinecart::entityInit();
    this.getDataManager().registe(COMMAND, "");
    this.getDataManager().registe(LAST_OUTPUT, new TextComponentString(""));
}

void EntityMinecartCommandBlock::readEntityFromNBT(NBTTagCompound *compound) {
    EntityMinecart::readEntityFromNBT(compound);
    commandBlockLogic.readDataFromNBT(compound);
    getDataManager().set(COMMAND, getCommandBlockLogic()->getCommand());
    getDataManager().set(LAST_OUTPUT, getCommandBlockLogic()->getLastOutput());
}

void EntityMinecartCommandBlock::writeEntityToNBT(NBTTagCompound *compound) {
    EntityMinecart::writeEntityToNBT(compound);
    commandBlockLogic.writeToNBT(compound);
}
