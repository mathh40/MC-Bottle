#pragma once
#include <vector>

class Entity;
class EntityLiving;
class EntitySenses
{
public:
    explicit EntitySenses(EntityLiving* entityIn);
    void clearSensingCache();
    bool canSee(Entity* entityIn);

private:
    EntityLiving* entity;
    std::vector<EntityLiving*> seenEntities;
    std::vector<EntityLiving*> unseenEntities;
};