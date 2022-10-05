#pragma once
#include "../../tileentity/CommandBlockBaseLogic.h"
#include "EntityMinecart.h"

class EntityMinecartCommandBlock;

class MinecartCommandBlockLogic : public CommandBlockBaseLogic
{
  public:
    MinecartCommandBlockLogic(EntityMinecartCommandBlock *block);

    void updateCommand() override;

    int32_t getCommandBlockType() override;

    void fillInInfo(ByteBuffer &buf) override;

    BlockPos getPosition() const;

    Vec3d getPositionVector() const;

    World *getEntityWorld() const;

    Entity *getCommandSenderEntity() const;

    MinecraftServer *getServer() const;

  private:
    EntityMinecartCommandBlock *block;
};

class EntityMinecartCommandBlock : public EntityMinecart
{
  public:
    EntityMinecartCommandBlock(World *worldIn);
    EntityMinecartCommandBlock(World *worldIn, double x, double y, double z);

    static void registerFixesMinecartCommand(DataFixer fixer);
    EntityMinecart::Type getType() override;
    IBlockState *getDefaultDisplayTile() override;
    CommandBlockBaseLogic *getCommandBlockLogic();
    void onActivatorRailPass(int32_t x, int32_t y, int32_t z, bool receivingPower) override;
    bool processInitialInteract(EntityPlayer *player, EnumHand hand) override;
    void notifyDataManagerChange(DataParameter key) override;
    bool ignoreItemEntityData() override;

  protected:
    void entityInit() override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void writeEntityToNBT(NBTTagCompound *compound) override;

  private:
    static DataParameter COMMAND;
    static DataParameter LAST_OUTPUT;
    MinecartCommandBlockLogic commandBlockLogic;
    int32_t activatorRailCooldown = 0;
};
