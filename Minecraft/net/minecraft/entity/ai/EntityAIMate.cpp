#include "EntityAIMate.h"

#include <limits>


#include "EnumParticleTypes.h"
#include "../../stats/StatList.h"

class EntityPlayerMP;

EntityAIMate::EntityAIMate(EntityAnimal *animal, double speedIn)
    :EntityAIMate(animal, speedIn, typeid(animal))
{
}

EntityAIMate::EntityAIMate(EntityAnimal *p_i47306_1_, double p_i47306_2_, std::type_index p_i47306_4_)
    :animal(p_i47306_1_),world(p_i47306_1_->world),mateClass (p_i47306_4_),moveSpeed (p_i47306_2_)
{
    setMutexBits(3);
}

bool EntityAIMate::shouldExecute()
{
    if (!animal->isInLove()) 
    {
        return false;
    }
    else 
    {
        targetMate = getNearbyMate();
        return targetMate != nullptr;
    }
}

bool EntityAIMate::shouldContinueExecuting()
{
    return targetMate->isEntityAlive() && targetMate->isInLove() && spawnBabyDelay < 60;
}

void EntityAIMate::resetTask()
{
    targetMate = nullptr;
    spawnBabyDelay = 0;
}

void EntityAIMate::updateTask()
{
    animal->getLookHelper().setLookPositionWithEntity(targetMate, 10.0F, animal->getVerticalFaceSpeed());
    animal->getNavigator().tryMoveToEntityLiving(targetMate, moveSpeed);
    ++spawnBabyDelay;
    if (spawnBabyDelay >= 60 && animal->getDistanceSq(targetMate) < 9.0) 
    {
        spawnBaby();
    }
}

EntityAnimal * EntityAIMate::getNearbyMate()
{
    auto list = world->getEntitiesWithinAABB(mateClass, animal->getEntityBoundingBox().grow(8.0));
    double d0 = std::numeric_limits<double>::max();
    EntityAnimal* entityanimal = nullptr;

    for(auto entityanimal1 : list) 
    {
        if (animal->canMateWith(entityanimal1) && animal->getDistanceSq(entityanimal1) < d0) 
        {
            entityanimal = entityanimal1;
            d0 = animal->getDistanceSq(entityanimal1);
        }
    }

    return entityanimal;
}

void EntityAIMate::spawnBaby()
{
    EntityAgeable* entityageable = animal->createChild(targetMate);
    if (entityageable != nullptr) 
    {
        EntityPlayerMP* entityplayermp = animal->getLoveCause();
        if (entityplayermp == nullptr && targetMate->getLoveCause() != nullptr) 
        {
            entityplayermp = targetMate->getLoveCause();
        }

        if (entityplayermp != nullptr) 
        {
            entityplayermp.addStat(StatList::ANIMALS_BRED);
            CriteriaTriggers::BRED_ANIMALS.trigger(entityplayermp, animal, targetMate, entityageable);
        }

        animal->setGrowingAge(6000);
        targetMate->setGrowingAge(6000);
        animal->resetInLove();
        targetMate->resetInLove();
        entityageable.setGrowingAge(-24000);
        entityageable.setLocationAndAngles(animal->posX, animal->posY, animal->posZ, 0.0F, 0.0F);
        world->spawnEntity(entityageable);
        auto random = animal->getRNG();

        for(auto i = 0; i < 7; ++i) 
        {
            double d0 = random.nextGaussian() * 0.02;
            double d1 = random.nextGaussian() * 0.02;
            double d2 = random.nextGaussian() * 0.02;
            double d3 = random.nextDouble() * (double)animal->width * 2.0 - (double)animal->width;
            double d4 = 0.5 + random.nextDouble() * (double)animal->height;
            double d5 = random.nextDouble() * (double)animal->width * 2.0 - (double)animal->width;
            world->spawnParticle(EnumParticleTypes::HEART, animal->posX + d3, animal->posY + d4, animal->posZ + d5, d0, d1, d2);
        }

        if (world->getGameRules().getBoolean("doMobLoot")) 
        {
            world->spawnEntity(new EntityXPOrb(world, animal->posX, animal->posY, animal->posZ, random.nextInt(7) + 1));
        }
    }
}
