#include "EntityAIWatchClosest2.h"

EntityAIWatchClosest2::EntityAIWatchClosest2(EntityLiving *entityIn, std::type_index watchTargetClass,
    float maxDistance, float chanceIn)
    :EntityAIWatchClosest(entityIn,watchTargetClass,maxDistance,chanceIn)

{
    setMutexBits(3);
}
