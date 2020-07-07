#include "EntitySenses.h"

EntitySenses::EntitySenses(EntityLiving *entityIn)
    :entity(entityIn)
{
}

void EntitySenses::clearSensingCache()
{
    seenEntities.clear();
    unseenEntities.clear();
}

bool EntitySenses::canSee(Entity *entityIn)
{
    
    if (std::find(seenEntities.begin(),seenEntities.end(),entityIn) != seenEntities.end()) 
    {
        return true;
    }
    else if (std::find(unseenEntities.begin(),unseenEntities.end(),entityIn) != unseenEntities.end()) 
    {
        return false;
    }
    else 
    {
        entity->world.profiler.startSection("canSee");
        bool flag = entity->canEntityBeSeen(entityIn);
        entity->world.profiler.endSection();
        if (flag) 
        {
            seenEntities.emplace_back(entityIn);
        }
        else 
        {
            unseenEntities.emplace_back(entityIn);
        }

        return flag;
    }
}
