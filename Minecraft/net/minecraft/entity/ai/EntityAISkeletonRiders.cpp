#include "EntityAISkeletonRiders.h"

#include "DifficultyInstance.h"
#include "../Entity.h"
#include "../../inventory/ContainerHorseInventory.h"
#include "math/BlockPos.h"

EntityAISkeletonRiders::EntityAISkeletonRiders(EntitySkeletonHorse *horseIn)
    :horse(horseIn)
{
}

bool EntityAISkeletonRiders::shouldExecute()
{
    return horse->world->isAnyPlayerWithinRangeAt(horse->posX, horse->posY, horse->posZ, 10.0);
}

void EntityAISkeletonRiders::updateTask()
{
    DifficultyInstance difficultyinstance = horse->world->getDifficultyForLocation(BlockPos(horse));
    horse->setTrap(false);
    horse->setHorseTamed(true);
    horse->setGrowingAge(0);
    horse->world.addWeatherEffect(new EntityLightningBolt(horse->world, horse->posX, horse->posY, horse->posZ, true));
    EntitySkeleton entityskeleton = createSkeleton(difficultyinstance, horse);
    entityskeleton.startRiding(horse);

    for(int i = 0; i < 3; ++i) 
    {
        AbstractHorse* abstracthorse = createHorse(difficultyinstance);
        EntitySkeleton* entityskeleton1 = createSkeleton(difficultyinstance, abstracthorse);
        entityskeleton1.startRiding(abstracthorse);
        abstracthorse.addVelocity(horse->getRNG().nextGaussian() * 0.5, 0.0, horse->getRNG().nextGaussian() * 0.5);
    }
}

AbstractHorse * EntityAISkeletonRiders::createHorse(const DifficultyInstance &p_188515_1_) const
{
    EntitySkeletonHorse* entityskeletonhorse = new EntitySkeletonHorse(horse->world);
    entityskeletonhorse->onInitialSpawn(p_188515_1_, nullptr);
    entityskeletonhorse->setPosition(horse->posX, horse->posY, horse->posZ);
    entityskeletonhorse->hurtResistantTime = 60;
    entityskeletonhorse->enablePersistence();
    entityskeletonhorse->setHorseTamed(true);
    entityskeletonhorse->setGrowingAge(0);
    entityskeletonhorse->world.spawnEntity(entityskeletonhorse);
    return entityskeletonhorse;
}

EntitySkeleton * EntityAISkeletonRiders::createSkeleton(const DifficultyInstance p_188514_1_,
    const AbstractHorse *p_188514_2_)
{
    EntitySkeleton* entityskeleton = new EntitySkeleton(p_188514_2_->world);
    entityskeleton->onInitialSpawn(p_188514_1_, nullptr);
    entityskeleton->setPosition(p_188514_2_->posX, p_188514_2_->posY, p_188514_2_->posZ);
    entityskeleton->hurtResistantTime = 60;
    entityskeleton->enablePersistence();
    if (entityskeleton->getItemStackFromSlot(EntityEquipmentSlot::HEAD).isEmpty()) 
    {
        entityskeleton->setItemStackToSlot(EntityEquipmentSlot::HEAD, ItemStack(Items::IRON_HELMET));
    }

    entityskeleton->setItemStackToSlot(EntityEquipmentSlot::MAINHAND, EnchantmentHelper::addRandomEnchantment(entityskeleton->getRNG(), entityskeleton->getHeldItemMainhand(), (int)(5.0F + p_188514_1_.getClampedAdditionalDifficulty() * (float)entityskeleton->getRNG().nextInt(18)), false));
    entityskeleton->setItemStackToSlot(EntityEquipmentSlot::HEAD, EnchantmentHelper::addRandomEnchantment(entityskeleton->getRNG(), entityskeleton->getItemStackFromSlot(EntityEquipmentSlot::HEAD), (int)(5.0F + p_188514_1_.getClampedAdditionalDifficulty() * (float)entityskeleton->getRNG().nextInt(18)), false));
    entityskeleton->world.spawnEntity(entityskeleton);
    return entityskeleton;
}
