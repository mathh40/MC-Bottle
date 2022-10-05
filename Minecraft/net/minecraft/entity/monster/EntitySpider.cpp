#include "EntitySpider.h"

#include "../../pathfinding/PathNavigateClimber.h"
#include "../IEntityLivingData.h"
#include "../SharedMonsterAttributes.h"
#include "../ai/EntityAIAttackMelee.h"
#include "../ai/EntityAIFollowGolem.h"
#include "../ai/EntityAIHurtByTarget.h"
#include "../ai/EntityAILeapAtTarget.h"
#include "../ai/EntityAILookIdle.h"
#include "../ai/EntityAINearestAttackableTarget.h"
#include "../ai/EntityAISkeletonRiders.h"
#include "../ai/EntityAISwimming.h"
#include "../ai/EntityAIWanderAvoidWater.h"
#include "../ai/EntityAIWatchClosest.h"
#include "../ai/attributes/IAttributeInstance.h"
#include "datafix/DataFixer.h"

void EntitySpider::GroupData::setRandomEffect(pcg32 &rand)
{
    int i = rand(5);
    if (i <= 1)
    {
        effect = MobEffects::SPEED;
    }
    else if (i <= 2)
    {
        effect = MobEffects::STRENGTH;
    }
    else if (i <= 3)
    {
        effect = MobEffects::REGENERATION;
    }
    else if (i <= 4)
    {
        effect = MobEffects::INVISIBILITY;
    }
}

class AISpiderTarget : public EntityAINearestAttackableTarget
{
  public:
    AISpiderTarget(EntitySpider *spider, std::type_index classTarget)
        : EntityAINearestAttackableTarget(spider, classTarget, true)
    {
    }

    bool shouldExecute() override
    {
        float f = taskOwner->getBrightness();
        return f >= 0.5F ? false : EntityAINearestAttackableTarget::shouldExecute();
    }
};

class AISpiderAttack : public EntityAIAttackMelee
{
  public:
    AISpiderAttack(EntitySpider *spider) : EntityAIAttackMelee(spider, 1.0, true)
    {
    }
    bool shouldContinueExecuting() override
    {
        float f = attacker->getBrightness();
        if (f >= 0.5F && attacker->getRNG()(100) == 0)
        {
            attacker->setAttackTarget(nullptr);
            return false;
        }
        else
        {
            return EntityAIAttackMelee::shouldContinueExecuting();
        }
    }

  protected:
    double getAttackReachSqr(EntityLivingBase *attackTarget)
    {
        return (double)(4.0F + attackTarget->width);
    }
};

DataParameter EntitySpider::CLIMBING = EntityDataManager.createKey(EntitySpider.class, DataSerializers.BYTE);

EntitySpider::EntitySpider(World *worldIn) : EntityMob(worldIn)
{
    setSize(1.4F, 0.9F);
}

void EntitySpider::registerFixesSpider(DataFixer fixer)
{
    EntityLiving::registerFixesMob(fixer, EntitySpider.class);
}

double EntitySpider::getMountedYOffset() const
{
    return height * 0.5F;
}

void EntitySpider::onUpdate()
{
    EntityMob::onUpdate();
    if (!world->isRemote)
    {
        setBesideClimbableBlock(collidedHorizontally);
    }
}

bool EntitySpider::isOnLadder()
{
    return isBesideClimbableBlock();
}

void EntitySpider::setInWeb()
{
}

EnumCreatureAttribute EntitySpider::getCreatureAttribute()
{
    return EnumCreatureAttribute::ARTHROPOD;
}

bool EntitySpider::isPotionApplicable(PotionEffect potioneffectIn)
{
    return potioneffectIn.getPotion() == MobEffects::POISON ? false : EntityMob::isPotionApplicable(potioneffectIn);
}

bool EntitySpider::isBesideClimbableBlock()
{
    return (dataManager.get(CLIMBING) & 1) != 0;
}

void EntitySpider::setBesideClimbableBlock(bool climbing)
{
    std::byte b0 = dataManager.get(CLIMBING);
    if (climbing)
    {
        b0 = (b0 | 1);
    }
    else
    {
        b0 &= -2;
    }

    dataManager.set(CLIMBING, b0);
}

IEntityLivingData *EntitySpider::onInitialSpawn(DifficultyInstance difficulty, IEntityLivingData *livingdata)
{
    IEntityLivingData *livingdata = EntityMob::onInitialSpawn(difficulty, livingdata);
    if (world->rand(100) == 0)
    {
        EntitySkeleton *entityskeleton = new EntitySkeleton(world);
        entityskeleton->setLocationAndAngles(posX, posY, posZ, rotationYaw, 0.0F);
        entityskeleton->onInitialSpawn(difficulty, nullptr);
        world->spawnEntity(entityskeleton);
        entityskeleton->startRiding(this);
    }

    if (livingdata == nullptr)
    {
        livingdata = new EntitySpider::GroupData();
        if (world->getDifficulty() == EnumDifficulty::HARD &&
            MathHelper::nextFloat(world->rand) < 0.1F * difficulty.getClampedAdditionalDifficulty())
        {
            ((EntitySpider::GroupData)livingdata).setRandomEffect(world->rand);
        }
    }

    if (Util:: instanceof <EntitySpider::GroupData>(livingdata))
    {
        Potion potion = ((EntitySpider::GroupData)livingdata).effect;
        if (potion != nullptr)
        {
            addPotionEffect(new PotionEffect(potion, std::numeric_limits<int>::max()));
        }
    }

    return (IEntityLivingData *)livingdata;
}

float EntitySpider::getEyeHeight() const
{
    return 0.65F;
}

void EntitySpider::initEntityAI()
{
    tasks.addTask(1, new EntityAISwimming(this));
    tasks.addTask(3, new EntityAILeapAtTarget(this, 0.4F));
    tasks.addTask(4, new AISpiderAttack(this));
    tasks.addTask(5, new EntityAIWanderAvoidWater(this, 0.8));
    tasks.addTask(6, new EntityAIWatchClosest(this, typeid(EntityPlayer), 8.0F));
    tasks.addTask(6, new EntityAILookIdle(this));
    targetTasks.addTask(1, new EntityAIHurtByTarget(this, false, {}));
    targetTasks.addTask(2, new AISpiderTarget(this, typeid(EntityPlayer)));
    targetTasks.addTask(3, new AISpiderTarget(this, typeid(EntityIronGolem)));
}

PathNavigate *EntitySpider::createNavigator(World *worldIn)
{
    return new PathNavigateClimber(this, worldIn);
}

void EntitySpider::entityInit()
{
    EntityMob::entityInit();
    dataManager.registe(CLIMBING, 0);
}

void EntitySpider::applyEntityAttributes()
{
    EntityMob::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(16.0);
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.30000001192092896);
}

SoundEvent EntitySpider::getAmbientSound()
{
    return SoundEvents::ENTITY_SPIDER_AMBIENT;
}

SoundEvent EntitySpider::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    return SoundEvents::ENTITY_SPIDER_HURT;
}

SoundEvent EntitySpider::getDeathSound()
{
    return SoundEvents::ENTITY_SPIDER_DEATH;
}

void EntitySpider::playStepSound(BlockPos pos, Block *blockIn)
{
    playSound(SoundEvents::ENTITY_SPIDER_STEP, 0.15F, 1.0F);
}

std::optional<ResourceLocation> EntitySpider::getLootTable()
{
    return LootTableList::ENTITIES_SPIDER;
}
