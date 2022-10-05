#pragma once
#include "../../pathfinding/PathHeap.h"
#include "../../world/end/DragonFightManager.h"
#include "../EntityLiving.h"
#include "../IEntityMultiPart.h"
#include "../MultiPartEntityPart.h"
#include "../monster/IMob.h"
#include "dragon/phase/PhaseManager.h"

class EntityDragon : public EntityLiving, public IEntityMultiPart, public IMob
{
  public:
    EntityDragon(World *worldIn);
    std::array<double, 3> getMovementOffsets(int32_t p_70974_1_, float p_70974_2_);
    void onLivingUpdate() override;
    bool attackEntityFromPart(MultiPartEntityPart *part, DamageSource::DamageSource source, float damage) override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    void onKillCommand() override;
    int32_t initPathPoints();
    int32_t getNearestPpIdx(double x, double y, double z);
    std::optional<Path> findPath(int32_t startIdx, int32_t finishIdx, std::optional<PathPoint> andThen);
    static void registerFixesDragon(DataFixer fixer);
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    std::vector<Entity *> getParts() override;
    bool canBeCollidedWith() override;
    World *getWorld() override;
    SoundCategory getSoundCategory() override;
    float getHeadPartYOffset(int32_t p_184667_1_, double p_184667_2_[], double p_184667_3_[]);
    Vec3d getHeadLookVec(float p_184665_1_);
    void onCrystalDestroyed(EntityEnderCrystal *crystal, const BlockPos &pos, DamageSource::DamageSource dmgSrc);
    void notifyDataManagerChange(DataParameter key) override;
    PhaseManager getPhaseManager();
    const std::optional<DragonFightManager> getFightManager() const;
    void addPotionEffect(PotionEffect potioneffectIn) override;
    bool isNonBoss() override;

    static DataParameter PHASE;
    std::array<std::array<double, 3>, 64> ringBuffer;
    int32_t ringBufferIndex                            = -1;
    MultiPartEntityPart dragonPartHead                 = MultiPartEntityPart(this, "head", 6.0F, 6.0F);
    MultiPartEntityPart dragonPartNeck                 = MultiPartEntityPart(this, "neck", 6.0F, 6.0F);
    MultiPartEntityPart dragonPartBody                 = MultiPartEntityPart(this, "body", 8.0F, 8.0F);
    MultiPartEntityPart dragonPartTail1                = MultiPartEntityPart(this, "tail", 4.0F, 4.0F);
    MultiPartEntityPart dragonPartTail2                = MultiPartEntityPart(this, "tail", 4.0F, 4.0F);
    MultiPartEntityPart dragonPartTail3                = MultiPartEntityPart(this, "tail", 4.0F, 4.0F);
    MultiPartEntityPart dragonPartWing1                = MultiPartEntityPart(this, "wing", 4.0F, 4.0F);
    MultiPartEntityPart dragonPartWing2                = MultiPartEntityPart(this, "wing", 4.0F, 4.0F);
    std::array<MultiPartEntityPart, 8> dragonPartArray = {dragonPartHead,  dragonPartNeck,  dragonPartBody,
                                                          dragonPartTail1, dragonPartTail2, dragonPartTail3,
                                                          dragonPartWing1, dragonPartWing2};

    float prevAnimTime;
    float animTime;
    bool slowed;
    int32_t deathTicks;
    EntityEnderCrystal *healingEnderCrystal;

  protected:
    void applyEntityAttributes() override;
    void entityInit() override;
    bool attackDragonFrom(DamageSource::DamageSource source, float amount);
    void onDeathUpdate() override;
    void despawnEntity() override;
    SoundEvent getAmbientSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    float getSoundVolume() override;
    ResourceLocation getLootTable();
    bool canBeRidden(Entity *entityIn) override;

  private:
    float getHeadYOffset(float p_184662_1_);
    void updateDragonEnderCrystal();
    void collideWithEntities(const std::vector<Entity *> &p_70970_1_);
    void attackEntitiesInList(const std::vector<Entity *> &p_70971_1_);
    float simplifyAngle(double p_70973_1_);
    bool destroyBlocksInAABB(const AxisAlignedBB &p_70972_1_);
    void dropExperience(int32_t p_184668_1_) const;
    Path makePath(PathPoint start, PathPoint finish);

    std::optional<DragonFightManager> fightManager;
    PhaseManager phaseManager;
    int32_t growlTime = 200;
    int32_t sittingDamageReceived;
    std::array<std::optional<PathPoint>, 24> pathPoints;
    std::array<int32_t, 24> neighbors;
    PathHeap pathFindQueue;
    static std::shared_ptr<spdlog::logger> LOGGER;
};
