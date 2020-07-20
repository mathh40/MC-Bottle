#pragma once
#include <typeindex>

#include "EntityAIBase.h"

class EntityAnimal;
class World;

class EntityAIMate :public EntityAIBase
{
public:
    EntityAIMate(EntityAnimal* animal, double speedIn);
    EntityAIMate(EntityAnimal* p_i47306_1_, double p_i47306_2_, std::type_index p_i47306_4_);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void resetTask() override;
    void updateTask() override;

private:
    EntityAnimal* getNearbyMate();
    void spawnBaby();

    EntityAnimal* animal;
    std::type_index mateClass;
    World* world;
    EntityAnimal* targetMate;
    int32_t spawnBabyDelay;
    double moveSpeed;
};
