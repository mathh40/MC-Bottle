#include "MapGenStructure.h"

#include "ReportedException.h"
#include "chunk/ChunkPrimer.h"
#include "math/ChunkPos.h"

bool MapGenStructure::generateStructure(World *worldIn, pcg32 &randomIn, const ChunkPos &chunkCoord)
{
    initializeStructureData(worldIn);
    auto i    = (chunkCoord.getx() << 4) + 8;
    auto j    = (chunkCoord.getz() << 4) + 8;
    bool flag = false;

    for (auto structurestart : structureMap)
    {
        if (structurestart.second.isSizeableStructure() && structurestart.second.isValidForPostProcess(chunkCoord) &&
            structurestart.second.getBoundingBox().intersectsWith(i, j, i + 15, j + 15))
        {
            structurestart.second.generateStructure(worldIn, randomIn, StructureBoundingBox(i, j, i + 15, j + 15));
            structurestart.second.notifyPostProcessAt(chunkCoord);
            flag = true;
            setStructureStart(structurestart.second.getChunkPosX(), structurestart.second.getChunkPosZ(),
                              structurestart);
        }
    }

    return flag;
}

bool MapGenStructure::isInsideStructure(const BlockPos &pos)
{
    if (world == nullptr)
    {
        return false;
    }
    else
    {
        initializeStructureData(world);
        return getStructureAt(pos).has_value();
    }
}

bool MapGenStructure::isPositionInStructure(World *worldIn, const BlockPos &pos)
{
    initializeStructureData(worldIn);
    auto objectiterator = structureMap.begin();

    StructureStart structurestart;
    do
    {
        if (objectiterator == structureMap.end())
        {
            return false;
        }

        structurestart = (objectiterator++)->second;
    } while (!structurestart.isSizeableStructure() || !structurestart.getBoundingBox().isVecInside(pos));

    return true;
}

void MapGenStructure::recursiveGenerate(World *worldIn, int32_t chunkX, int32_t chunkZ, int32_t originalX,
                                        int32_t originalZ, ChunkPrimer &chunkPrimerIn)
{
    initializeStructureData(worldIn);
    if (structureMap.find(ChunkPos::asLong(chunkX, chunkZ)) == structureMap.end())
    {
        rand();
        try
        {
            if (canSpawnStructureAtCoords(chunkX, chunkZ))
            {
                StructureStart structurestart = getStructureStart(chunkX, chunkZ);
                structureMap.emplace(ChunkPos::asLong(chunkX, chunkZ), structurestart);
                if (structurestart.isSizeableStructure())
                {
                    setStructureStart(chunkX, chunkZ, structurestart);
                }
            }
        }
        catch (std::exception &var10)
        {
            CrashReport crashreport = CrashReport.makeCrashReport(var10, "Exception preparing structure feature");
            CrashReportCategory crashreportcategory = crashreport.makeCategory("Feature being prepared");
            crashreportcategory.addDetail("Is feature chunk", new ICrashReportDetail()
				{
			   public String call() throws Exception
				{
				  return MapGenStructure.this.canSpawnStructureAtCoords(chunkX, chunkZ) ? "True" : "False";
        }
    });
    crashreportcategory.addCrashSection("Chunk location", String.format("%d,%d", chunkX, chunkZ));
            crashreportcategory.addDetail("Chunk pos hash", new ICrashReportDetail()
				{
			   public String call() throws Exception {
				  return String.valueOf(ChunkPos.asLong(chunkX, chunkZ));
}
});
            crashreportcategory.addDetail("Structure type", new ICrashReportDetail()
				{
			   public String call() throws Exception {
				  return MapGenStructure.this.getClass().getCanonicalName();
            }
            });
            throw ReportedException(crashreport);
            }
            }
            }

            std::optional<StructureStart> MapGenStructure::getStructureAt(const BlockPos &pos)
            {
                auto objectiterator = structureMap.begin();

                while (true)
                {
                    StructureStart structurestart;
                    do
                    {
                        do
                        {
                            if (objectiterator == structureMap.end())
                            {
                                return std::nullopt;
                            }

                            structurestart = (*objectiterator++).second;
                        } while (!structurestart.isSizeableStructure());
                    } while (!structurestart.getBoundingBox().isVecInside(pos));

                    for (auto structurecomponent : structurestart.getComponents())
                    {
                        if (structurecomponent.getBoundingBox().isVecInside(pos))
                        {
                            return structurestart;
                        }
                    }
                }
            }

            void MapGenStructure::initializeStructureData(World *worldIn)
            {
                if (structureData.empty() && worldIn != nullptr)
                {
                    structureData = worldIn->loadData(MapGenStructureData.class, getStructureName());
                    if (structureData == fmt::internal::null)
                    {
                        structureData = MapGenStructureData(getStructureName());
                        worldIn->setData(getStructureName(), structureData);
                    }
                    else
                    {
                        auto nbttagcompound = structureData.getTagCompound();

                        for (auto s : nbttagcompound)
                        {
                            auto nbtbase = nbttagcompound->getTag(s);
                            if (nbtbase.getId() == 10)
                            {
                                auto nbttagcompound1 = (NBTTagCompound)nbtbase;
                                if (nbttagcompound1.hasKey("ChunkX") && nbttagcompound1.hasKey("ChunkZ"))
                                {
                                    int i = nbttagcompound1.getInteger("ChunkX");
                                    int j = nbttagcompound1.getInteger("ChunkZ");
                                    auto structurestart =
                                        MapGenStructureIO::getStructureStart(nbttagcompound1, worldIn);
                                    if (structurestart != std::nullopt)
                                    {
                                        structureMap.emplace(ChunkPos::asLong(i, j), structurestart);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            std::optional<BlockPos> MapGenStructure::findNearestStructurePosBySpacing(
                World *worldIn, MapGenStructure &structureType, const BlockPos &startPos, int32_t distanceStep,
                int32_t stepOffset, int32_t randomSeedZ, bool addExtraRandomness, int32_t maxAttempts,
                bool findUnexplored)
            {
                auto i = startPos.getx() >> 4;
                auto j = startPos.getz() >> 4;
                auto k = 0;

                for (pcg32 random; k <= maxAttempts; ++k)
                {
                    for (auto l = -k; l <= k; ++l)
                    {
                        bool flag = l == -k || l == k;

                        for (auto i1 = -k; i1 <= k; ++i1)
                        {
                            bool flag1 = i1 == -k || i1 == k;
                            if (flag || flag1)
                            {
                                auto j1 = i + distanceStep * l;
                                auto k1 = j + distanceStep * i1;
                                if (j1 < 0)
                                {
                                    j1 -= distanceStep - 1;
                                }

                                if (k1 < 0)
                                {
                                    k1 -= distanceStep - 1;
                                }

                                auto l1      = j1 / distanceStep;
                                auto i2      = k1 / distanceStep;
                                auto random1 = worldIn->setRandomSeed(l1, i2, randomSeedZ);
                                l1 *= distanceStep;
                                i2 *= distanceStep;
                                if (addExtraRandomness)
                                {
                                    l1 += (random1(distanceStep - stepOffset) + random1(distanceStep - stepOffset)) / 2;
                                    i2 += (random1(distanceStep - stepOffset) + random1(distanceStep - stepOffset)) / 2;
                                }
                                else
                                {
                                    l1 += random1(distanceStep - stepOffset);
                                    i2 += random1(distanceStep - stepOffset);
                                }

                                MapGenBase::setupChunkSeed(worldIn->getSeed(), random, l1, i2);
                                random();
                                if (structureType.canSpawnStructureAtCoords(l1, i2))
                                {
                                    if (!findUnexplored || !worldIn->isChunkGeneratedAt(l1, i2))
                                    {
                                        return BlockPos((l1 << 4) + 8, 64, (i2 << 4) + 8);
                                    }
                                }
                                else if (k == 0)
                                {
                                    break;
                                }
                            }
                        }

                        if (k == 0)
                        {
                            break;
                        }
                    }
                }

                return std::nullopt;
            }

            void MapGenStructure::setStructureStart(int chunkX, int chunkZ, StructureStart start)
            {
                structureData.writeInstance(start.writeStructureComponentsToNBT(chunkX, chunkZ), chunkX, chunkZ);
                structureData.markDirty();
            }
