#pragma once
#include "EntityMinecartContainer.h"

class EntityMinecartHopper : public EntityMinecartContainer, public IHopper
{
  public:
    EntityMinecartHopper(World *worldIn);
    EntityMinecartHopper(World *worldIn, double x, double y, double z);
    EntityMinecart::Type getType() override;
    IBlockState *getDefaultDisplayTile() override;
    int32_t getDefaultDisplayTileOffset() const override;
    int32_t getSizeInventory() const override;
    bool processInitialInteract(EntityPlayer *player, EnumHand hand) override;
    void onActivatorRailPass(int32_t x, int32_t y, int32_t z, bool receivingPower) override;
    bool getBlocked() const;
    void setBlocked(bool p_96110_1_);
    World *getWorld() override;
    double getXPos() override;
    double getYPos() override;
    double getZPos() override;
    void onUpdate() override;
    bool captureDroppedItems();
    void killMinecart(DamageSource::DamageSource source) override;
    static void registerFixesMinecartHopper(DataFixer fixer);
    void setTransferTicker(int32_t p_98042_1_);
    bool canTransfer() const;
    std::string getGuiID() const override;
    Container *createContainer(InventoryPlayer *playerInventory, EntityPlayer *playerIn);

  protected:
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;

  private:
    bool isBlocked         = true;
    int32_t transferTicker = -1;
    BlockPos lastPosition;
};
