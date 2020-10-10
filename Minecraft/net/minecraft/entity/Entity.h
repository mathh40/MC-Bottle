#pragma once
#include <unordered_set>

#include "EnumActionResult.h"
#include "SoundEvent.h"
#include "../../../../crossguid/include/crossguid/guid.hpp"
#include "../../../../spdlog/include/spdlog/logger.h"
#include "../command/CommandResultStats.h"
#include "../command/ICommandSender.h"
#include "../nbt/NBTTagList.h"
#include "../tileentity/TileEntityHopper.h"
#include "../util/text/event/HoverEvent.h"
#include "math/AxisAlignedBB.h"

class DataParameter;
enum class MoverType;

namespace std {
    class type_index;
}

class EntityPlayerMP;
class EntityEquipmentSlot;
class DataFixer;
class Team;
class EntityLightningBolt;

namespace DamageSource {
    class DamageSource;
}

class EntityPlayer;
class Material;
class Block;
class IBlockState;
class CrashReportCategory;


class Entity :public ICommandSender ,public IWorldNameable
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
    virtual void onKillCommand();
    EntityDataManager getDataManager();
    friend bool operator==(const Entity& lhs , const Entity& rhs);
    virtual void setDead();
    virtual void setDropItemsWhenDead(bool dropWhenDead);
    virtual void setPosition(double x, double y, double z);
    void turn(float yaw, float pitch);
    virtual void onUpdate();
    virtual void onEntityUpdate();
    int32_t getMaxInPortalTime() const;
    void setFire(int32_t seconds);
    void extinguish();
    bool isOffsetPositionInLiquid(double x, double y, double z);
    virtual void move(const MoverType& type, double x, double y, double z);
    void resetPositionToBB();
    void playSound(SoundEvent soundIn, float volume, float pitch);
    bool isSilent();
    void setSilent(bool isSilent);
    bool hasNoGravity();
    void setNoGravity(bool noGravity);
    virtual std::optional<AxisAlignedBB> getCollisionBoundingBox();
    bool isImmuneToFire() const;
    virtual void fall(float distance, float damageMultiplier);
    bool isWet();
    bool isInWater();
    bool isOverWater();
    bool handle_water_movement();
    void spawnRunningParticles();
    bool isInsideOfMaterial(const Material& materialIn);
    bool isInLava();
    void moveRelative(float strafe, float up, float forward, float friction);
    virtual int32_t getBrightnessForRender();
    virtual float getBrightness();
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
    virtual void applyEntityCollision(Entity* entityIn);
    virtual void addVelocity(double x, double y, double z);
    virtual bool attackEntityFrom(DamageSource::DamageSource source, float amount);
    virtual Vec3d getLook(float partialTicks);
    Vec3d getPositionEyes(float partialTicks);
    std::optional<RayTraceResult> rayTrace(double blockReachDistance, float partialTicks);
    virtual bool canBeCollidedWith();
    virtual bool canBePushed();
    void awardKillScore(Entity* p_191956_1_, int32_t p_191956_2_, DamageSource::DamageSource p_191956_3_);
    virtual bool isInRangeToRender3d(double x, double y, double z);
    virtual bool isInRangeToRenderDist(double distance);
    bool writeToNBTAtomically(NBTTagCompound* compound);
    virtual bool writeToNBTOptional(NBTTagCompound* compound);
    static void registerFixes(DataFixer fixer);
    NBTTagCompound* writeToNBT(NBTTagCompound* compound);
    void readFromNBT(NBTTagCompound* compound);
    EntityItem* dropItem(const Item* itemIn, int32_t size);
    EntityItem* dropItemWithOffset(const Item* itemIn, int32_t size, float offsetY);
    virtual EntityItem* entityDropItem(ItemStack stack, float offsetY);
    bool isEntityAlive() const;
    bool isEntityInsideOpaqueBlock();
    virtual bool processInitialInteract(EntityPlayer* player, EnumHand hand);
    virtual std::optional<AxisAlignedBB> getCollisionBox(Entity* entityIn);
    virtual void updateRidden();
    virtual void updatePassenger(Entity* passenger);
    virtual void applyOrientationToEntity(Entity* entityToUpdate);
    virtual double getYOffset();
    virtual double getMountedYOffset() const;
    bool startRiding(Entity* entityIn);
    virtual bool startRiding(Entity* entityIn, bool force);
    void removePassengers();
    virtual void dismountRidingEntity();
    virtual void setPositionAndRotationDirect(double x, double y, double z, float yaw, float pitch, int32_t posRotationIncrements, bool teleport);
    virtual float getCollisionBorderSize();
    Vec3d getLookVec();
    Vec2f getPitchYaw() const;
    Vec3d getForward() const;
    void setPortal(const BlockPos& pos);
    int32_t getPortalCooldown();
    virtual void setVelocity(double x, double y, double z);
    virtual void handleStatusUpdate(std::byte id);
    virtual void performHurtAnimation();
    virtual std::vector<> getHeldEquipment();
    std::vector<> getArmorInventoryList();
    std::vector<> getEquipmentAndArmor();
    void setItemStackToSlot(EntityEquipmentSlot slotIn, ItemStack stack);
    bool isBurning();
    bool isRiding();
    bool isBeingRidden();
    bool isSneaking();
    void setSneaking(bool sneaking);
    bool isSprinting();
    virtual void setSprinting(bool sprinting);
    bool isGlowing();
    void setGlowing(bool glowingIn);
    bool isInvisible();
    bool isInvisibleToPlayer(EntityPlayer* player);
    Team* getTeam();
    bool isOnSameTeam(Entity* entityIn);
    bool isOnScoreboardTeam(Team* teamIn);
    virtual void setInvisible(bool invisible);
    int32_t getAir();
    void setAir(int32_t air);
    virtual void onStruckByLightning(EntityLightningBolt* lightningBolt);
    void onKillEntity(EntityLivingBase* entityLivingIn);
    void setInWeb();
    std::string getName() override;
    std::vector<Entity*> getParts();
    virtual bool isEntityEqual(Entity* entityIn);
    virtual float getRotationYawHead();
    virtual void setRotationYawHead(float rotation);
    virtual void setRenderYawOffset(float offset);
    virtual bool canBeAttackedWithItem();
    virtual bool hitByEntity(Entity* entityIn);
    std::string toString();
    bool isEntityInvulnerable(DamageSource::DamageSource source);
    bool getIsInvulnerable() const;
    void setEntityInvulnerable(bool isInvulnerable);
    void copyLocationAndAnglesFrom(Entity* entityIn);
    virtual Entity* changeDimension(int32_t dimensionIn);
    bool isNonBoss();
    virtual float getExplosionResistance(Explosion explosionIn, World* worldIn, BlockPos pos, IBlockState* blockStateIn);
    virtual bool canExplosionDestroyBlock(Explosion explosionIn, World* worldIn, BlockPos pos, IBlockState* blockStateIn, float p_174816_5_);
    virtual int32_t getMaxFallHeight();
    Vec3d getLastPortalVec() const;
    EnumFacing getTeleportDirection() const;
    bool doesEntityNotTriggerPressurePlate();
    void addEntityCrashInfo(CrashReportCategory& category);
    virtual bool canRenderOnFire();
    void setUniqueId(xg::Guid uniqueIdIn);
    xg::Guid getUniqueID() const;
    std::string_view getCachedUniqueIdString() const;
    bool isPushedByWater();
    static double getRenderDistanceWeight();
    static void setRenderDistanceWeight(double renderDistWeight);
    ITextComponent* getDisplayName() override;
    void setCustomNameTag(std::string name);
    std::string getCustomNameTag();
    virtual bool hasCustomName() const;
    void setAlwaysRenderNameTag(bool alwaysRenderNameTag);
    bool getAlwaysRenderNameTag();
    void setPositionAndUpdate(double x, double y, double z);
    virtual bool getAlwaysRenderNameTagForRender();
    virtual void notifyDataManagerChange(DataParameter key);
    EnumFacing getHorizontalFacing() const;
    virtual EnumFacing getAdjustedHorizontalFacing() const;
    bool isSpectatedByPlayer(EntityPlayerMP* player);
    AxisAlignedBB getEntityBoundingBox() const;
    AxisAlignedBB getRenderBoundingBox() const;
    void setEntityBoundingBox(AxisAlignedBB bb);
    float getEyeHeight() const;
    bool isOutsideBorder() const;
    void setOutsideBorder(bool outsideBorder);
    virtual bool replaceItemInInventory(int32_t inventorySlot, ItemStack itemStackIn);
    void sendMessage(ITextComponent* component) override;
    bool canUseCommand(int32_t permLevel, std::string_view commandName) override;
    BlockPos getPosition() override;
    Vec3d getPositionVector() override;
    World* getEntityWorld() override;
    Entity* getCommandSenderEntity() override;
    bool sendCommandFeedback() override;
    void setCommandStat(const CommandResultStatsType& type, int32_t amount) override;
    MinecraftServer* getServer() override;
    CommandResultStats getCommandStats() const;
    void setCommandStats(Entity* entityIn);
    virtual EnumActionResult applyPlayerInteraction(EntityPlayer* player, Vec3d vec, EnumHand hand);
    virtual bool isImmuneToExplosions();
    void addTrackingPlayer(EntityPlayerMP* player);
    void removeTrackingPlayer(EntityPlayerMP* player);
    float getRotatedYaw(Rotation transformRotation) const;
    float getMirroredYaw(Mirror transformMirror) const;
    virtual bool ignoreItemEntityData();
    bool setPositionNonDirty();
    virtual Entity* getControllingPassenger();
    std::vector<Entity*> getPassengers() const;
    bool isPassenger(Entity* entityIn) const;
    std::unordered_set<Entity*> getRecursivePassengers() const;
    std::unordered_set<Entity*> getRecursivePassengersByType(std::type_index entityClass) const;
    Entity* getLowestRidingEntity();
    bool isRidingSameEntity(Entity* entityIn);
    bool isRidingOrBeingRiddenBy(Entity* entityIn) const;
    virtual bool canPassengerSteer();
    Entity* getRidingEntity() const;
    virtual EnumPushReaction getPushReaction();
    virtual SoundCategory getSoundCategory();



protected:
    virtual void entityInit() = 0;
    void preparePlayerToSpawn();
    virtual void setSize(float width, float height);
    void setRotation(float yaw, float pitch);
    void decrementTimeUntilPortal();
    void setOnFireFromLava();
    virtual void outOfWorld();
    SoundEvent getSwimSound();
    SoundEvent getSplashSound();
    virtual void doBlockCollisions();
    virtual void onInsideBlock(IBlockState* p_191955_1_);
    void playStepSound(BlockPos pos, Block* blockIn);
    float playFlySound(float p_191954_1_);
    bool makeFlySound();
    virtual bool canTriggerWalking();
    virtual void updateFallState(double y, bool onGroundIn, IBlockState* state, const BlockPos& pos);
    virtual void dealFireDamage(int32_t amount);
    void doWaterSplashEffect();
    void createRunningParticles();
    virtual void markVelocityChanged();
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
    virtual bool canFitPassenger(Entity* passenger);
    bool getFlag(int32_t flag);
    void setFlag(int32_t flag, bool set);
    bool pushOutOfBlocks(double x, double y, double z);
    HoverEvent getHoverEvent();
    void applyEnchantments(EntityLivingBase* entityLivingBaseIn, Entity* entityIn);
    int32_t getFireImmuneTicks();


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
    void copyDataFromOld(Entity* entityIn);
    void getRecursivePassengersByType(std::type_index entityClass, std::unordered_set<Entity*> theSet) const;


    static std::shared_ptr<spdlog::logger> LOGGER;
    static std::vector<> EMPTY_EQUIPMENT;
    static AxisAlignedBB ZERO_AABB = AxisAlignedBB(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    static double renderDistanceWeight = 1.0;
    static int32_t nextEntityID;
    int32_t entityId;
    std::vector< Entity*> riddenByEntities;
    Entity* ridingEntity;
    AxisAlignedBB boundingBox;
    bool OutsideBorder;
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
