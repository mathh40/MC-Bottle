#pragma once
#include "projectile/EntityThrowable.h"

class EntityEnderPearl : public EntityThrowable
{
  public:
    EntityEnderPearl(World *worldIn);
    EntityEnderPearl(World *worldIn, EntityLivingBase *throwerIn);
    EntityEnderPearl(World *worldIn, double x, double y, double z);
    static void registerFixesEnderPearl(DataFixer fixer);
    void onUpdate() override;
    Entity *changeDimension(int32_t dimensionIn) override;

  protected:
    void onImpact(RayTraceResult result) override;

  private:
    EntityLivingBase *perlThrower;
};
