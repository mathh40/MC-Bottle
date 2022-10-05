#include "EntityAIAttackRanged.h"

#include "Util.h"
#include "biome/Biome.h"
#include "math/MathHelper.h"

#include <stdexcept>

EntityAIAttackRanged::EntityAIAttackRanged(IRangedAttackMob *attacker, double movespeed, int32_t maxAttackTime,
                                           float maxAttackDistanceIn)
    : EntityAIAttackRanged(attacker, movespeed, maxAttackTime, maxAttackTime, maxAttackDistanceIn)
{
}

EntityAIAttackRanged::EntityAIAttackRanged(IRangedAttackMob *attacker, double movespeed, int32_t p_i1650_4_,
                                           int32_t maxAttackTime, float maxAttackDistanceIn)
{
    rangedAttackTime = -1;
    if (!Util:: instanceof <EntityLivingBase>(attacker))
    {
        throw std::logic_error("ArrowAttackGoal requires Mob implements RangedAttackMob");
    }
    else
    {
        rangedAttackEntityHost = attacker;
        entityHost             = attacker;
        entityMoveSpeed        = movespeed;
        attackIntervalMin      = p_i1650_4_;
        maxRangedAttackTime    = maxAttackTime;
        attackRadius           = maxAttackDistanceIn;
        maxAttackDistance      = maxAttackDistanceIn * maxAttackDistanceIn;
        setMutexBits(3);
    }
}

bool EntityAIAttackRanged::shouldExecute()
{
    EntityLivingBase *entitylivingbase = entityHost->getAttackTarget();
    if (entitylivingbase == nullptr)
    {
        return false;
    }
    else
    {
        attackTarget = entitylivingbase;
        return true;
    }
}

bool EntityAIAttackRanged::shouldContinueExecuting()
{
    return shouldExecute() || !entityHost->getNavigator().noPath();
}

void EntityAIAttackRanged::resetTask()
{
    attackTarget     = nullptr;
    seeTime          = 0;
    rangedAttackTime = -1;
}

void EntityAIAttackRanged::updateTask()
{
    double d0 =
        entityHost->getDistanceSq(attackTarget->posX, attackTarget->getEntityBoundingBox().minY, attackTarget->posZ);
    bool flag = entityHost->getEntitySenses().canSee(attackTarget);
    if (flag)
    {
        ++seeTime;
    }
    else
    {
        seeTime = 0;
    }

    if (d0 <= maxAttackDistance && seeTime >= 20)
    {
        entityHost->getNavigator().clearPath();
    }
    else
    {
        entityHost->getNavigator().tryMoveToEntityLiving(attackTarget, entityMoveSpeed);
    }

    entityHost->getLookHelper().setLookPositionWithEntity(attackTarget, 30.0F, 30.0F);
    float f;
    if (--rangedAttackTime == 0)
    {
        if (!flag)
        {
            return;
        }

        f              = MathHelper::sqrt(d0) / attackRadius;
        float lvt_5_1_ = MathHelper::clamp(f, 0.1F, 1.0F);
        rangedAttackEntityHost->attackEntityWithRangedAttack(attackTarget, lvt_5_1_);
        rangedAttackTime = MathHelper::floor(f * (maxRangedAttackTime - attackIntervalMin) + attackIntervalMin);
    }
    else if (rangedAttackTime < 0)
    {
        f                = MathHelper::sqrt(d0) / attackRadius;
        rangedAttackTime = MathHelper::floor(f * (maxRangedAttackTime - attackIntervalMin) + attackIntervalMin);
    }
}
