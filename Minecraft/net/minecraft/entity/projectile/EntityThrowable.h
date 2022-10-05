#pragma once
#include "../Entity.h"
#include "../IProjectile.h"

class EntityThrowable : public Entity, public IProjectile
{
  public:
    EntityThrowable(World *worldIn);
    EntityThrowable(World *worldIn, double x, double y, double z);
    EntityThrowable(World *worldIn, EntityLivingBase *throwerIn);
    bool isInRangeToRenderDist(double distance) override;
    void shoot(Entity *entityThrower, float rotationPitchIn, float rotationYawIn, float pitchOffset, float velocity,
               float inaccuracy);
    void shoot(double x, double y, double z, float velocity, float inaccuracy) override;
    void setVelocity(double x, double y, double z) override;
    void onUpdate() override;
    static void registerFixesThrowable(DataFixer fixer, std::string_view name);
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    EntityLivingBase *getThrower();

    Entity *ignoreEntity;
    int32_t throwableShake;

  protected:
    void entityInit() override;
    virtual float getGravityVelocity();
    virtual void onImpact(RayTraceResult var1) = 0;

    bool inGround;
    EntityLivingBase *thrower;

  private:
    int32_t xTile;
    int32_t yTile;
    int32_t zTile;
    Block *inTile;
    std::string throwerName;
    int32_t ticksInGround;
    int32_t ticksInAir;
    int32_t ignoreTime;
};
