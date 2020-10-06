#pragma once
#include "../projectile/EntityThrowable.h"

class EntityExpBottle :public EntityThrowable {
public:
    explicit EntityExpBottle(World* worldIn);
    EntityExpBottle(World* worldIn, EntityLivingBase* throwerIn);
    EntityExpBottle(World* worldIn, double x, double y, double z);
    static void registerFixesExpBottle(DataFixer fixer);

protected:
    float getGravityVelocity() override;
    void onImpact(RayTraceResult result) override;
};