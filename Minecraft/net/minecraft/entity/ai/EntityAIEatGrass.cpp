#include "EntityAIEatGrass.h"

#include "Block.h"
#include "math/MathHelper.h"
#include "state/IBlockState.h"

auto IS_TALL_GRASS = [](IBlockState *state) -> bool {
    BlockStateMatcher.forBlock(Blocks::TALLGRASS)
        .where(BlockTallGrass::TYPE, Predicates.equalTo(BlockTallGrass::EnumType.GRASS));
    return true;
};

EntityAIEatGrass::EntityAIEatGrass(EntityLiving *grassEaterEntityIn)
    : grassEaterEntity(grassEaterEntityIn), entityWorld(grassEaterEntityIn->world)
{
    setMutexBits(7);
}

bool EntityAIEatGrass::shouldExecute()
{
    if (grassEaterEntity->getRNG()(grassEaterEntity->isChild() ? 50 : 1000) != 0)
    {
        return false;
    }
    else
    {
        BlockPos blockpos = BlockPos(grassEaterEntity->posX, grassEaterEntity->posY, grassEaterEntity->posZ);
        if (IS_TALL_GRASS(entityWorld->getBlockState(blockpos)))
        {
            return true;
        }
        else
        {
            return entityWorld->getBlockState(blockpos.down()).getBlock() == Blocks::GRASS;
        }
    }
}

void EntityAIEatGrass::startExecuting()
{
    eatingGrassTimer = 40;
    entityWorld->setEntityState(grassEaterEntity, 10);
    grassEaterEntity->getNavigator().clearPath();
}

void EntityAIEatGrass::resetTask()
{
    eatingGrassTimer = 0;
}

bool EntityAIEatGrass::shouldContinueExecuting()
{
    return eatingGrassTimer > 0;
}

int32_t EntityAIEatGrass::getEatingGrassTimer() const
{
    return eatingGrassTimer;
}

void EntityAIEatGrass::updateTask()
{
    eatingGrassTimer = MathHelper::max(0, eatingGrassTimer - 1);
    if (eatingGrassTimer == 4)
    {
        BlockPos blockpos = BlockPos(grassEaterEntity->posX, grassEaterEntity->posY, grassEaterEntity->posZ);
        if (IS_TALL_GRASS(entityWorld->getBlockState(blockpos)))
        {
            if (entityWorld->getGameRules().getBoolean("mobGriefing"))
            {
                entityWorld->destroyBlock(blockpos, false);
            }

            grassEaterEntity->eatGrassBonus();
        }
        else
        {
            BlockPos blockpos1 = blockpos.down();
            if (entityWorld->getBlockState(blockpos1).getBlock() == Blocks::GRASS)
            {
                if (entityWorld->getGameRules().getBoolean("mobGriefing"))
                {
                    entityWorld->playEvent(2001, blockpos1, Block::getIdFromBlock(Blocks::GRASS));
                    entityWorld->setBlockState(blockpos1, Blocks::DIRT.getDefaultState(), 2);
                }

                grassEaterEntity->eatGrassBonus();
            }
        }
    }
}
