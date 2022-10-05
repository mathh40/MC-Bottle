#include "EntityAIWanderAvoidWater.h"

#include "RandomPositionGenerator.h"

EntityAIWanderAvoidWater::EntityAIWanderAvoidWater(EntityCreature *p_i47301_1_, double p_i47301_2_)
    : EntityAIWanderAvoidWater(p_i47301_1_, p_i47301_2_, 0.001F)
{
}

EntityAIWanderAvoidWater::EntityAIWanderAvoidWater(EntityCreature *p_i47302_1_, double p_i47302_2_, float p_i47302_4_)
    : EntityAIWander(p_i47302_1_, p_i47302_2_), probability(p_i47302_4_)
{
}

std::optional<Vec3d> EntityAIWanderAvoidWater::getPosition()
{
    if (entity->isInWater())
    {
        Vec3d vec3d = RandomPositionGenerator::getLandPos(entity, 15, 7);
        return vec3d ? vec3d : EntityAIWander::getPosition();
    }
    else
    {
        return entity->getRNG().nextFloat() >= probability ? RandomPositionGenerator::getLandPos(entity, 10, 7)
                                                           : EntityAIWander::getPosition();
    }
}
