#include "EntityAIFollowOwner.h"
#include "Util.h"
#include "../../pathfinding/PathNavigateFlying.h"
#include "../../pathfinding/PathNavigateGround.h"

EntityAIFollowOwner::EntityAIFollowOwner(EntityTameable *tameableIn, double followSpeedIn, float minDistIn,
                                         float maxDistIn)
        :tameable(tameableIn),world(tameableIn->world),followSpeed(followSpeedIn),petPathfinder(tameableIn->getNavigator())
        ,minDist(minDistIn),maxDist(maxDistIn)
{
    setMutexBits(3);
    if (!Util::instanceof<PathNavigateGround>(tameableIn->getNavigator()) && ! Util::instanceof<PathNavigateFlying>(tameableIn->getNavigator())) 
    {
        throw std::logic_error("Unsupported mob type for FollowOwnerGoal");
    }
}

bool EntityAIFollowOwner::shouldExecute()
{
    EntityLivingBase* entitylivingbase = tameable->getOwner();
    if (entitylivingbase == nullptr) 
    {
        return false;
    }
    else if (Util::instanceof<EntityPlayer>(entitylivingbase) && ((EntityPlayer*)entitylivingbase)->isSpectator()) 
    {
        return false;
    }
    else if (tameable->isSitting()) 
    {
        return false;
    }
    else if (tameable->getDistanceSq(entitylivingbase) < minDist * minDist) 
    {
        return false;
    }
    else 
    {
        owner = entitylivingbase;
        return true;
    }
}

bool EntityAIFollowOwner::shouldContinueExecuting()
{
    return !petPathfinder->noPath() && tameable->getDistanceSq(owner) > maxDist * maxDist && !tameable->isSitting();
}

void EntityAIFollowOwner::startExecuting()
{
    timeToRecalcPath = 0;
    oldWaterCost = tameable->getPathPriority(PathNodeType::WATER);
    tameable->setPathPriority(PathNodeType::WATER, 0.0F);
}

void EntityAIFollowOwner::resetTask()
{
    owner = nullptr;
    petPathfinder->clearPath();
    tameable->setPathPriority(PathNodeType::WATER, oldWaterCost);
}

void EntityAIFollowOwner::updateTask()
{
    tameable->getLookHelper().setLookPositionWithEntity(owner, 10.0F, tameable->getVerticalFaceSpeed());
    if (!tameable->isSitting() && --timeToRecalcPath <= 0) 
    {
        timeToRecalcPath = 10;
        if (!petPathfinder->tryMoveToEntityLiving(owner, followSpeed) && !tameable->getLeashed() && !tameable->isRiding() && tameable->getDistanceSq(owner) >= 144.0) 
        {
            auto i = MathHelper::floor(owner->posX) - 2;
            auto j = MathHelper::floor(owner->posZ) - 2;
            auto k = MathHelper::floor(owner->getEntityBoundingBox().minY);

            for(auto l = 0; l <= 4; ++l) 
            {
                for(auto i1 = 0; i1 <= 4; ++i1) 
                {
                    if ((l < 1 || i1 < 1 || l > 3 || i1 > 3) && isTeleportFriendlyBlock(i, j, k, l, i1)) 
                    {
                        tameable->setLocationAndAngles(i + l + 0.5F, k, j + i1 + 0.5F, tameable->rotationYaw, tameable->rotationPitch);
                        petPathfinder->clearPath();
                        return;
                    }
                }
            }
        }
    }
}

bool EntityAIFollowOwner::isTeleportFriendlyBlock(int32_t x, int32_t z, int32_t y, int32_t xOffset, int32_t zOffset)
{
    BlockPos blockpos = BlockPos(x + xOffset, y - 1, z + zOffset);
    IBlockState* iblockstate = world->getBlockState(blockpos);
    return iblockstate->getBlockFaceShape(world, blockpos, EnumFacing::DOWN) == BlockFaceShape::SOLID && iblockstate->canEntitySpawn(tameable) && world->isAirBlock(blockpos.up()) && world->isAirBlock(blockpos.up(2));
}
