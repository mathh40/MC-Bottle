#include "MapGenStronghold.h"

MapGenStronghold::MapGenStronghold() : structureCoords(128), distance(32.0), spread(3)
{
    for (auto biome : Biome::REGISTRY)
    {
        if (biome.second != nullptr && biome.second->getBaseHeight() > 0.0F)
        {
            allowedBiomes.emplace_back(biome);
        }
    }
}

MapGenStronghold::MapGenStronghold(std::unordered_map<std::string, std::string> p_i2068_1_) : MapGenStronghold()
{
    for (auto entry : p_i2068_1_)
    {
        if ((entry.first) == ("distance"))
        {
            distance = MathHelper::getDouble(entry.second, distance, 1.0);
        }
        else if ((entry.first) == ("count"))
        {
            structureCoords.resize(MathHelper::getInt((entry.second), structureCoords.size(), 1));
        }
        else if ((entry.first) == ("spread"))
        {
            spread = MathHelper::getInt((entry.second), spread, 1);
        }
    }
}

std::string MapGenStronghold::getStructureName()
{
    return "Stronghold";
}

std::optional<BlockPos> MapGenStronghold::getNearestStructurePos(World *worldIn, BlockPos &pos, bool findUnexplored)
{
    if (!ranBiomeCheck)
    {
        generatePositions();
        ranBiomeCheck = true;
    }

    std::optional<BlockPos> blockpos;
    MutableBlockPos blockpos$mutableblockpos = MutableBlockPos(0, 0, 0);
    double d0                                = std::numeric_limits<double>::max();
    auto var8                                = structureCoords;
    auto var9                                = var8.size();

    for (int32_t var10 = 0; var10 < var9; ++var10)
    {
        ChunkPos chunkpos = var8[var10];
        blockpos$mutableblockpos.setPos((chunkpos.getx() << 4) + 8, 32, (chunkpos.getz() << 4) + 8);
        double d1 = blockpos$mutableblockpos.distanceSq(pos);
        if (!blockpos.has_value())
        {
            blockpos = blockpos$mutableblockpos;
            d0       = d1;
        }
        else if (d1 < d0)
        {
            blockpos = blockpos$mutableblockpos;
            d0       = d1;
        }
    }

    return blockpos;
}

MapGenStronghold::Start::Start(World *worldIn, pcg32 &random, int32_t chunkX, int32_t chunkZ)
    : StructureStart(chunkX, chunkZ)
{
    StructureStrongholdPieces.prepareStructurePieces();
    StructureStrongholdPieces.Stairs2 structurestrongholdpieces$stairs2 =
        StructureStrongholdPieces.Stairs2(0, random, (chunkX << 4) + 2, (chunkZ << 4) + 2);
    components.add(structurestrongholdpieces$stairs2);
    structurestrongholdpieces$stairs2.buildComponent(structurestrongholdpieces$stairs2, components, random);
    List list = structurestrongholdpieces$stairs2.pendingChildren;

    while (!list.empty())
    {
        int i                                 = random(list.size());
        StructureComponent structurecomponent = (StructureComponent)list.remove(i);
        structurecomponent.buildComponent(structurestrongholdpieces$stairs2, components, random);
    }

    updateBoundingBox();
    markAvailableHeight(worldIn, random, 10);
}

bool MapGenStronghold::canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ)
{
    if (!ranBiomeCheck)
    {
        generatePositions();
        ranBiomeCheck = true;
    }

    auto var3 = structureCoords;
    auto var4 = var3.size();

    for (auto var5 = 0; var5 < var4; ++var5)
    {
        ChunkPos chunkpos = var3[var5];
        if (chunkX == chunkpos.getx() && chunkZ == chunkpos.getz())
        {
            return true;
        }
    }

    return false;
}

StructureStart MapGenStronghold::getStructureStart(int32_t chunkX, int32_t chunkZ)
{
    Start mapgenstronghold$start;
    for (mapgenstronghold$start = Start(world, rand, chunkX, chunkZ);
         mapgenstronghold$start.getComponents().empty() ||
         ((Stairs2)mapgenstronghold$start.getComponents().get(0)).strongholdPortalRoom == nullptr;
         mapgenstronghold$start = Start(world, rand, chunkX, chunkZ))
    {
    }

    return mapgenstronghold$start;
}

void MapGenStronghold::generatePositions()
{
    initializeStructureData(world);
    int i = 0;

    for (auto structurestart : structureMap)
    {
        if (i < structureCoords.size())
        {
            structureCoords[i++] = ChunkPos(structurestart.second.getChunkPosX(), structurestart.second.getChunkPosZ());
        }
    }
    pcg32 random;
    random.seed(world->getSeed());
    double d1 = MathHelper::nextDouble(random) * 3.141592653589793 * 2.0;
    int32_t j = 0;
    int32_t k = 0;
    int32_t l = structureMap.size();
    if (l < structureCoords.size())
    {
        for (int i1 = 0; i1 < structureCoords.size(); ++i1)
        {
            double d0 =
                4.0 * distance + distance * (double)j * 6.0 + (MathHelper::nextDouble(random) - 0.5) * distance * 2.5;
            int j1 = MathHelper::round(MathHelper::cos(d1) * d0);
            int k1 = MathHelper::round(MathHelper::sin(d1) * d0);
            auto blockpos =
                world->getBiomeProvider().findBiomePosition((j1 << 4) + 8, (k1 << 4) + 8, 112, allowedBiomes, random);
            if (blockpos.has_value())
            {
                j1 = blockpos.value.getx() >> 4;
                k1 = blockpos.value.getz() >> 4;
            }

            if (i1 >= l)
            {
                structureCoords[i1] = ChunkPos(j1, k1);
            }

            d1 += 6.283185307179586 / (double)spread;
            ++k;
            if (k == spread)
            {
                ++j;
                k = 0;
                spread += 2 * spread / (j + 1);
                spread = MathHelper::min<int32_t>(spread, structureCoords.size() - i1);
                d1 += MathHelper::nextDouble(random) * 3.141592653589793 * 2.0;
            }
        }
    }
}
