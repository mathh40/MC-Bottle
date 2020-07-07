#pragma once
#include <unordered_set>


#include "SoundEvent.h"
#include "../../../../crossguid/include/crossguid/guid.hpp"
#include "../../../../spdlog/include/spdlog/logger.h"
#include "../command/CommandResultStats.h"
#include "../command/ICommandSender.h"
#include "../nbt/NBTTagList.h"
#include "../tileentity/TileEntityHopper.h"
#include "math/AxisAlignedBB.h"

class DataFixer;

namespace DamageSource {
    class DamageSource;
}

class EntityPlayer;
class Material;
class Block;
class IBlockState;
class MoverType;

class Entity :public ICommandSender
{
public:
    bool preventEntitySpawning;
    bool forceSpawn;
    World* world;
    double prevPosX;
    double prevPosY;
    double prevPosZ;
    double posX;
    double posY;
    double posZ;
    double motionX;
    double motionY;
    double motionZ;
    float rotationYaw;
    float rotationPitch;
    float prevRotationYaw;
    float prevRotationPitch;
    bool onGround;
    bool collidedHorizontally;
    bool collidedVertically;
    bool collided;
    bool velocityChanged;
    bool isDead;
    float width;
    float height;
    float prevDistanceWalkedModified;
    float distanceWalkedModified;
    float distanceWalkedOnStepModified;
    float fallDistance;
    double lastTickPosX;
    double lastTickPosY;
    double lastTickPosZ;
    float stepHeight;
    bool noClip;
    float entityCollisionReduction;
    int32_t ticksExisted;
    int32_t hurtResistantTime;
    bool addedToChunk;
    int32_t chunkCoordX;
    int32_t chunkCoordY;
    int32_t chunkCoordZ;
    int64_t serverPosX;
    int64_t serverPosY;
    int64_t serverPosZ;
    bool ignoreFrustumCheck;
    bool isAirBorne;
    int32_t timeUntilPortal;
    int32_t dimension;

    Entity(World* worldIn);
    int32_t getEntityId() const;
    void setEntityId(int32_t id);
    std::unordered_set<std::string> getTags() const;
    bool addTag(std::string_view tag);
    bool removeTag(std::string_view tag);
    void onKillCommand();
    EntityDataManager getDataManager();
    friend bool operator==(const Entity& lhs , const Entity& rhs);
    void setDead();
    virtual void setDropItemsWhenDead(bool dropWhenDead);
    void setPosition(double x, double y, double z);
    void turn(float yaw, float pitch);
    void onUpdate();
    void onEntityUpdate();
    int32_t getMaxInPortalTime() const;
    void setFire(int32_t seconds);
    void extinguish();
    bool isOffsetPositionInLiquid(double x, double y, double z);
    void move(const MoverType& type, double x, double y, double z);
    void resetPositionToBB();
    void playSound(SoundEvent soundIn, float volume, float pitch);
    bool isSilent();
    void setSilent(bool isSilent);
    bool hasNoGravity();
    void setNoGravity(bool noGravity);
    std::optional<AxisAlignedBB> getCollisionBoundingBox();
    bool isImmuneToFire() const;
    void fall(float distance, float damageMultiplier);
    bool isWet();
    bool isInWater();
    bool isOverWater();
    bool handle_water_movement();
    void spawnRunningParticles();
    bool isInsideOfMaterial(const Material& materialIn);
    bool isInLava();
    void moveRelative(float strafe, float up, float forward, float friction);
    int32_t getBrightnessForRender();
    float getBrightness();
    void setWorld(World* worldIn);
    void setPositionAndRotation(double x, double y, double z, float yaw, float pitch);
    void moveToBlockPosAndAngles(BlockPos pos, float rotationYawIn, float rotationPitchIn);
    void setLocationAndAngles(double x, double y, double z, float yaw, float pitch);
    float getDistance(Entity* entityIn) const;
    double getDistanceSq(double x, double y, double z) const;
    double getDistanceSq(const BlockPos& pos) const;
    double getDistanceSqToCenter(const BlockPos& pos) const;
    double getDistance(double x, double y, double z) const;
    double getDistanceSq(Entity* entityIn) const;
    virtual void onCollideWithPlayer(EntityPlayer* entityIn);
    void applyEntityCollision(Entity* entityIn);
    void addVelocity(double x, double y, double z);
    bool attackEntityFrom(DamageSource::DamageSource source, float amount);
    Vec3d getLook(float partialTicks);
    Vec3d getPositionEyes(float partialTicks);
    std::optional<RayTraceResult> rayTrace(double blockReachDistance, float partialTicks);
    virtual bool canBeCollidedWith();
    virtual bool canBePushed();
    void awardKillScore(Entity* p_191956_1_, int32_t p_191956_2_, DamageSource::DamageSource p_191956_3_);
    bool isInRangeToRender3d(double x, double y, double z);
    bool isInRangeToRenderDist(double distance);
    bool writeToNBTAtomically(NBTTagCompound* compound);
    bool writeToNBTOptional(NBTTagCompound* compound);
    static void registerFixes(DataFixer fixer);
    NBTTagCompound* writeToNBT(NBTTagCompound* compound);
    void readFromNBT(NBTTagCompound* compound);
    EntityItem* dropItem(const Item* itemIn, int32_t size);
    EntityItem* dropItemWithOffset(const Item* itemIn, int32_t size, float offsetY);
    EntityItem* entityDropItem(ItemStack stack, float offsetY);
    bool isEntityAlive() const;
    bool isEntityInsideOpaqueBlock();
    bool processInitialInteract(EntityPlayer* player, EnumHand hand);
    std::optional<AxisAlignedBB> getCollisionBox(Entity* entityIn);
    void updateRidden();
    void updatePassenger(Entity* passenger);
    virtual void applyOrientationToEntity(Entity* entityToUpdate);
    double getYOffset();
    double getMountedYOffset() const;
    bool startRiding(Entity* entityIn);
    bool startRiding(Entity* entityIn, bool force);
    void removePassengers();
    void dismountRidingEntity();
    void setPositionAndRotationDirect(double x, double y, double z, float yaw, float pitch, int32_t posRotationIncrements, bool teleport);
    float getCollisionBorderSize();


protected:
    virtual void entityInit() = 0;
    void preparePlayerToSpawn();
    void setSize(float width, float height);
    void setRotation(float yaw, float pitch);
    void decrementTimeUntilPortal();
    void setOnFireFromLava();
    void outOfWorld();
    SoundEvent getSwimSound();
    SoundEvent getSplashSound();
    virtual void doBlockCollisions();
    virtual void onInsideBlock(IBlockState* p_191955_1_);
    void playStepSound(BlockPos pos, Block* blockIn);
    float playFlySound(float p_191954_1_);
    bool makeFlySound();
    bool canTriggerWalking();
    void updateFallState(double y, bool onGroundIn, IBlockState* state, BlockPos pos);
    void dealFireDamage(int32_t amount);
    void doWaterSplashEffect();
    void createRunningParticles();
    void markVelocityChanged();
    Vec3d getVectorForRotation(float pitch, float yaw);
    virtual bool shouldSetPosAfterLoading();
    std::string getEntityString();
    virtual void readEntityFromNBT(NBTTagCompound* var1) = 0;
    virtual void writeEntityToNBT(NBTTagCompound* var1) = 0;
    NBTTagList* newDoubleNBTList(std::initializer_list<double> numbers);
    NBTTagList* newFloatNBTList(std::initializer_list<float> numbers);
    bool canBeRidden(Entity* entityIn);
    void addPassenger(Entity* passenger);
    void removePassenger(Entity* passenger);
    bool canFitPassenger(Entity* passenger);


    int32_t rideCooldown;
    bool isInWeb;
    pcg32 rand;
    bool inWater;
    bool firstUpdate;
    bool bisImmuneToFire;
    EntityDataManager dataManager;
    static DataParameter FLAGS;
    bool inPortal;
    int32_t portalCounter;
    BlockPos lastPortalPos;
    Vec3d lastPortalVec;
    EnumFacing teleportDirection;
    xg::Guid entityUniqueID;
    std::string cachedUniqueIdString;
    bool glowing;
private:
    bool isLiquidPresentInAABB(const AxisAlignedBB& bb);


    static std::shared_ptr<spdlog::logger> LOGGER;
    static std::vector<> EMPTY_EQUIPMENT;
    static AxisAlignedBB ZERO_AABB = AxisAlignedBB(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    static double renderDistanceWeight = 1.0;
    static int32_t nextEntityID;
    int32_t entityId;
    std::vector< Entity*> riddenByEntities;
    Entity* ridingEntity;
    AxisAlignedBB boundingBox;
    bool isOutsideBorder;
    int32_t nextStepDistance;
    float nextFlap;
    int32_t fire;
    static DataParameter AIR;
    static DataParameter CUSTOM_NAME;
    static DataParameter CUSTOM_NAME_VISIBLE;
    static DataParameter SILENT;
    static DataParameter NO_GRAVITY;
    bool invulnerable;
    CommandResultStats cmdResultStats;
    std::unordered_set<std::string> tags;
    bool isPositionDirty;
    std::array<double,3> pistonDeltas;
    int64_t pistonDeltasGameTime;
};

template<>
class MyHash<Entity>
{
public:
    size_t operator()(const Entity &s) const 
    {
        return s.getEntityId();
    }
};
