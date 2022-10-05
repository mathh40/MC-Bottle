#include "ChunkGeneratorFlat.h"

#include "../../../../../spdlog/spdlog/fmt/bundled/format.h"
#include "chunk/Chunk.h"
#include "chunk/ChunkPrimer.h"

ChunkGeneratorFlat::ChunkGeneratorFlat(World *worldIn, int64_t seed, bool generateStructures,
                                       std::string_view flatGeneratorSettings)
    : world(worldIn), random(seed),
      flatWorldGenInfo(FlatGeneratorInfo::createFlatGeneratorFromString(flatGeneratorSettings))
{
    if (generateStructures)
    {
        auto map = flatWorldGenInfo.getWorldFeatures();
        if (map.containsKey("village"))
        {
            auto map1 = map.at("village");
            if (!map1.containsKey("size"))
            {
                map1.put("size", "1");
            }

            structureGenerators.emplace("Village", MapGenVillage(map1));
        }

        if (map.containsKey("biome_1"))
        {
            structureGenerators.emplace("Temple", MapGenScatteredFeature((Map)map.get("biome_1")));
        }

        if (map.containsKey("mineshaft"))
        {
            structureGenerators.emplace("Mineshaft", MapGenMineshaft((Map)map.get("mineshaft")));
        }

        if (map.containsKey("stronghold"))
        {
            structureGenerators.emplace("Stronghold", MapGenStronghold((Map)map.get("stronghold")));
        }

        if (map.containsKey("oceanmonument"))
        {
            structureGenerators.emplace("Monument", StructureOceanMonument((Map)map.get("oceanmonument")));
        }
    }

    if (flatWorldGenInfo.getWorldFeatures().containsKey("lake"))
    {
        waterLakeGenerator = WorldGenLakes(Blocks::WATER);
    }

    if (flatWorldGenInfo.getWorldFeatures().containsKey("lava_lake"))
    {
        lavaLakeGenerator = WorldGenLakes(Blocks::LAVA);
    }

    hasDungeons = flatWorldGenInfo.getWorldFeatures().containsKey("dungeon");
    auto j      = 0;
    auto k      = 0;
    bool flag   = true;

    for (auto flatlayerinfo : flatWorldGenInfo.getFlatLayers())
    {
        for (auto i = flatlayerinfo.getMinY(); i < flatlayerinfo.getMinY() + flatlayerinfo.getLayerCount(); ++i)
        {
            IBlockState *iblockstate = flatlayerinfo.getLayerMaterial();
            if (iblockstate.getBlock() != Blocks.AIR)
            {
                flag              = false;
                cachedBlockIDs[i] = iblockstate;
            }
        }

        if (flatlayerinfo.getLayerMaterial().getBlock() == Blocks.AIR)
        {
            k += flatlayerinfo.getLayerCount();
        }
        else
        {
            j += flatlayerinfo.getLayerCount() + k;
            k = 0;
        }
    }

    worldIn->setSeaLevel(j);
    hasDecoration = flag && flatWorldGenInfo.getBiome() != Biome::getIdForBiome(Biomes::VOID)
                        ? false
                        : flatWorldGenInfo.getWorldFeatures().containsKey("decoration");
}

Chunk ChunkGeneratorFlat::generateChunk(int32_t x, int32_t z)
{
    ChunkPrimer chunkprimer;

    for (auto i = 0; i < cachedBlockIDs.size(); ++i)
    {
        IBlockState *iblockstate = cachedBlockIDs[i];
        if (iblockstate != nullptr)
        {
            for (auto j = 0; j < 16; ++j)
            {
                for (auto k = 0; k < 16; ++k)
                {
                    chunkprimer.setBlockState(j, i, k, iblockstate);
                }
            }
        }
    }

    for (auto mapgenbase : structureGenerators)
    {
        mapgenbase.second().generate(world, x, z, chunkprimer);
    }

    Chunk chunk(world, chunkprimer, x, z);
    auto abiome = world->getBiomeProvider().getBiomes(std::vector<Biome *>(), x * 16, z * 16, 16, 16);
    auto abyte  = chunk.getBiomeArray();

    for (auto k = 0; k < abyte.size(); ++k)
    {
        abyte[k] = Biome::getIdForBiome(abiome[k]);
    }

    chunk.generateSkylightMap();
    return chunk;
}

void ChunkGeneratorFlat::populate(int32_t x, int32_t z)
{
    auto i = x * 16;
    auto j = z * 16;
    BlockPos blockpos(i, 0, j);
    auto biome = world->getBiome(BlockPos(i + 16, 0, j + 16));
    bool flag  = false;
    random.seed(world->getSeed());
    auto k = random(std::numeric_limits<int64_t>::max()) / 2L * 2L + 1L;
    auto l = random(std::numeric_limits<int64_t>::max()) / 2L * 2L + 1L;
    random.seed((long)x * k + (long)z * l ^ world->getSeed());
    ChunkPos chunkpos(x, z);

    for (auto mapgenstructure : structureGenerators)
    {
        bool flag1 = mapgenstructure.generateStructure(world, random, chunkpos);
        if (Util:: instanceof <MapGenVillage>(mapgenstructure))
        {
            flag |= flag1;
        }
    }

    if (!flag && random(4) == 0)
    {
        waterLakeGenerator.generate(world, random, blockpos.add(random(16) + 8, random(256), random(16) + 8));
    }

    if (!flag && random(8) == 0)
    {
        BlockPos blockpos1 = blockpos.add(random(16) + 8, random(random(248) + 8), random(16) + 8);
        if (blockpos1.gety() < world->getSeaLevel() || random(10) == 0)
        {
            lavaLakeGenerator.generate(world, random, blockpos1);
        }
    }

    if (hasDungeons)
    {
        for (auto i1 = 0; i1 < 8; ++i1)
        {
            (new WorldGenDungeons()).generate(world, random, blockpos.add(random(16) + 8, random(256), random(16) + 8));
        }
    }

    if (hasDecoration)
    {
        biome.decorate(world, random, blockpos);
    }
}

bool ChunkGeneratorFlat::generateStructures(Chunk &chunkIn, int32_t x, int32_t z)
{
    return false;
}

std::vector<SpawnListEntry> ChunkGeneratorFlat::getPossibleCreatures(EnumCreatureType creatureType, BlockPos &pos)
{
    auto biome = world->getBiome(pos);
    return biome.getSpawnableList(creatureType);
}

std::optional<BlockPos> ChunkGeneratorFlat::getNearestStructurePos(World *worldIn, std::string_view structureName,
                                                                   BlockPos &position, bool findUnexplored)
{
    auto mapgenstructure = structureGenerators.get(structureName);
    return mapgenstructure != nullptr ? mapgenstructure.getNearestStructurePos(worldIn, position, findUnexplored)
                                      : std::nullopt;
}

bool ChunkGeneratorFlat::isInsideStructure(World *worldIn, std::string_view structureName, BlockPos &pos)
{
    auto mapgenstructure = structureGenerators.get(structureName);
    return mapgenstructure != nullptr ? mapgenstructure.isInsideStructure(pos) : false;
}

void ChunkGeneratorFlat::recreateStructures(Chunk &chunkIn, int32_t x, int32_t z)
{
    for (auto mapgenstructure : structureGenerators)
    {
        mapgenstructure.generate(world, x, z, nullptr);
    }
}
