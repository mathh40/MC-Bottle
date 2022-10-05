#include "EntityGiantZombie.h"

#include "../SharedMonsterAttributes.h"
#include "../ai/attributes/IAttributeInstance.h"

EntityGiantZombie::EntityGiantZombie(World *worldIn) : EntityMob(worldIn)
{
    setSize(width * 6.0F, height * 6.0F);
}

void EntityGiantZombie::registerFixesGiantZombie(DataFixer fixer)
{
    EntityLiving::registerFixesMob(fixer, EntityGiantZombie.class);
}

float EntityGiantZombie::getEyeHeight() const
{
    return 10.440001F;
}

float EntityGiantZombie::getBlockPathWeight(BlockPos pos)
{
    return world->getLightBrightness(pos) - 0.5F;
}

std::optional<ResourceLocation> EntityGiantZombie::getLootTable()
{
    return LootTableList::ENTITIES_GIANT;
}

void EntityGiantZombie::applyEntityAttributes()
{
    EntityMob::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(100.0);
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.5);
    getEntityAttribute(SharedMonsterAttributes::ATTACK_DAMAGE.get())->setBaseValue(50.0);
}
