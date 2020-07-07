#include "EntityAIFindEntityNearest.h"

#include "EntityAITarget.h"
#include "Util.h"
#include "../../../../../spdlog/include/spdlog/spdlog-inl.h"
#include "attributes/IAttributeInstance.h"

std::shared_ptr<spdlog::logger> EntityAIFindEntityNearest::LOGGER = spdlog::get("Minecraft")->clone("EntityAIFindEntityNearest");

EntityAIFindEntityNearest::EntityAIFindEntityNearest(EntityLiving *mobIn, std::type_index p_i45884_2_)
    :mob(mobIn),classToCheck(p_i45884_2_)
{
    if (Util::instanceof<EntityCreature>(mobIn)) 
    {
        LOGGER->warn("Use NearestAttackableTargetGoal.class for PathfinerMob mobs!");
    }

    predicate = [&](EntityLivingBase* p_apply_1_)->bool
    {
        double d0 = getFollowRange();
        if (p_apply_1_->isSneaking()) 
        {
            d0 *= 0.800000011920929;
        }

        if (p_apply_1_->isInvisible()) 
        {
            return false;
        }
        else 
        {
            return p_apply_1_->getDistance(mob) > d0 ? false : EntityAITarget::isSuitableTarget(mob, p_apply_1_, false, true);
        }
    };
    sorter = EntityAINearestAttackableTarget::Sorter(mobIn);
}

bool EntityAIFindEntityNearest::shouldExecute()
{
    double d0 = getFollowRange();
    auto list = mob->world.getEntitiesWithinAABB(classToCheck, mob->getEntityBoundingBox().grow(d0, 4.0, d0), predicate);
    Collections.sort(list, sorter);
    if (list.isEmpty()) 
    {
        return false;
    }
    else 
    {
        target = (EntityLivingBase)list.get(0);
        return true;
    }
}

bool EntityAIFindEntityNearest::shouldContinueExecuting()
{
    EntityLivingBase* entitylivingbase = mob->getAttackTarget();
    if (entitylivingbase == nullptr) 
    {
        return false;
    }
    else if (!entitylivingbase->isEntityAlive()) 
    {
        return false;
    }
    else 
    {
        double d0 = getFollowRange();
        if (mob->getDistanceSq(entitylivingbase) > d0 * d0) 
        {
            return false;
        }
        else 
        {
            return !(Util::instanceof<EntityPlayerMP>(entitylivingbase)) || !((EntityPlayerMP*)entitylivingbase).interactionManager.isCreative();
        }
    }
}

void EntityAIFindEntityNearest::startExecuting()
{
    mob->setAttackTarget(target);
    EntityAIBase::startExecuting();
}

void EntityAIFindEntityNearest::resetTask()
{
    mob->setAttackTarget(nullptr);
    EntityAIBase::startExecuting();
}

double EntityAIFindEntityNearest::getFollowRange()
{
    IAttributeInstance* iattributeinstance = mob->getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE);
    return iattributeinstance == nullptr ? 16.0 : iattributeinstance.getAttributeValue();
}
