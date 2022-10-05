#include "EntityCaveSpider.h"

#include "../SharedMonsterAttributes.h"
#include "../ai/attributes/IAttributeInstance.h"
#include "datafix/DataFixer.h"

EntityCaveSpider::EntityCaveSpider(World *worldIn) : EntitySpider(worldIn)
{
    setSize(0.7F, 0.5F);
}

void EntityCaveSpider::registerFixesCaveSpider(DataFixer fixer)
{
    EntityLiving::registerFixesMob(fixer, EntityCaveSpider.class);
}

bool EntityCaveSpider::attackEntityAsMob(Entity *entityIn)
{
    if (EntitySpider::attackEntityAsMob(entityIn))
    {
        if (Util:: instanceof <EntityLivingBase>(entityIn))
        {
            auto i = 0;
            if (world->getDifficulty() == EnumDifficulty::NORMAL)
            {
                i = 7;
            }
            else if (world->getDifficulty() == EnumDifficulty::HARD)
            {
                i = 15;
            }

            if (i > 0)
            {
                ((EntityLivingBase *)entityIn)->addPotionEffect(PotionEffect(MobEffects::POISON, i * 20, 0));
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

IEntityLivingData *EntityCaveSpider::onInitialSpawn(DifficultyInstance difficulty, IEntityLivingData *livingdata)
{
    return livingdata;
}

float EntityCaveSpider::getEyeHeight() const
{
    return 0.45F;
}

void EntityCaveSpider::applyEntityAttributes()
{
    EntitySpider::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(12.0);
}

std::optional<ResourceLocation> EntityCaveSpider::getLootTable()
{
    return LootTableList::ENTITIES_CAVE_SPIDER;
}
