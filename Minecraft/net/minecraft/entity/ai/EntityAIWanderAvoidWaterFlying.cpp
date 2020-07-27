#include "EntityAIWanderAvoidWaterFlying.h"


#include "Block.h"
#include "RandomPositionGenerator.h"
#include "Util.h"
#include "math/BlockPos.h"
#include "math/MathHelper.h"

EntityAIWanderAvoidWaterFlying::EntityAIWanderAvoidWaterFlying(EntityCreature *p_i47413_1_, double p_i47413_2_)
    :EntityAIWanderAvoidWater(p_i47413_1_, p_i47413_2_)
{

}

std::optional<Vec3d> EntityAIWanderAvoidWaterFlying::getPosition()
{
    std::optional<Vec3d> vec3d;
    if (entity->isInWater() || entity->isOverWater()) 
    {
        vec3d = RandomPositionGenerator::getLandPos(entity, 15, 15);
    }

    if (entity->getRNG().nextFloat() >= probability) {
        vec3d = getTreePos();
    }

    return vec3d ? vec3d : EntityAIWanderAvoidWater::getPosition();
}

std::optional<Vec3d> EntityAIWanderAvoidWaterFlying::getTreePos()
{
    BlockPos blockpos(entity);
    MutableBlockPos blockpos$mutableblockpos;
    MutableBlockPos blockpos$mutableblockpos1;
    auto iterable = getAllInBoxMutable(MathHelper::floor(entity->posX - 3.0), MathHelper::floor(entity->posY - 6.0), MathHelper::floor(entity->posZ - 3.0), MathHelper::floor(entity->posX + 3.0), MathHelper::floor(entity->posY + 6.0), MathHelper::floor(entity->posZ + 3.0));
    const auto iterator = iterable.begin();

    BlockPos blockpos1;
    bool flag;
    do 
    {
        do 
        {
            if (iterator == iterable.end()) 
            {
                return std::nullopt;
            }

            blockpos1 = *iterator;
        }
        while(blockpos == blockpos1);

        Block* block = entity->world.getBlockState(blockpos$mutableblockpos1.setPos((Vec3i)blockpos1).move(EnumFacing::DOWN)).getBlock();
        flag = Util::instanceof<BlockLeaves>(block) || block == Blocks::LOG || block == Blocks::LOG2;
    } while(!flag || !entity->world.isAirBlock(blockpos1) || !entity->world.isAirBlock(blockpos$mutableblockpos.setPos((Vec3i)blockpos1).move(EnumFacing::UP)));

    return Vec3d((double)blockpos1.getx(), (double)blockpos1.gety(), (double)blockpos1.getz());
}
