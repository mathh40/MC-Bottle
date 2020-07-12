#include "EntityAILlamaFollowCaravan.h"

#include <limits>


#include "math/MathHelper.h"
#include "math/Vec3d.h"

EntityAILlamaFollowCaravan::EntityAILlamaFollowCaravan(EntityLlama* llamaIn, double speedModifierIn)
    :llama(llamaIn),speedModifier(speedModifierIn)
{
    setMutexBits(1);
}

bool EntityAILlamaFollowCaravan::shouldExecute()
{
    if (!llama->getLeashed() && !llama->inCaravan()) 
    {
        auto list = llama->world->getEntitiesWithinAABB(llama->getClass(), llama->getEntityBoundingBox().grow(9.0, 4.0, 9.0));
        EntityLlama* entityllama = nullptr;
        double d0 = std::numeric_limits<double>::max();

        double d2;
        for(auto entityllama2 : list)
        {
            if (entityllama2->inCaravan() && !entityllama2->hasCaravanTrail()) 
            {
                d2 = llama->getDistanceSq(entityllama2);
                if (d2 <= d0) 
                {
                    d0 = d2;
                    entityllama = entityllama2;
                }
            }
        }

        if (entityllama == nullptr) 
        {
            for(auto entityllama2 : list)
            {
                if (entityllama2->getLeashed() && !entityllama2->hasCaravanTrail()) 
                {
                    d2 = llama->getDistanceSq(entityllama2);
                    if (d2 <= d0) 
                    {
                        d0 = d2;
                        entityllama = entityllama2;
                    }
                }
            }
        }

        if (entityllama == nullptr) 
        {
            return false;
        }
        else if (d0 < 4.0) 
        {
            return false;
        }
        else if (!entityllama->getLeashed() && !firstIsLeashed(entityllama, 1)) 
        {
            return false;
        }
        else 
        {
            llama->joinCaravan(entityllama);
            return true;
        }
    }
    else 
    {
        return false;
    }
}

bool EntityAILlamaFollowCaravan::shouldContinueExecuting()
{
    if (llama->inCaravan() && llama->getCaravanHead().isEntityAlive() && firstIsLeashed(llama, 0)) 
    {
        double d0 = llama->getDistanceSq(llama->getCaravanHead());
        if (d0 > 676.0) 
        {
            if (speedModifier <= 3.0) 
            {
                speedModifier *= 1.2;
                distCheckCounter = 40;
                return true;
            }

            if (distCheckCounter == 0) 
            {
                return false;
            }
        }

        if (distCheckCounter > 0) 
        {
            --distCheckCounter;
        }

        return true;
    }
    else 
    {
        return false;
    }
}

void EntityAILlamaFollowCaravan::resetTask()
{
    llama->leaveCaravan();
    speedModifier = 2.1;
}

void EntityAILlamaFollowCaravan::updateTask()
{
    if (llama->inCaravan()) 
    {
        EntityLlama* entityllama = llama->getCaravanHead();
        double d0 = (double)llama->getDistance(entityllama);
        float f = 2.0F;
        Vec3d vec3d = (Vec3d(entityllama->posX - llama->posX, entityllama->posY - llama->posY, entityllama->posZ - llama->posZ)).normalize().scale(MathHelper::max(d0 - 2.0, 0.0));
        llama->getNavigator().tryMoveToXYZ(llama->posX + vec3d.getx(), llama->posY + vec3d.gety(), llama->posZ + vec3d.getz(), speedModifier);
    }
}

bool EntityAILlamaFollowCaravan::firstIsLeashed(EntityLlama *p_190858_1_, int32_t p_190858_2_)
{
    if (p_190858_2_ > 8) 
    {
        return false;
    }
    else if (p_190858_1_->inCaravan()) 
    {
        if (p_190858_1_->getCaravanHead().getLeashed()) 
        {
            return true;
        }
        else 
        {
            EntityLlama* entityllama = p_190858_1_->getCaravanHead();
            ++p_190858_2_;
            return firstIsLeashed(entityllama, p_190858_2_);
        }
    }
    else 
    {
        return false;
    }
}
