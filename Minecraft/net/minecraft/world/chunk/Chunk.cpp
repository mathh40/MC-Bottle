#include "Chunk.h"

#include "../../../../../spdlog/include/spdlog/logger.h"
#include "ITileEntityProvider.h"
#include "ReportedException.h"

std::shared_ptr<spdlog::logger> Chunk::LOGGER = spdlog::get("Minecraft")->clone("Chunk");

Chunk::Chunk(World *worldIn, int32_t xIn, int32_t zIn) : queuedLightChecks(4096), world(worldIn), x(xIn), z(zIn)
{
    this.tileEntities = Maps.newHashMap();
    this.entityLists  = (ClassInheritanceMultiMap[])(new ClassInheritanceMultiMap[16]);

    for (auto i = 0; i < entityLists.length; ++i)
    {
        entityLists[i] = new ClassInheritanceMultiMap(entity->class);
    }

    std::fill(precipitationHeightMap.begin(), precipitationHeightMap.end(), -999);
    std::fill(blockBiomeArray.begin(), blockBiomeArray.end(), -1);
}

Chunk::Chunk(World *worldIn, ChunkPrimer primer, int32_t x, int32_t z) : Chunk(worldIn, x, z)
{
    auto flag = worldIn->provider.hasSkyLight();

    for (auto j = 0; j < 16; ++j)
    {
        for (auto k = 0; k < 16; ++k)
        {
            for (auto l = 0; l < 256; ++l)
            {
                IBlockState *iblockstate = primer.getBlockState(j, l, k);
                if (iblockstate->getMaterial() != Material::AIR)
                {
                    auto i1 = l >> 4;
                    if (storageArrays[i1] == NULL_BLOCK_STORAGE)
                    {
                        storageArrays[i1] = ExtendedBlockStorage(i1 << 4, flag);
                    }

                    storageArrays[i1].set(j, l & 15, k, iblockstate);
                }
            }
        }
    }
}

bool Chunk::isAtLocation(int32_t x, int32_t z)
{
    return x == x && z == z;
}

int32_t Chunk::getHeight(BlockPos &pos)
{
    return getHeightValue(pos.getx() & 15, pos.getz() & 15);
}

int32_t Chunk::getHeightValue(int32_t x, int32_t z)
{
    return heightMap[z << 4 | x];
}

int32_t Chunk::getTopFilledSegment()
{
    auto extendedblockstorage = getLastExtendedBlockStorage();
    return extendedblockstorage == nullptr ? 0 : extendedblockstorage->getYLocation();
}

std::array<ExtendedBlockStorage *, 16> Chunk::getBlockStorageArray()
{
    return storageArrays;
}

void Chunk::generateSkylightMap()
{
    auto i           = getTopFilledSegment();
    heightMapMinimum = std::numeric_limits<int32_t>::max();

    for (auto j = 0; j < 16; ++j)
    {
        for (auto k = 0; k < 16; ++k)
        {
            precipitationHeightMap[j + (k << 4)] = -999;

            for (auto k1 = i + 16; k1 > 0; --k1)
            {
                if (getBlockLightOpacity(j, k1 - 1, k) != 0)
                {
                    heightMap[k << 4 | j] = k1;
                    if (k1 < heightMapMinimum)
                    {
                        heightMapMinimum = k1;
                    }
                    break;
                }
            }

            if (world.provider.hasSkyLight())
            {
                auto k1 = 15;
                auto i1 = i + 16 - 1;

                do
                {
                    auto j1 = getBlockLightOpacity(j, i1, k);
                    if (j1 == 0 && k1 != 15)
                    {
                        j1 = 1;
                    }

                    k1 -= j1;
                    if (k1 > 0)
                    {
                        auto extendedblockstorage = storageArrays[i1 >> 4];
                        if (extendedblockstorage != NULL_BLOCK_STORAGE)
                        {
                            extendedblockstorage->setSkyLight(j, i1 & 15, k, k1);
                            world->notifyLightSet(BlockPos((x << 4) + j, i1, (z << 4) + k));
                        }
                    }

                    --i1;
                } while (i1 > 0 && k1 > 0);
            }
        }
    }

    dirty = true;
}

int32_t Chunk::getBlockLightOpacity(BlockPos &pos)
{
    return getBlockState(pos).getLightOpacity();
}

IBlockState *Chunk::getBlockState(BlockPos &pos)
{
    return getBlockState(pos.getx(), pos.gety(), pos.getz());
}

IBlockState *Chunk::getBlockState(int32_t x, int32_t y, int32_t z)
{
    if (world->getWorldType() == WorldType::DEBUG_ALL_BLOCK_STATES)
    {
        IBlockState *iblockstate = nullptr;
        if (y == 60)
        {
            iblockstate = Blocks::BARRIER.getDefaultState();
        }

        if (y == 70)
        {
            iblockstate = ChunkGeneratorDebug.getBlockStateFor(x, z);
        }

        return iblockstate == nullptr ? Blocks::AIR.getDefaultState() : iblockstate;
    }
    else
    {
        try
        {
            if (y >= 0 && y >> 4 < storageArrays.size())
            {
                auto extendedblockstorage = storageArrays[y >> 4];
                if (extendedblockstorage != NULL_BLOCK_STORAGE)
                {
                    return extendedblockstorage.get(x & 15, y & 15, z & 15);
                }
            }

            return Blocks::AIR.getDefaultState();
        }
        catch (Throwable var7)
        {
            CrashReport crashreport                 = CrashReport.makeCrashReport(var7, "Getting block state");
            CrashReportCategory crashreportcategory = crashreport.makeCategory("Block being got");
            crashreportcategory.addDetail("Location", new ICrashReportDetail()
				{
			   public String call() throws Exception
				{
				  return CrashReportCategory.getCoordinateInfo(x, y, z);
        }
    });
    throw ReportedException(crashreport);
}
}
}

IBlockState *Chunk::setBlockState(BlockPos &pos, IBlockState *state)
{
    auto i = pos.getx() & 15;
    auto j = pos.gety();
    auto k = pos.getz() & 15;
    auto l = k << 4 | i;
    if (j >= precipitationHeightMap[l] - 1)
    {
        precipitationHeightMap[l] = -999;
    }

    auto i1                  = heightMap[l];
    IBlockState *iblockstate = getBlockState(pos);
    if (iblockstate == state)
    {
        return nullptr;
    }
    else
    {
        auto block                = state->getBlock();
        auto block1               = iblockstate->getBlock();
        auto extendedblockstorage = storageArrays[j >> 4];
        auto flag                 = false;
        if (extendedblockstorage == NULL_BLOCK_STORAGE)
        {
            if (block == Blocks::AIR)
            {
                return nullptr;
            }

            extendedblockstorage  = new ExtendedBlockStorage(j >> 4 << 4, world->provider.hasSkyLight());
            storageArrays[j >> 4] = extendedblockstorage;
            flag                  = j >= i1;
        }

        extendedblockstorage.set(i, j & 15, k, state);
        if (block1 != block)
        {
            if (!world->isRemote)
            {
                block1->breakBlock(world, pos, iblockstate);
            }
            else if (Util:: instanceof <ITileEntityProvider>(block1))
            {
                world->removeTileEntity(pos);
            }
        }

        if (extendedblockstorage.get(i, j & 15, k).getBlock() != block)
        {
            return nullptr;
        }
        else
        {
            if (flag)
            {
                generateSkylightMap();
            }
            else
            {
                auto j1 = state->getLightOpacity();
                auto k1 = iblockstate->getLightOpacity();
                if (j1 > 0)
                {
                    if (j >= i1)
                    {
                        relightBlock(i, j + 1, k);
                    }
                }
                else if (j == i1 - 1)
                {
                    relightBlock(i, j, k);
                }

                if (j1 != k1 &&
                    (j1 < k1 || getLightFor(EnumSkyBlock::SKY, pos) > 0 || getLightFor(EnumSkyBlock::BLOCK, pos) > 0))
                {
                    propagateSkylightOcclusion(i, k);
                }
            }

            TileEntity *tileentity1;
            if (Util:: instanceof <ITileEntityProvider>(block1))
            {
                tileentity1 = getTileEntity(pos, Chunk.EnumCreateEntityType.CHECK);
                if (tileentity1 != nullptr)
                {
                    tileentity1->updateContainingBlockInfo();
                }
            }

            if (!world->isRemote && block1 != block)
            {
                block->onBlockAdded(world, pos, state);
            }

            if (Util:: instanceof <ITileEntityProvider>(block))
            {
                tileentity1 = getTileEntity(pos, Chunk.EnumCreateEntityType.CHECK);
                if (tileentity1 == nullptr)
                {
                    tileentity1 = block->createNewTileEntity(world, block->getMetaFromState(state));
                    world->setTileEntity(pos, tileentity1);
                }

                if (tileentity1 != nullptr)
                {
                    tileentity1->updateContainingBlockInfo();
                }
            }

            dirty = true;
            return iblockstate;
        }
    }
}

int32_t Chunk::getLightFor(EnumSkyBlock type, BlockPos &pos)
{
    auto i                    = pos.getx() & 15;
    auto j                    = pos.gety();
    auto k                    = pos.getz() & 15;
    auto extendedblockstorage = storageArrays[j >> 4];
    if (extendedblockstorage == NULL_BLOCK_STORAGE)
    {
        return canSeeSky(pos) ? type.defaultLightValue : 0;
    }
    else if (type == EnumSkyBlock::SKY)
    {
        return !world->provider.hasSkyLight() ? 0 : extendedblockstorage.getSkyLight(i, j & 15, k);
    }
    else
    {
        return type == EnumSkyBlock::BLOCK ? extendedblockstorage.getBlockLight(i, j & 15, k) : type.defaultLightValue;
    }
}

void Chunk::setLightFor(EnumSkyBlock type, BlockPos &pos, int32_t value)
{
    auto i                    = pos.getx() & 15;
    auto j                    = pos.gety();
    auto k                    = pos.getz() & 15;
    auto extendedblockstorage = storageArrays[j >> 4];
    if (extendedblockstorage == NULL_BLOCK_STORAGE)
    {
        extendedblockstorage  = new ExtendedBlockStorage(j >> 4 << 4, world.provider.hasSkyLight());
        storageArrays[j >> 4] = extendedblockstorage;
        generateSkylightMap();
    }

    dirty = true;
    if (type == EnumSkyBlock::SKY)
    {
        if (world.provider.hasSkyLight())
        {
            extendedblockstorage.setSkyLight(i, j & 15, k, value);
        }
    }
    else if (type == EnumSkyBlock::BLOCK)
    {
        extendedblockstorage.setBlockLight(i, j & 15, k, value);
    }
}

int32_t Chunk::getLightSubtracted(BlockPos &pos, int32_t amount)
{
    auto i                    = pos.getx() & 15;
    auto j                    = pos.gety();
    auto k                    = pos.getz() & 15;
    auto extendedblockstorage = storageArrays[j >> 4];
    if (extendedblockstorage != NULL_BLOCK_STORAGE)
    {
        auto l = !world.provider.hasSkyLight() ? 0 : extendedblockstorage.getSkyLight(i, j & 15, k);
        l -= amount;
        int i1 = extendedblockstorage.getBlockLight(i, j & 15, k);
        if (i1 > l)
        {
            l = i1;
        }

        return l;
    }
    else
    {
        return world.provider.hasSkyLight() && amount < EnumSkyBlock.SKY.defaultLightValue
                   ? EnumSkyBlock.SKY.defaultLightValue - amount
                   : 0;
    }
}

void Chunk::addEntity(Entity *entityIn)
{
    hasEntities = true;
    auto i      = MathHelper::floor(entityIn.posX / 16.0);
    auto j      = MathHelper::floor(entityIn.posZ / 16.0);
    if (i != x || j != z)
    {
        LOGGER->warn("Wrong location! ({}, {}) should be ({}, {}), {}", i, j, x, z, entityIn);
        entityIn->setDead();
    }

    int k = MathHelper::floor(entityIn.posY / 16.0);
    if (k < 0)
    {
        k = 0;
    }

    if (k >= entityLists.size())
    {
        k = entityLists.size() - 1;
    }

    entityIn.addedToChunk = true;
    entityIn.chunkCoordX  = x;
    entityIn.chunkCoordY  = k;
    entityIn.chunkCoordZ  = z;
    entityLists[k].add(entityIn);
}

void Chunk::removeEntity(Entity *entityIn)
{
    removeEntityAtIndex(entityIn, entityIn->chunkCoordY);
}

void Chunk::removeEntityAtIndex(Entity *entityIn, int32_t index)
{
    if (index < 0)
    {
        index = 0;
    }

    if (index >= entityLists.size())
    {
        index = entityLists.size() - 1;
    }

    entityLists[index].remove(entityIn);
}

bool Chunk::canSeeSky(BlockPos &pos)
{
    auto i = pos.getx() & 15;
    auto j = pos.gety();
    auto k = pos.getz() & 15;
    return j >= heightMap[k << 4 | i];
}

int32_t Chunk::getBlockLightOpacity(int32_t x, int32_t y, int32_t z)
{
    return getBlockState(x, y, z).getLightOpacity();
}

TileEntity *Chunk::createNewTileEntity(BlockPos &pos)
{
    auto iblockstate = getBlockState(pos);
    auto block       = iblockstate->getBlock();
    return !block->hasTileEntity()
               ? nullptr
               : block->createNewTileEntity(world, iblockstate->getBlock()->getMetaFromState(iblockstate));
}

void Chunk::setSkylightUpdated()
{
    for (auto &sky : updateSkylightColumns)
    {
        sky = true;
    }

    recheckGaps(false);
}

void Chunk::checkLightSide(EnumFacing facing)
{
    if (isTerrainPopulated)
    {
        int l;
        if (facing == EnumFacing::EAST)
        {
            for (l = 0; l < 16; ++l)
            {
                checkLight(15, l);
            }
        }
        else if (facing == EnumFacing::WEST)
        {
            for (l = 0; l < 16; ++l)
            {
                checkLight(0, l);
            }
        }
        else if (facing == EnumFacing::SOUTH)
        {
            for (l = 0; l < 16; ++l)
            {
                checkLight(l, 15);
            }
        }
        else if (facing == EnumFacing::NORTH)
        {
            for (l = 0; l < 16; ++l)
            {
                checkLight(l, 0);
            }
        }
    }
}

bool Chunk::checkLight(int32_t x, int32_t z)
{
    auto i     = getTopFilledSegment();
    auto flag  = false;
    auto flag1 = false;
    MutableBlockPos blockpos$mutableblockpos((x << 4) + x, 0, (z << 4) + z);

    for (auto l = i + 16 - 1; l > world->getSeaLevel() || l > 0 && !flag1; --l)
    {
        blockpos$mutableblockpos.setPos(blockpos$mutableblockpos.getx(), l, blockpos$mutableblockpos.getz());
        auto k = getBlockLightOpacity(blockpos$mutableblockpos);
        if (k == 255 && blockpos$mutableblockpos.gety() < world->getSeaLevel())
        {
            flag1 = true;
        }

        if (!flag && k > 0)
        {
            flag = true;
        }
        else if (flag && k == 0 && !world->checkLight(blockpos$mutableblockpos))
        {
            return false;
        }
    }

    for (auto l = blockpos$mutableblockpos.gety(); l > 0; --l)
    {
        blockpos$mutableblockpos.setPos(blockpos$mutableblockpos.getx(), l, blockpos$mutableblockpos.getz());
        if (getBlockState(blockpos$mutableblockpos).getLightValue() > 0)
        {
            world->checkLight(blockpos$mutableblockpos);
        }
    }

    return true;
}

bool Chunk::needsSaving(bool p_76601_1_) const
{
    if (p_76601_1_)
    {
        if (hasEntities && world->getTotalWorldTime() != lastSaveTime || dirty)
        {
            return true;
        }
    }
    else if (hasEntities && world->getTotalWorldTime() >= lastSaveTime + 600L)
    {
        return true;
    }

    return dirty;
}

pcg32 Chunk::getRandomWithSeed(int64_t seed) const
{
    return pcg32(world->getSeed() + (x * x * 4987142) + (x * 5947611) + (z * z) * 4392871L + (z * 389711) ^ seed);
}

bool Chunk::isEmpty()
{
    return false;
}

void Chunk::populate(IChunkProvider *chunkProvider, IChunkGenerator *chunkGenrator)
{
    auto chunk  = chunkProvider->getLoadedChunk(x, z - 1);
    auto chunk1 = chunkProvider->getLoadedChunk(x + 1, z);
    auto chunk2 = chunkProvider->getLoadedChunk(x, z + 1);
    auto chunk3 = chunkProvider->getLoadedChunk(x - 1, z);
    if (chunk1 != nullptr && chunk2 != nullptr && chunkProvider->getLoadedChunk(x + 1, z + 1) != nullptr)
    {
        populate(chunkGenrator);
    }

    if (chunk3 != nullptr && chunk2 != nullptr && chunkProvider->getLoadedChunk(x - 1, z + 1) != nullptr)
    {
        chunk3->populate(chunkGenrator);
    }

    if (chunk != nullptr && chunk1 != nullptr && chunkProvider->getLoadedChunk(x + 1, z - 1) != nullptr)
    {
        chunk->populate(chunkGenrator);
    }

    if (chunk != nullptr && chunk3 != nullptr)
    {
        auto chunk4 = chunkProvider->getLoadedChunk(x - 1, z - 1);
        if (chunk4 != nullptr)
        {
            chunk4->populate(chunkGenrator);
        }
    }
}

BlockPos Chunk::getPrecipitationHeight(BlockPos &pos)
{
    auto i        = pos.getx() & 15;
    auto j        = pos.getz() & 15;
    auto k        = i | j << 4;
    auto blockpos = BlockPos(pos.getx(), precipitationHeightMap[k], pos.getz());
    if (blockpos.gety() == -999)
    {
        auto l   = getTopFilledSegment() + 15;
        blockpos = BlockPos(pos.getx(), l, pos.getz());
        auto i1  = -1;

        while (true)
        {
            while (blockpos.gety() > 0 && i1 == -1)
            {
                auto iblockstate = getBlockState(blockpos);
                auto material    = iblockstate->getMaterial();
                if (!material.blocksMovement() && !material.isLiquid())
                {
                    blockpos = blockpos.down();
                }
                else
                {
                    i1 = blockpos.gety() + 1;
                }
            }

            precipitationHeightMap[k] = i1;
            break;
        }
    }

    return BlockPos(pos.getx(), precipitationHeightMap[k], pos.getz());
}

void Chunk::onTick(bool skipRecheckGaps)
{
    if (isGapLightingUpdated && world->provider.hasSkyLight() && !skipRecheckGaps)
    {
        recheckGaps(world->isRemote);
    }

    ticked = true;
    if (!isLightPopulated && isTerrainPopulated)
    {
        checkLight();
    }

    BlockPos blockpos;
    while (tileEntityPosQueue.try_dequeue(blockpos))
    {
        if (getTileEntity(blockpos, Chunk.EnumCreateEntityType.CHECK) == nullptr &&
            getBlockState(blockpos).getBlock().hasTileEntity())
        {
            auto tileentity = createNewTileEntity(blockpos);
            world->setTileEntity(blockpos, tileentity);
            world->markBlockRangeForRenderUpdate(blockpos, blockpos);
        }
    }
}

bool Chunk::isPopulated() const
{
    return ticked && isTerrainPopulated && isLightPopulated;
}

bool Chunk::wasTicked() const
{
    return ticked;
}

ChunkPos Chunk::getPos() const
{
    return ChunkPos(x, z);
}

bool Chunk::isEmptyBetween(int32_t startY, int32_t endY)
{
    if (startY < 0)
    {
        startY = 0;
    }

    if (endY >= 256)
    {
        endY = 255;
    }

    for (auto i = startY; i <= endY; i += 16)
    {
        auto extendedblockstorage = storageArrays[i >> 4];
        if (extendedblockstorage != NULL_BLOCK_STORAGE && !extendedblockstorage->isEmpty())
        {
            return false;
        }
    }

    return true;
}

void Chunk::setStorageArrays(std::array<ExtendedBlockStorage *, 16> newStorageArrays)
{
    if (storageArrays.size() != newStorageArrays.size())
    {
        LOGGER->warn("Could not set level chunk sections, array length is {} instead of {}", newStorageArrays.size(),
                     storageArrays.size());
    }
    else
    {
        std::copy(newStorageArrays.begin(), newStorageArrays.end(), storageArrays.begin());
    }
}

void Chunk::read(PacketBuffer buf, int32_t availableSections, bool groundUpContinuous)
{
    bool flag = world->provider.hasSkyLight();

    for (auto j = 0; j < storageArrays.size(); ++j)
    {
        auto extendedblockstorage = storageArrays[j];
        if ((availableSections & 1 << j) == 0)
        {
            if (groundUpContinuous && extendedblockstorage != NULL_BLOCK_STORAGE)
            {
                storageArrays[j] = NULL_BLOCK_STORAGE;
            }
        }
        else
        {
            if (extendedblockstorage == NULL_BLOCK_STORAGE)
            {
                extendedblockstorage = ExtendedBlockStorage(j << 4, flag);
                storageArrays[j]     = extendedblockstorage;
            }

            extendedblockstorage->getData().read(buf);
            buf.readBytes(extendedblockstorage->getBlockLight().getData());
            if (flag)
            {
                buf.readBytes(extendedblockstorage->getSkyLight().getData());
            }
        }
    }

    if (groundUpContinuous)
    {
        buf.readBytes(blockBiomeArray);
    }

    for (auto j = 0; j < storageArrays.size(); ++j)
    {
        if (storageArrays[j] != NULL_BLOCK_STORAGE && (availableSections & 1 << j) != 0)
        {
            storageArrays[j]->recalculateRefCounts();
        }
    }

    isLightPopulated   = true;
    isTerrainPopulated = true;
    generateHeightMap();

    for (auto tileentity : tileEntities)
    {
        tileentity.second->updateContainingBlockInfo();
    }
}

Biome *Chunk::getBiome(BlockPos &pos, BiomeProvider provider)
{
    auto i = pos.getx() & 15;
    auto j = pos.getz() & 15;
    auto k = blockBiomeArray[j << 4 | i] & 255;
    if (k == 255)
    {
        auto biome1                 = provider.getBiome(pos, Biomes::PLAINS);
        k                           = Biome::getIdForBiome(biome1);
        blockBiomeArray[j << 4 | i] = (unsigned char)(k & 255);
    }

    auto biome1 = Biome::getBiome(k);
    return biome1 == nullptr ? Biomes::PLAINS : biome1;
}

std::array<unsigned char, 256> Chunk::getBiomeArray() const
{
    return blockBiomeArray;
}

void Chunk::setBiomeArray(std::array<unsigned char, 256> biomeArray)
{
    if (blockBiomeArray.size() != biomeArray.size())
    {
        LOGGER->warn("Could not set level chunk biomes, array length is {} instead of {}", biomeArray.size(),
                     blockBiomeArray.size());
    }
    else
    {
        std::copy(biomeArray.begin(), biomeArray.end(), blockBiomeArray.begin());
    }
}

void Chunk::resetRelightChecks()
{
    queuedLightChecks = 0;
}

void Chunk::enqueueRelightChecks()
{
    if (queuedLightChecks < 4096)
    {
        BlockPos blockpos(x << 4, 0, z << 4);

        for (auto i = 0; i < 8; ++i)
        {
            if (queuedLightChecks >= 4096)
            {
                return;
            }

            auto j = queuedLightChecks % 16;
            auto k = queuedLightChecks / 16 % 16;
            auto l = queuedLightChecks / 256;
            ++queuedLightChecks;

            for (auto i1 = 0; i1 < 16; ++i1)
            {
                auto blockpos1 = blockpos.add(k, (j << 4) + i1, l);
                auto flag      = i1 == 0 || i1 == 15 || k == 0 || k == 15 || l == 0 || l == 15;
                if (storageArrays[j] == NULL_BLOCK_STORAGE && flag ||
                    storageArrays[j] != NULL_BLOCK_STORAGE &&
                        storageArrays[j].get(k, i1, l).getMaterial() == Material::AIR)
                {
                    for (auto enumfacing : EnumFacing::values())
                    {
                        auto blockpos2 = blockpos1.offset(enumfacing);
                        if (world->getBlockState(blockpos2).getLightValue() > 0)
                        {
                            world->checkLight(blockpos2);
                        }
                    }

                    world->checkLight(blockpos1);
                }
            }
        }
    }
}

void Chunk::checkLight()
{
    isTerrainPopulated = true;
    isLightPopulated   = true;
    BlockPos blockpos(x << 4, 0, z << 4);
    if (world->provider.hasSkyLight())
    {
        if (world->isAreaLoaded(blockpos.add(-1, 0, -1), blockpos.add(16, world->getSeaLevel(), 16)))
        {
        label44:
            for (auto i = 0; i < 16; ++i)
            {
                for (auto j = 0; j < 16; ++j)
                {
                    if (!checkLight(i, j))
                    {
                        isLightPopulated = false;
                        goto label44;
                    }
                }
            }

            if (isLightPopulated)
            {
                Iterator var5 = EnumFacing.Plane.HORIZONTAL.iterator();

                while (var5.hasNext())
                {
                    EnumFacing enumfacing = (EnumFacing)var5.next();
                    int k                 = enumfacing.getAxisDirection() == EnumFacing.AxisDirection.POSITIVE ? 16 : 1;
                    world.getChunk(blockpos.offset(enumfacing, k)).checkLightSide(enumfacing.getOpposite());
                }

                setSkylightUpdated();
            }
        }
        else
        {
            isLightPopulated = false;
        }
    }
}

bool Chunk::isLoaded() const
{
    return loaded;
}

void Chunk::markLoaded(bool loaded)
{
    loaded = loaded;
}

World *Chunk::getWorld() const
{
    return world;
}

std::array<int32_t, 256> Chunk::getHeightMap() const
{
    return heightMap;
}

void Chunk::setHeightMap(std::array<int32_t, 256> newHeightMap)
{
    if (heightMap.size() != newHeightMap.size())
    {
        LOGGER->warn("Could not set level chunk heightmap, array length is {} instead of {}", newHeightMap.size(),
                     heightMap.size());
    }
    else
    {
        std::copy(newHeightMap.begin(), newHeightMap.end(), heightMap.begin());
    }
}

ClassInheritanceMultiMap[] Chunk::getEntityLists()
{
    return entityLists;
}

std::unordered_map<BlockPos, Entity *> Chunk::getTileEntityMap() const
{
    return tileEntities;
}

bool Chunk::isTerrainPopulated() const
{
    return TerrainPopulated;
}

void Chunk::setTerrainPopulated(bool terrainPopulated)
{
    TerrainPopulated = terrainPopulated;
}

bool Chunk::isLightPopulated() const
{
    return LightPopulated;
}

void Chunk::setLightPopulated(bool lightPopulated)
{
    LightPopulated = lightPopulated;
}

void Chunk::setModified(bool modified)
{
    dirty = modified;
}

void Chunk::setHasEntities(bool hasEntitiesIn)
{
    hasEntities = hasEntitiesIn;
}

void Chunk::setLastSaveTime(int64_t saveTime)
{
    lastSaveTime = saveTime;
}

int32_t Chunk::getLowestHeight() const
{
    return heightMapMinimum;
}

int64_t Chunk::getInhabitedTime() const
{
    return inhabitedTime;
}

void Chunk::setInhabitedTime(int64_t newInhabitedTime)
{
    inhabitedTime = newInhabitedTime;
}

void Chunk::populate(IChunkGenerator *generator)
{
    if (isTerrainPopulated())
    {
        if (generator->generateStructures(this, x, z))
        {
            markDirty();
        }
    }
    else
    {
        checkLight();
        generator->populate(x, z);
        markDirty();
    }
}

void Chunk::generateHeightMap()
{
    auto i           = getTopFilledSegment();
    heightMapMinimum = std::numeric_limits<int32_t>::max();

    for (auto j = 0; j < 16; ++j)
    {
        for (auto k = 0; k < 16; ++k)
        {
            precipitationHeightMap[j + (k << 4)] = -999;

            for (auto l = i + 16; l > 0; --l)
            {
                IBlockState *iblockstate = getBlockState(j, l - 1, k);
                if (iblockstate->getLightOpacity() != 0)
                {
                    heightMap[k << 4 | j] = l;
                    if (l < heightMapMinimum)
                    {
                        heightMapMinimum = l;
                    }
                    break;
                }
            }
        }
    }

    dirty = true;
}

ExtendedBlockStorage *Chunk::getLastExtendedBlockStorage()
{
    for (auto i = storageArrays.size() - 1; i >= 0; --i)
    {
        if (storageArrays[i] != NULL_BLOCK_STORAGE)
        {
            return storageArrays[i];
        }
    }

    return nullptr;
}

void Chunk::propagateSkylightOcclusion(int32_t x, int32_t z)
{
    updateSkylightColumns[x + z * 16] = true;
    isGapLightingUpdated              = true;
}

void Chunk::recheckGaps(bool onlyOne)
{
    world->profiler.startSection("recheckGaps");
    if (world->isAreaLoaded(BlockPos(x * 16 + 8, 0, z * 16 + 8), 16))
    {
        for (auto i = 0; i < 16; ++i)
        {
            for (auto j = 0; j < 16; ++j)
            {
                if (updateSkylightColumns[i + j * 16])
                {
                    updateSkylightColumns[i + j * 16] = false;
                    auto k                            = getHeightValue(i, j);
                    auto l                            = x * 16 + i;
                    auto i1                           = z * 16 + j;
                    auto j1                           = std::numeric_limits<int32_t>::max();

                    Iterator var8;
                    EnumFacing enumfacing1;
                    for (var8 = Plane::HORIZONTAL.iterator(); var8.hasNext();
                         j1   = MathHelper::min(j1, world->getChunksLowestHorizon(l + enumfacing1.getXOffset(),
                                                                                  i1 + enumfacing1.getZOffset())))
                    {
                        enumfacing1 = (EnumFacing)var8.next();
                    }

                    checkSkylightNeighborHeight(l, i1, j1);
                    var8 = Plane::HORIZONTAL.iterator();

                    while (var8.hasNext())
                    {
                        enumfacing1 = (EnumFacing)var8.next();
                        checkSkylightNeighborHeight(l + enumfacing1.getXOffset(), i1 + enumfacing1.getZOffset(), k);
                    }

                    if (onlyOne)
                    {
                        world->profiler.endSection();
                        return;
                    }
                }
            }
        }

        isGapLightingUpdated = false;
    }

    world.profiler.endSection();
}

void Chunk::checkSkylightNeighborHeight(int32_t x, int32_t z, int32_t maxValue)
{
    auto i = world->getHeight(BlockPos(x, 0, z)).gety();
    if (i > maxValue)
    {
        updateSkylightNeighborHeight(x, z, maxValue, i + 1);
    }
    else if (i < maxValue)
    {
        updateSkylightNeighborHeight(x, z, i, maxValue + 1);
    }
}

void Chunk::updateSkylightNeighborHeight(int32_t x, int32_t z, int32_t startY, int32_t endY)
{
    if (endY > startY && world->isAreaLoaded(BlockPos(x, 0, z), 16))
    {
        for (auto i = startY; i < endY; ++i)
        {
            world->checkLightFor(EnumSkyBlock::SKY, BlockPos(x, i, z));
        }

        dirty = true;
    }
}

void Chunk::relightBlock(int32_t x, int32_t y, int32_t z)
{
    auto i = heightMap[z << 4 | x] & 255;
    auto j = i;
    if (y > i)
    {
        j = y;
    }

    while (j > 0 && getBlockLightOpacity(x, j - 1, z) == 0)
    {
        --j;
    }

    if (j != i)
    {
        world->markBlocksDirtyVertical(x + x * 16, z + z * 16, j, i);
        heightMap[z << 4 | x] = j;
        auto k                = x * 16 + x;
        auto l                = z * 16 + z;
        if (world->provider.hasSkyLight())
        {
            ExtendedBlockStorage *extendedblockstorage2;
            if (j < i)
            {
                for (auto k1 = j; k1 < i; ++k1)
                {
                    extendedblockstorage2 = storageArrays[k1 >> 4];
                    if (extendedblockstorage2 != NULL_BLOCK_STORAGE)
                    {
                        extendedblockstorage2.setSkyLight(x, k1 & 15, z, 15);
                        world->notifyLightSet(BlockPos((x << 4) + x, k1, (z << 4) + z));
                    }
                }
            }
            else
            {
                for (auto k1 = i; k1 < j; ++k1)
                {
                    extendedblockstorage2 = storageArrays[k1 >> 4];
                    if (extendedblockstorage2 != NULL_BLOCK_STORAGE)
                    {
                        extendedblockstorage2.setSkyLight(x, k1 & 15, z, 0);
                        world->notifyLightSet(BlockPos((x << 4) + x, k1, (z << 4) + z));
                    }
                }
            }

            auto k1 = 15;

            while (j > 0 && k1 > 0)
            {
                --j;
                auto j2 = getBlockLightOpacity(x, j, z);
                if (j2 == 0)
                {
                    j2 = 1;
                }

                k1 -= j2;
                if (k1 < 0)
                {
                    k1 = 0;
                }

                auto extendedblockstorage1 = storageArrays[j >> 4];
                if (extendedblockstorage1 != NULL_BLOCK_STORAGE)
                {
                    extendedblockstorage1->setSkyLight(x, j & 15, z, k1);
                }
            }
        }

        auto k1 = heightMap[z << 4 | x];
        auto j2 = i;
        int k2  = k1;
        if (k1 < i)
        {
            j2 = k1;
            k2 = i;
        }

        if (k1 < heightMapMinimum)
        {
            heightMapMinimum = k1;
        }

        if (world->provider.hasSkyLight())
        {
            Iterator var11 = EnumFacing.Plane.HORIZONTAL.iterator();

            while (var11.hasNext())
            {
                EnumFacing enumfacing = (EnumFacing)var11.next();
                updateSkylightNeighborHeight(k + enumfacing.getXOffset(), l + enumfacing.getZOffset(), j2, k2);
            }

            updateSkylightNeighborHeight(k, l, j2, k2);
        }

        dirty = true;
    }
}

TileEntity *Chunk::getTileEntity(BlockPos &pos, Chunk::EnumCreateEntityType creationMode)
{
    auto tileentity = tileEntities.get(pos);
    if (tileentity == nullptr)
    {
        if (creationMode == Chunk.EnumCreateEntityType::IMMEDIATE)
        {
            tileentity = createNewTileEntity(pos);
            world->setTileEntity(pos, tileentity);
        }
        else if (creationMode == Chunk.EnumCreateEntityType::QUEUED)
        {
            tileEntityPosQueue.enqueue(pos);
        }
    }
    else if (tileentity.isInvalid())
    {
        tileEntities.remove(pos);
        return nullptr;
    }

    return tileentity;
}

void Chunk::addTileEntity(TileEntity *tileEntityIn)
{
    addTileEntity(tileEntityIn->getPos(), tileEntityIn);
    if (loaded)
    {
        world->addTileEntity(tileEntityIn);
    }
}

void Chunk::addTileEntity(BlockPos &pos, TileEntity *tileEntityIn)
{
    tileEntityIn.setWorld(world);
    tileEntityIn.setPos(pos);
    if (Util:: instanceof <ITileEntityProvider>(getBlockState(pos).getBlock()))
    {
        if (tileEntities.containsKey(pos))
        {
            tileEntities.get(pos).invalidate();
        }

        tileEntityIn.validate();
        tileEntities.put(pos, tileEntityIn);
    }
}

void Chunk::removeTileEntity(BlockPos &pos)
{
    if (loaded)
    {
        auto tileentity = tileEntities.remove(pos);
        if (tileentity != nullptr)
        {
            tileentity.invalidate();
        }
    }
}

void Chunk::onLoad()
{
    loaded = true;
    world.addTileEntities(tileEntities.values());
    ClassInheritanceMultiMap[] var1 = this.entityLists;
    int var2                        = var1.length;

    for (int var3 = 0; var3 < var2; ++var3)
    {
        ClassInheritanceMultiMap classinheritancemultimap = var1[var3];
        world.loadEntities(classinheritancemultimap);
    }
}

void Chunk::onUnload()
{
    loaded        = false;
    Iterator var1 = tileEntities.values().iterator();

    while (var1.hasNext())
    {
        TileEntity tileentity = (TileEntity)var1.next();
        world.markTileEntityForRemoval(tileentity);
    }

    ClassInheritanceMultiMap[] var5 = this.entityLists;
    int var6                        = var5.length;

    for (int var3 = 0; var3 < var6; ++var3)
    {
        ClassInheritanceMultiMap classinheritancemultimap = var5[var3];
        world.unloadEntities(classinheritancemultimap);
    }
}

void Chunk::markDirty()
{
    dirty = true;
}
