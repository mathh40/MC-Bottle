#include "MapGenEndCity.h"

#include "Rotation.h"
#include "chunk/ChunkPrimer.h"

MapGenEndCity::MapGenEndCity(ChunkGeneratorEnd endProviderIn) : endProvider(endProviderIn)
{
}

std::string MapGenEndCity::getStructureName()
{
    return "EndCity";
}

BlockPos MapGenEndCity::getNearestStructurePos(World *worldIn, BlockPos pos, bool findUnexplored)
{
    world = worldIn;
    return findNearestStructurePosBySpacing(worldIn, this, pos, 20, 11, 10387313, true, 100, findUnexplored);
}

void MapGenEndCity::Start::create(World *worldIn, ChunkGeneratorEnd chunkProvider, pcg32 &rnd, int32_t chunkX,
                                  int32_t chunkZ)
{
    pcg32 random((long)(chunkX + chunkZ * 10387313));
    Rotation rotation = Rotation.values()[random(Rotation.values().length)];
    auto i            = MapGenEndCity::getYPosForStructure(chunkX, chunkZ, chunkProvider);
    if (i < 60)
    {
        isSizeable = false;
    }
    else
    {
        BlockPos blockpos(chunkX * 16 + 8, i, chunkZ * 16 + 8);
        StructureEndCityPieces.startHouseTower(worldIn->getSaveHandler().getStructureTemplateManager(), blockpos,
                                               rotation, components, rnd);
        updateBoundingBox();
        isSizeable = true;
    }
}

MapGenEndCity::Start::Start(World *worldIn, ChunkGeneratorEnd chunkProvider, pcg32 &random, int32_t chunkX,
                            int32_t chunkZ)
    : StructureStart(chunkX, chunkZ)
{
    create(worldIn, chunkProvider, random, chunkX, chunkZ);
}

bool MapGenEndCity::Start::isSizeableStructure()
{
    return isSizeable;
}

bool MapGenEndCity::canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ)
{
    auto i = chunkX;
    auto j = chunkZ;
    if (chunkX < 0)
    {
        chunkX -= 19;
    }

    if (chunkZ < 0)
    {
        chunkZ -= 19;
    }

    auto k      = chunkX / 20;
    auto l      = chunkZ / 20;
    auto random = world->setRandomSeed(k, l, 10387313);
    k *= 20;
    l *= 20;
    k += (random(9) + random(9)) / 2;
    l += (random(9) + random(9)) / 2;
    if (i == k && j == l && endProvider.isIslandChunk(i, j))
    {
        int i1 = getYPosForStructure(i, j, endProvider);
        return i1 >= 60;
    }
    else
    {
        return false;
    }
}

StructureStart MapGenEndCity::getStructureStart(int32_t chunkX, int32_t chunkZ)
{
    return Start(world, endProvider, rand, chunkX, chunkZ);
}

int32_t MapGenEndCity::getYPosForStructure(int32_t chunkX, int32_t chunkY, ChunkGeneratorEnd generatorIn)
{
    pcg32 random((long)(chunkX + chunkY * 10387313));
    Rotation rotation = Rotation.values()[random(Rotation.values().length)];
    ChunkPrimer chunkprimer;
    generatorIn.setBlocksInChunk(chunkX, chunkY, chunkprimer);
    auto i = 5;
    auto j = 5;
    if (rotation == Rotation::CLOCKWISE_90)
    {
        i = -5;
    }
    else if (rotation == Rotation::CLOCKWISE_180)
    {
        i = -5;
        j = -5;
    }
    else if (rotation == Rotation::COUNTERCLOCKWISE_90)
    {
        j = -5;
    }

    auto k  = chunkprimer.findGroundBlockIdx(7, 7);
    auto l  = chunkprimer.findGroundBlockIdx(7, 7 + j);
    auto i1 = chunkprimer.findGroundBlockIdx(7 + i, 7);
    auto j1 = chunkprimer.findGroundBlockIdx(7 + i, 7 + j);
    auto k1 = MathHelper::min(MathHelper::min(k, l), MathHelper::min(i1, j1));
    return k1;
}
