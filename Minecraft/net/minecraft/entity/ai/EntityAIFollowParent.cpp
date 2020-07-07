#include "EntityAIFollowParent.h"

#include <vector>

EntityAIFollowParent::EntityAIFollowParent(EntityAnimal *animal, double speed)
    :childAnimal(animal),moveSpeed(speed)
{
}

bool EntityAIFollowParent::shouldExecute()
{
    if (childAnimal->getGrowingAge() >= 0) 
    {
        return false;
    }
    else 
    {
        std::vector<EntityAnimal*> list = childAnimal->world.getEntitiesWithinAABB(childAnimal->getClass(), childAnimal->getEntityBoundingBox().grow(8.0, 4.0, 8.0));
        EntityAnimal* entityanimal = nullptr;
        double d0 = std::numeric_limits<double>::max();

        for(auto entityanimal1 : list) 
        {
            if (entityanimal1->getGrowingAge() >= 0) 
            {
                double d1 = childAnimal->getDistanceSq(entityanimal1);
                if (d1 <= d0) 
                {
                    d0 = d1;
                    entityanimal = entityanimal1;
                }
            }
        }

        if (entityanimal == nullptr) 
        {
            return false;
        }
        else if (d0 < 9.0) 
        {
            return false;
        }
        else 
        {
            parentAnimal = entityanimal;
            return true;
        }
    }
}

bool EntityAIFollowParent::shouldContinueExecuting()
{
    if (childAnimal->getGrowingAge() >= 0) 
    {
        return false;
    }
    else if (!parentAnimal->isEntityAlive()) 
    {
        return false;
    }
    else 
    {
        double d0 = childAnimal->getDistanceSq(parentAnimal);
        return d0 >= 9.0 && d0 <= 256.0;
    }
}

void EntityAIFollowParent::startExecuting()
{
    delayCounter = 0;
}

void EntityAIFollowParent::resetTask()
{
    parentAnimal = nullptr;
}

void EntityAIFollowParent::updateTask()
{
    if (--delayCounter <= 0) 
    {
        delayCounter = 10;
        childAnimal->getNavigator().tryMoveToEntityLiving(parentAnimal, moveSpeed);
    }
}
