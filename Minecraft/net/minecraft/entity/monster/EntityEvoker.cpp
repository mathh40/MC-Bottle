#include "EntityEvoker.h"

#include "../SharedMonsterAttributes.h"
#include "../ai/EntityAIAvoidEntity.h"
#include "../ai/EntityAIFollowGolem.h"
#include "../ai/EntityAIHurtByTarget.h"
#include "../ai/EntityAINearestAttackableTarget.h"
#include "../ai/EntityAISwimming.h"
#include "../ai/EntityAIWander.h"
#include "../ai/EntityAIWatchClosest.h"
#include "../ai/attributes/IAttributeInstance.h"

auto wololoSelector = [](EntitySheep *p_apply_1_) { return p_apply_1_->getFleeceColor() == EnumDyeColor::BLUE; };

EntityEvoker::EntityEvoker(World *worldIn) : EntitySpellcasterIllager(worldIn)
{
    setSize(0.6F, 1.95F);
    experienceValue = 10;
}

void EntityEvoker::registerFixesEvoker(DataFixer fixer)
{
    EntityLiving::registerFixesMob(fixer, EntityEvoker.class);
}

void EntityEvoker::readEntityFromNBT(NBTTagCompound *compound)
{
    EntitySpellcasterIllager::readEntityFromNBT(compound);
}

void EntityEvoker::writeEntityToNBT(NBTTagCompound *compound)
{
    EntitySpellcasterIllager::writeEntityToNBT(compound);
}

void EntityEvoker::onUpdate()
{
    EntitySpellcasterIllager::onUpdate();
}

bool EntityEvoker::isOnSameTeam(Entity *entityIn)
{
    if (entityIn == nullptr)
    {
        return false;
    }
    else if (entityIn == this)
    {
        return true;
    }
    else if (EntitySpellcasterIllager::isOnSameTeam(entityIn))
    {
        return true;
    }
    else if (Util:: instanceof <EntityVex>(entityIn))
    {
        return isOnSameTeam(((EntityVex *)entityIn)->getOwner());
    }
    else if (Util:: instanceof <EntityLivingBase>(entityIn) && ((EntityLivingBase *)entityIn)->getCreatureAttribute() ==
                                                                   EnumCreatureAttribute::ILLAGER)
    {
        return getTeam() == nullptr && entityIn->getTeam() == nullptr;
    }
    else
    {
        return false;
    }
}

EntityEvoker::AIWololoSpell::AIWololoSpell(EntityEvoker *entityEvokerIn)
    : AIUseSpell(entityEvokerIn), entityEvoker(entityEvokerIn)
{
}

bool EntityEvoker::AIWololoSpell::shouldExecute()
{
    if (entityEvoker->getAttackTarget() != nullptr)
    {
        return false;
    }
    else if (entityEvoker->isSpellcasting())
    {
        return false;
    }
    else if (entityEvoker->ticksExisted < spellCooldown)
    {
        return false;
    }
    else if (!entityEvoker->world->getGameRules().getBoolean("mobGriefing"))
    {
        return false;
    }
    else
    {
        auto list = entityEvoker->world->getEntitiesWithinAABB<EntitySheep>(
            entityEvoker->getEntityBoundingBox().grow(16.0, 4.0, 16.0), wololoSelector);
        if (list.empty())
        {
            return false;
        }
        else
        {
            entityEvoker->setWololoTarget(list[entityEvoker->rand(list.size())]);
            return true;
        }
    }
}

bool EntityEvoker::AIWololoSpell::shouldContinueExecuting()
{
    return entityEvoker->getWololoTarget() != nullptr && spellWarmup > 0;
}

void EntityEvoker::AIWololoSpell::resetTask()
{
    AIUseSpell::resetTask();
    entityEvoker->setWololoTarget(nullptr);
}

void EntityEvoker::AIWololoSpell::castSpell()
{
    EntitySheep *entitysheep = entityEvoker->getWololoTarget();
    if (entitysheep != nullptr && entitysheep->isEntityAlive())
    {
        entitysheep->setFleeceColor(EnumDyeColor::RED);
    }
}

int32_t EntityEvoker::AIWololoSpell::getCastWarmupTime()
{
    return 40;
}

int32_t EntityEvoker::AIWololoSpell::getCastingTime()
{
    return 60;
}

int32_t EntityEvoker::AIWololoSpell::getCastingInterval()
{
    return 140;
}

std::optional<SoundEvent> EntityEvoker::AIWololoSpell::getSpellPrepareSound()
{
    return SoundEvents::EVOCATION_ILLAGER_PREPARE_WOLOLO;
}

EntitySpellcasterIllager::SpellType EntityEvoker::AIWololoSpell::getSpellType()
{
    return EntitySpellcasterIllager::SpellType::WOLOLO;
}

EntityEvoker::AISummonSpell::AISummonSpell(EntityEvoker *entityEvokerIn)
    : AIUseSpell(entityEvokerIn), entityEvoker(entityEvokerIn)
{
}

bool EntityEvoker::AISummonSpell::shouldExecute()
{
    if (!AIUseSpell::shouldExecute())
    {
        return false;
    }
    else
    {
        auto i =
            entityEvoker->world->getEntitiesWithinAABB(EntityVex.class, entityEvoker->getEntityBoundingBox().grow(16.0))
                .size();
        return entityEvoker->rand(8) + 1 > i;
    }
}

int32_t EntityEvoker::AISummonSpell::getCastingTime()
{
    return 100;
}

int32_t EntityEvoker::AISummonSpell::getCastingInterval()
{
    return 340;
}

void EntityEvoker::AISummonSpell::castSpell()
{
    for (auto i = 0; i < 3; ++i)
    {
        BlockPos blockpos    = (BlockPos(entityEvoker)).add(-2 + entityEvoker->rand(5), 1, -2 + entityEvoker->rand(5));
        EntityVex *entityvex = new EntityVex(entityEvoker->world);
        entityvex->moveToBlockPosAndAngles(blockpos, 0.0F, 0.0F);
        entityvex->onInitialSpawn(entityEvoker->world->getDifficultyForLocation(blockpos), nullptr);
        entityvex->setOwner(EntityEvoker.this);
        entityvex->setBoundOrigin(blockpos);
        entityvex->setLimitedLife(20 * (30 + entityEvoker->rand(90)));
        entityEvoker->world->spawnEntity(entityvex);
    }
}

std::optional<SoundEvent> EntityEvoker::AISummonSpell::getSpellPrepareSound()
{
    return SoundEvents::EVOCATION_ILLAGER_PREPARE_SUMMON;
}

EntitySpellcasterIllager::SpellType EntityEvoker::AISummonSpell::getSpellType()
{
    return EntitySpellcasterIllager::SpellType::SUMMON_VEX;
}

EntityEvoker::AICastingSpell::AICastingSpell(EntityEvoker *entityEvokerIn)
    : AICastingApell(entityEvokerIn), entityEvoker(entityEvokerIn)
{
}

void EntityEvoker::AICastingSpell::updateTask()
{
    if (entityEvoker->getAttackTarget() != nullptr)
    {
        entityEvoker->getLookHelper().setLookPositionWithEntity(entityEvoker->getAttackTarget(),
                                                                entityEvoker->getHorizontalFaceSpeed(),
                                                                entityEvoker->getVerticalFaceSpeed());
    }
    else if (entityEvoker->getWololoTarget() != nullptr)
    {
        entityEvoker->getLookHelper().setLookPositionWithEntity(entityEvoker->getWololoTarget(),
                                                                entityEvoker->getHorizontalFaceSpeed(),
                                                                entityEvoker->getVerticalFaceSpeed());
    }
}

int32_t EntityEvoker::AIAttackSpell::getCastingTime()
{
    return 40;
}

int32_t EntityEvoker::AIAttackSpell::getCastingInterval()
{
    return 100;
}

void EntityEvoker::AIAttackSpell::castSpell()
{
    auto entitylivingbase = entityEvoker->getAttackTarget();
    double d0             = MathHelper::min(entitylivingbase->posY, entityEvoker->posY);
    double d1             = MathHelper::max(entitylivingbase->posY, entityEvoker->posY) + 1.0;
    float f =
        MathHelper::atan2(entitylivingbase->posZ - entityEvoker->posZ, entitylivingbase->posX - entityEvoker->posX);
    int k;
    if (entityEvoker->getDistanceSq(entitylivingbase) < 9.0)
    {
        float f2;
        for (k = 0; k < 5; ++k)
        {
            f2 = f + (float)k * 3.1415927F * 0.4F;
            spawnFangs(entityEvoker->posX + (double)MathHelper::cos(f2) * 1.5,
                       entityEvoker->posZ + (double)MathHelper::sin(f2) * 1.5, d0, d1, f2, 0);
        }

        for (k = 0; k < 8; ++k)
        {
            f2 = f + (float)k * 3.1415927F * 2.0F / 8.0F + 1.2566371F;
            spawnFangs(entityEvoker->posX + (double)MathHelper::cos(f2) * 2.5,
                       entityEvoker->posZ + (double)MathHelper::sin(f2) * 2.5, d0, d1, f2, 3);
        }
    }
    else
    {
        for (k = 0; k < 16; ++k)
        {
            double d2 = 1.25 * (double)(k + 1);
            auto j    = 1 * k;
            spawnFangs(entityEvoker->posX + (double)MathHelper::cos(f) * d2,
                       entityEvoker->posZ + (double)MathHelper::sin(f) * d2, d0, d1, f, j);
        }
    }
}

std::optional<SoundEvent> EntityEvoker::AIAttackSpell::getSpellPrepareSound()
{
    return SoundEvents::EVOCATION_ILLAGER_PREPARE_ATTACK;
}

EntitySpellcasterIllager::SpellType EntityEvoker::AIAttackSpell::getSpellType()
{
    return EntitySpellcasterIllager::SpellType::FANGS;
}

void EntityEvoker::AIAttackSpell::spawnFangs(double p_190876_1_, double p_190876_3_, double p_190876_5_,
                                             double p_190876_7_, float p_190876_9_, int p_190876_10_)
{
    BlockPos blockpos = BlockPos(p_190876_1_, p_190876_7_, p_190876_3_);
    bool flag         = false;
    double d0         = 0.0;

    do
    {
        if (!entityEvoker->world->isBlockNormalCube(blockpos, true) &&
            entityEvoker->world->isBlockNormalCube(blockpos.down(), true))
        {
            if (!entityEvoker->world->isAirBlock(blockpos))
            {
                IBlockState *iblockstate = entityEvoker->world->getBlockState(blockpos);
                auto axisalignedbb       = iblockstate->getCollisionBoundingBox(entityEvoker->world, blockpos);
                if (axisalignedbb.has_value())
                {
                    d0 = axisalignedbb->getmaxY();
                }
            }

            flag = true;
            break;
        }

        blockpos = blockpos.down();
    } while (blockpos.gety() >= MathHelper::floor(p_190876_5_) - 1);

    if (flag)
    {
        EntityEvokerFangs *entityevokerfangs =
            new EntityEvokerFangs(entityEvoker->world, p_190876_1_, (double)blockpos.gety() + d0, p_190876_3_,
                                  p_190876_9_, p_190876_10_, EntityEvoker.this);
        entityEvoker->world->spawnEntity(entityevokerfangs);
    }
}

void EntityEvoker::initEntityAI()
{
    EntitySpellcasterIllager::initEntityAI();
    tasks.addTask(0, new EntityAISwimming(this));
    tasks.addTask(1, new EntityEvoker::AICastingSpell(this));
    tasks.addTask(2, new EntityAIAvoidEntity(this, EntityPlayer.class, 8.0F, 0.6, 1.0));
    tasks.addTask(4, new EntityEvoker::AISummonSpell(this));
    tasks.addTask(5, new EntityEvoker::AIAttackSpell(this));
    tasks.addTask(6, new EntityEvoker::AIWololoSpell(this));
    tasks.addTask(8, new EntityAIWander(this, 0.6));
    tasks.addTask(9, new EntityAIWatchClosest(this, EntityPlayer.class, 3.0F, 1.0F));
    tasks.addTask(10, new EntityAIWatchClosest(this, EntityLiving.class, 8.0F));
    targetTasks.addTask(1, new EntityAIHurtByTarget(this, true, {EntityEvoker.class}));
    targetTasks.addTask(
        2, (new EntityAINearestAttackableTarget(this, EntityPlayer.class, true)).setUnseenMemoryTicks(300));
    targetTasks.addTask(
        3, (new EntityAINearestAttackableTarget(this, EntityVillager.class, false)).setUnseenMemoryTicks(300));
    targetTasks.addTask(3, new EntityAINearestAttackableTarget(this, EntityIronGolem.class, false));
}

void EntityEvoker::applyEntityAttributes()
{
    EntitySpellcasterIllager::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.5);
    getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE.get())->setBaseValue(12.0);
    getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(24.0);
}

void EntityEvoker::entityInit()
{
    EntitySpellcasterIllager::entityInit();
}

std::optional<ResourceLocation> EntityEvoker::getLootTable()
{
    return LootTableList::ENTITIES_EVOCATION_ILLAGER;
}

void EntityEvoker::updateAITasks()
{
    EntitySpellcasterIllager::updateAITasks();
}

SoundEvent EntityEvoker::getAmbientSound()
{
    return SoundEvents::ENTITY_EVOCATION_ILLAGER_AMBIENT;
}

SoundEvent EntityEvoker::getDeathSound()
{
    return SoundEvents::EVOCATION_ILLAGER_DEATH;
}

SoundEvent EntityEvoker::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    return SoundEvents::ENTITY_EVOCATION_ILLAGER_HURT;
}

SoundEvent EntityEvoker::getSpellSound()
{
    return SoundEvents::EVOCATION_ILLAGER_CAST_SPELL;
}

void EntityEvoker::setWololoTarget(EntitySheep *wololoTargetIn)
{
    wololoTarget = wololoTargetIn;
}

EntitySheep *EntityEvoker::getWololoTarget() const
{
    return wololoTarget;
}
