#include "EntityAIMoveIndoors.h"

#include "../../village/Village.h"
#include "RandomPositionGenerator.h"

EntityAIMoveIndoors::EntityAIMoveIndoors(EntityCreature *entityIn) : entity(entityIn)
{
    setMutexBits(1);
}

bool EntityAIMoveIndoors::shouldExecute()
{
    BlockPos blockpos = BlockPos(entity);
    if ((!entity->world.isDaytime() || entity->world.isRaining() && !entity->world.getBiome(blockpos).canRain()) &&
        entity->world.provider.hasSkyLight())
    {
        if (entity->getRNG().nextInt(50) != 0)
        {
            return false;
        }
        else if (insidePosX != -1 && entity->getDistanceSq((double)insidePosX, entity->posY, (double)insidePosZ) < 4.0)
        {
            return false;
        }
        else
        {
            Village village = entity->world.getVillageCollection().getNearestVillage(blockpos, 14);
            if (village == nullptr)
            {
                return false;
            }
            else
            {
                doorInfo = village.getDoorInfo(blockpos);
                return doorInfo != nullptr;
            }
        }
    }
    else
    {
        return false;
    }
}

bool EntityAIMoveIndoors::shouldContinueExecuting()
{
    return !entity->getNavigator().noPath();
}

void EntityAIMoveIndoors::startExecuting()
{
    insidePosX        = -1;
    BlockPos blockpos = doorInfo.getInsideBlockPos();
    int32_t i         = blockpos.getx();
    int32_t j         = blockpos.gety();
    int32_t k         = blockpos.getz();
    if (entity->getDistanceSq(blockpos) > 256.0)
    {
        auto vec3d = RandomPositionGenerator::findRandomTargetBlockTowards(entity, 14, 3, Vec3d(i + 0.5, j, k + 0.5));
        if (vec3d.has_value())
        {
            entity->getNavigator().tryMoveToXYZ(vec3d->getx(), vec3d->gety(), vec3d->getz(), 1.0);
        }
    }
    else
    {
        entity->getNavigator().tryMoveToXYZ(i + 0.5, j, k + 0.5, 1.0);
    }
}

void EntityAIMoveIndoors::resetTask()
{
    insidePosX = doorInfo.getInsideBlockPos().getx();
    insidePosZ = doorInfo.getInsideBlockPos().get();
    doorInfo   = nullptr;
}
