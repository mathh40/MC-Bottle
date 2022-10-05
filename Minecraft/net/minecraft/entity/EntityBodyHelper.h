#pragma once
#include <cstdint>
class EntityLivingBase;

class EntityBodyHelper
{
  public:
    EntityBodyHelper(EntityLivingBase *livingIn);
    void updateRenderAngles();

  private:
    float computeAngleWithBound(float p_75665_1_, float p_75665_2_, float p_75665_3_) const;

    EntityLivingBase *living;
    int32_t rotationTickCounter;
    float prevRenderYawHead;
};
