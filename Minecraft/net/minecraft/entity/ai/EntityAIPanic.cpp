#include "EntityAIPanic.h"


#include "RandomPositionGenerator.h"
#include "math/BlockPos.h"
#include "state/IBlockState.h"

EntityAIPanic::EntityAIPanic(EntityCreature *creature, double speedIn)
    :creature(creature),speed(speedIn)
{
    setMutexBits(1);
}

bool EntityAIPanic::shouldExecute()
{
    if (creature->getRevengeTarget() == nullptr && !creature->isBurning()) 
    {
        return false;
    }
    else 
    {
        if (creature->isBurning()) 
        {
            BlockPos blockpos = getRandPos(creature->world, creature, 5, 4);
            if (blockpos != nullptr) 
            {
                randPosX = (double)blockpos.getx();
                randPosY = (double)blockpos.gety();
                randPosZ = (double)blockpos.getz();
                return true;
            }
        }

        return findRandomPosition();
    }
}

void EntityAIPanic::startExecuting()
{
    creature->getNavigator().tryMoveToXYZ(randPosX, randPosY, randPosZ, speed);
}

bool EntityAIPanic::shouldContinueExecuting()
{
    return !creature->getNavigator().noPath();
}

bool EntityAIPanic::findRandomPosition()
{
    auto vec3d = RandomPositionGenerator::findRandomTarget(creature, 5, 4);
    if (!vec3d) 
    {
        return false;
    }
    else 
    {
        randPosX = vec3d->getx();
        randPosY = vec3d->gety();
        randPosZ = vec3d->getz();
        return true;
    }
}

std::optional<BlockPos> EntityAIPanic::getRandPos(World *worldIn, Entity *entityIn, int32_t horizontalRange,
    int32_t verticalRange)
{
    BlockPos blockpos(entityIn);
    auto i = blockpos.getx();
    auto j = blockpos.gety();
    auto k = blockpos.getz();
    float f = horizontalRange * horizontalRange * verticalRange * 2;
    BlockPos blockpos1;
    BlockPos blockpos$mutableblockpos;

    for(auto l = i - horizontalRange; l <= i + horizontalRange; ++l) 
    {
        for(auto i1 = j - verticalRange; i1 <= j + verticalRange; ++i1) 
        {
            for(auto j1 = k - horizontalRange; j1 <= k + horizontalRange; ++j1) 
            {
                blockpos$mutableblockpos.setPos(l, i1, j1);
                IBlockState* iblockstate = worldIn->getBlockState(blockpos$mutableblockpos);
                if (iblockstate->getMaterial() == Material::WATER) 
                {
                    float f1 = (l - i) * (l - i) + (i1 - j) * (i1 - j) + (j1 - k) * (j1 - k);
                    if (f1 < f) 
                    {
                        f = f1;
                        blockpos1 = blockpos$mutableblockpos;
                    }
                }
            }
        }
    }

    return blockpos1;
}
