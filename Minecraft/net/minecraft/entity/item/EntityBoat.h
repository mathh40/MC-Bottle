#pragma once
#include "../Entity.h"

class DataParameter;
class EntityBoat : public Entity
{
  public:
    enum class Status
    {
        IN_WATER,
        UNDER_WATER,
        UNDER_FLOWING_WATER,
        ON_LAND,
        IN_AIR
    };

    class Type
    {
      public:
        Type(int32_t metadataIn, std::string_view nameIn);
        std::string getName() const;
        int32_t getMetadata() const;
        std::string toString() const;
        static EntityBoat::Type &byId(int32_t id);
        static std::vector<EntityBoat::Type *> values();
        static EntityBoat::Type &getTypeFromString(std::string_view nameIn);

        static EntityBoat::Type OAK;
        static EntityBoat::Type SPRUCE;
        static EntityBoat::Type BIRCH;
        static EntityBoat::Type JUNGLE;
        static EntityBoat::Type ACACIA;
        static EntityBoat::Type DARK_OAK;

      private:
        std::string name;
        int32_t metadata;
        static std::vector<EntityBoat::Type *> value;
    };

    EntityBoat(World *worldIn);
    EntityBoat(World *worldIn, double x, double y, double z);
    std::optional<AxisAlignedBB> getCollisionBox(Entity *entityIn) override;
    std::optional<AxisAlignedBB> getCollisionBoundingBox() override;
    bool canBePushed() override;
    double getMountedYOffset() const override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    void applyEntityCollision(Entity *entityIn) override;
    Item *getItemBoat();
    void performHurtAnimation() override;
    bool canBeCollidedWith() override;
    void setPositionAndRotationDirect(double x, double y, double z, float yaw, float pitch,
                                      int32_t posRotationIncrements, bool teleport) override;
    EnumFacing getAdjustedHorizontalFacing() const;
    void onUpdate() override;
    void setPaddleState(bool left, bool right);
    float getRowingTime(int side, float limbSwing);
    float getWaterLevelAbove();
    float getBoatGlide();
    void updatePassenger(Entity *passenger) override;
    void applyOrientationToEntity(Entity *entityToUpdate) override;
    bool processInitialInteract(EntityPlayer *player, EnumHand hand) override;
    bool getPaddleState(int32_t side);
    void setDamageTaken(float damageTaken);
    float getDamageTaken();
    void setTimeSinceHit(int32_t timeSinceHit);
    int32_t getTimeSinceHit();
    void setForwardDirection(int32_t forwardDirection);
    int32_t getForwardDirection();
    void setBoatType(EntityBoat::Type boatType);
    EntityBoat::Type getBoatType();
    Entity *getControllingPassenger() override;
    void updateInputs(bool p_184442_1_, bool p_184442_2_, bool p_184442_3_, bool p_184442_4_);

  protected:
    bool canTriggerWalking() override;
    void entityInit() override;
    std::optional<SoundEvent> getPaddleSound();
    void applyYawToEntity(Entity *entityToUpdate) const;
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void updateFallState(double y, bool onGroundIn, IBlockState *state, const BlockPos &pos) override;
    bool canFitPassenger(Entity *passenger) override;

  private:
    void tickLerp();
    EntityBoat::Status getBoatStatus();
    bool checkInWater();
    std::optional<EntityBoat::Status> getUnderwaterStatus();
    void updateMotion();
    void controlBoat();

    static const DataParameter TIME_SINCE_HIT;
    static const DataParameter FORWARD_DIRECTION;
    static const DataParameter DAMAGE_TAKEN;
    static const DataParameter BOAT_TYPE;
    static const std::array<DataParameter, 2> DATA_ID_PADDLE;
    std::array<float, 2> paddlePositions;
    float momentum;
    float outOfControlTicks;
    float deltaRotation;
    int32_t lerpSteps;
    double lerpX;
    double lerpY;
    double lerpZ;
    double lerpYaw;
    double lerpPitch;
    bool leftInputDown;
    bool rightInputDown;
    bool forwardInputDown;
    bool backInputDown;
    double waterLevel;
    float boatGlide;
    EntityBoat::Status status;
    EntityBoat::Status previousStatus;
    double lastYd;
};
