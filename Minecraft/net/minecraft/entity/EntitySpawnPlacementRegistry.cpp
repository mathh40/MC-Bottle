#include "EntitySpawnPlacementRegistry.h"

#include "ai/EntityAIBeg.h"
#include "ai/EntityAICreeperSwell.h"
#include "ai/EntityAIFollowGolem.h"
#include "ai/EntityAIOcelotSit.h"
#include "ai/EntityAISkeletonRiders.h"
#include "ai/EntityAIZombieAttack.h"

#include <typeindex>

EntitySpawnPlacementRegistry::EntitySpawnPlacementRegistry()
{
    ENTITY_PLACEMENTS.emplace(EntityBat.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityChicken.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityCow.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityHorse.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntitySkeletonHorse.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityZombieHorse.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityDonkey.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityMule.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityMooshroom.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityOcelot.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityPig.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityRabbit.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityParrot.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntitySheep.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntitySnowman.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntitySquid.class, EntityLiving.SpawnPlacementType.IN_WATER);
    ENTITY_PLACEMENTS.emplace(EntityIronGolem.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityWolf.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityVillager.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityDragon.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityWither.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityBlaze.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityCaveSpider.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityCreeper.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityEnderman.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityEndermite.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityGhast.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityGiantZombie.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityGuardian.class, EntityLiving.SpawnPlacementType.IN_WATER);
    ENTITY_PLACEMENTS.emplace(EntityMagmaCube.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityPigZombie.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntitySilverfish.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntitySkeleton.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityWitherSkeleton.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityStray.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntitySlime.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntitySpider.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityWitch.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityZombie.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityZombieVillager.class, EntityLiving.SpawnPlacementType.ON_GROUND);
    ENTITY_PLACEMENTS.emplace(EntityHusk.class, EntityLiving.SpawnPlacementType.ON_GROUND);
}

SpawnPlacementType EntitySpawnPlacementRegistry::getPlacementForEntity(std::type_index entityClass)
{
    return ENTITY_PLACEMENTS[entityClass];
}
