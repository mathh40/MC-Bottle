#include "EntityAIRestrictSun.h"

#include "../../inventory/EntityEquipmentSlot.h"
#include "../../pathfinding/PathNavigateGround.h"

EntityAIRestrictSun::EntityAIRestrictSun(EntityCreature *creature)
    :entity(creature)
{

}

bool EntityAIRestrictSun::shouldExecute()
{
    return entity->world.isDaytime() && entity->getItemStackFromSlot(EntityEquipmentSlot::HEAD).isEmpty();
}

void EntityAIRestrictSun::startExecuting()
{
    ((PathNavigateGround)entity->getNavigator())->setAvoidSun(true);
}

void EntityAIRestrictSun::resetTask()
{
    ((PathNavigateGround)entity->getNavigator())->setAvoidSun(false);
}
