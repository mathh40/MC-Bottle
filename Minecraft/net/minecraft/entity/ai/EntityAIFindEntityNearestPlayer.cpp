#include "EntityAIFindEntityNearestPlayer.h"

#include "../../../../../spdlog/include/spdlog/spdlog-inl.h"
#include "../../scoreboard/Team.h"
#include "EntityAITarget.h"
#include "Util.h"
#include "attributes/IAttributeInstance.h"

std::shared_ptr<spdlog::logger> EntityAIFindEntityNearestPlayer::LOGGER =
    spdlog::get("Minecraft")->clone("EntityAIFindEntityNearestPlayer");

EntityAIFindEntityNearestPlayer::EntityAIFindEntityNearestPlayer(EntityLiving *entityLivingIn)
    : entityLiving(entityLivingIn)
{
    if (Util:: instanceof <EntityCreature>(entityLivingIn))
    {
        LOGGER->warn("Use NearestAttackableTargetGoal.class for PathfinerMob mobs!");
    }

    predicate = [&](Entity *p_apply_1_) -> bool {
        if (!Util:: instanceof <EntityPlayer>(p_apply_1_))
        {
            return false;
        }
        else if (((EntityPlayer *)p_apply_1_)->capabilities.disableDamage)
        {
            return false;
        }
        else
        {
            double d0 = maxTargetRange();
            if (p_apply_1_->isSneaking())
            {
                d0 *= 0.800000011920929;
            }

            if (p_apply_1_->isInvisible())
            {
                float f = ((EntityPlayer *)p_apply_1_)->getArmorVisibility();
                if (f < 0.1F)
                {
                    f = 0.1F;
                }

                d0 *= (0.7F * f);
            }

            return p_apply_1_->getDistance(entityLiving) > d0
                       ? false
                       : EntityAITarget::isSuitableTarget(entityLiving, (EntityLivingBase *)p_apply_1_, false, true);
        }
    };
    sorter = EntityAINearestAttackableTarget::Sorter(entityLivingIn);
}

bool EntityAIFindEntityNearestPlayer::shouldExecute()
{
    double d0                            = maxTargetRange();
    std::vector<EntityLivingBase *> list = entityLiving->world.getEntitiesWithinAABB(
        EntityPlayer.class, entityLiving->getEntityBoundingBox().grow(d0, 4.0, d0), predicate);
    Collections.sort(list, sorter);
    if (list.empty())
    {
        return false;
    }
    else
    {
        entityTarget = list[0];
        return true;
    }
}

bool EntityAIFindEntityNearestPlayer::shouldContinueExecuting()
{
    EntityLivingBase *entitylivingbase = entityLiving->getAttackTarget();
    if (entitylivingbase == nullptr)
    {
        return false;
    }
    else if (!entitylivingbase->isEntityAlive())
    {
        return false;
    }
    else if (Util:: instanceof <EntityPlayer>(entitylivingbase) &&
                                   ((EntityPlayer *)entitylivingbase)->capabilities.disableDamage)
    {
        return false;
    }
    else
    {
        Team *team  = entityLiving->getTeam();
        Team *team1 = entitylivingbase->getTeam();
        if (team != nullptr && team1 == team)
        {
            return false;
        }
        else
        {
            double d0 = maxTargetRange();
            if (entityLiving->getDistanceSq(entitylivingbase) > d0 * d0)
            {
                return false;
            }
            else
            {
                return !(Util:: instanceof <EntityPlayerMP>(entitylivingbase)) ||
                       !((EntityPlayerMP *)entitylivingbase)->interactionManager.isCreative();
            }
        }
    }
}

void EntityAIFindEntityNearestPlayer::startExecuting()
{
    entityLiving->setAttackTarget(entityTarget);
    EntityAIBase::startExecuting();
}

void EntityAIFindEntityNearestPlayer::resetTask()
{
    entityLiving->setAttackTarget(nullptr);
    EntityAIBase::startExecuting();
}

double EntityAIFindEntityNearestPlayer::maxTargetRange()
{
    IAttributeInstance *iattributeinstance = entityLiving->getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE);
    return iattributeinstance == nullptr ? 16.0 : iattributeinstance.getAttributeValue();
}
