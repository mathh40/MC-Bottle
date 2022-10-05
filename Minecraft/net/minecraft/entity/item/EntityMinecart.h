#pragma once
#include "../Entity.h"
#include "multivec.h"

class EntityMinecart : public Entity
{
  public:
    class Type
    {
      public:
        static Type RIDEABLE;
        static Type CHEST;
        static Type FURNACE;
        static Type TNT;
        static Type SPAWNER;
        static Type HOPPER;
        static Type COMMAND_BLOCK;

        Type(int32_t idIn, std::string nameIn);

        int32_t getId() const;

        std::string getName() const;

        static EntityMinecart::Type *getById(int32_t idIn);

      private:
        static std::unordered_map<int32_t, EntityMinecart::Type *> BY_ID;
        int32_t id;
        std::string name;
    };

    EntityMinecart(World *worldIn);
    EntityMinecart(World *worldIn, double x, double y, double z);

    static std::unique_ptr<EntityMinecart> create(World *worldIn, double x, double y, double z,
                                                  EntityMinecart::Type typeIn);
    std::optional<AxisAlignedBB> getCollisionBox(Entity *entityIn) override;
    std::optional<AxisAlignedBB> getCollisionBoundingBox() override;
    bool canBePushed() override;
    double getMountedYOffset() const override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    virtual void killMinecart(DamageSource::DamageSource source);
    void performHurtAnimation() override;
    bool canBeCollidedWith() override;
    EnumFacing getAdjustedHorizontalFacing() const override;
    void onUpdate() override;
    virtual void onActivatorRailPass(int32_t x, int32_t y, int32_t z, bool receivingPower);
    void setPosition(double x, double y, double z) override;
    std::optional<Vec3d> getPosOffset(double x, double y, double z, double offset);
    std::optional<Vec3d> getPos(double p_70489_1_, double p_70489_3_, double p_70489_5_);
    AxisAlignedBB getRenderBoundingBox();
    static void registerFixesMinecart(DataFixer fixer, Class name);
    void applyEntityCollision(Entity *entityIn) override;
    void setPositionAndRotationDirect(double x, double y, double z, float yaw, float pitch,
                                      int32_t posRotationIncrements, bool teleport) override;
    void setVelocity(double x, double y, double z) override;
    void setDamage(float damage);
    float getDamage();
    void setRollingAmplitude(int32_t rollingAmplitude);
    int32_t getRollingAmplitude();
    void setRollingDirection(int32_t rollingDirection);
    int32_t getRollingDirection();
    virtual EntityMinecart::Type getType() = 0;
    IBlockState *getDisplayTile();
    virtual IBlockState *getDefaultDisplayTile();
    int32_t getDisplayTileOffset();
    virtual int32_t getDefaultDisplayTileOffset() const;
    void setDisplayTile(IBlockState *displayTile);
    void setDisplayTileOffset(int32_t displayTileOffset);
    bool hasDisplayTile();
    void setHasDisplayTile(bool showBlock);

  protected:
    bool canTriggerWalking() override;
    void entityInit() override;
    virtual double getMaximumSpeed() const;
    void moveDerailedMinecart();
    virtual void moveAlongTrack(const BlockPos &pos, IBlockState *state);
    virtual void applyDrag();
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void writeEntityToNBT(NBTTagCompound *compound) override;

  private:
    static DataParameter ROLLING_AMPLITUDE;
    static DataParameter ROLLING_DIRECTION;
    static DataParameter DAMAGE;
    static DataParameter DISPLAY_TILE;
    static DataParameter DISPLAY_TILE_OFFSET;
    static DataParameter SHOW_BLOCK;
    bool isInReverse;
    static constexpr std::array<multivec<int32_t>, 8> MATRIX = {
        {{0, 0, -1}, {0, 0, 1}},  {{-1, 0, 0}, {1, 0, 0}},  {{-1, -1, 0}, {1, 0, 0}}, {{-1, 0, 0}, {1, -1, 0}},
        {{0, 0, -1}, {0, -1, 1}}, {{0, -1, -1}, {0, 0, 1}}, {{0, 0, 1}, {1, 0, 0}},   {{0, 0, 1}, {-1, 0, 0}},
        {{0, 0, -1}, {-1, 0, 0}}, {{0, 0, -1}, {1, 0, 0}}};
    int32_t turnProgress;
    double minecartX;
    double minecartY;
    double minecartZ;
    double minecartYaw;
    double minecartPitch;
    double velocityX;
    double velocityY;
    double velocityZ;
};
