#include "ChunkCache.h"

#include "biome/Biome.h"

ChunkCache::ChunkCache(World *worldIn, BlockPos &posFromIn, BlockPos &posToIn, int32_t subIn)
    : world(worldIn), chunkX(posFromIn.getx() - subIn >> 4), chunkZ(posFromIn.getz() - subIn >> 4)
{
    auto i     = posToIn.getX() + subIn >> 4;
    auto j     = posToIn.getZ() + subIn >> 4;
    chunkArray = multivec<Chunk>(i - chunkX + 1, j - chunkZ + 1);
    empty      = true;

    for (auto i1 = chunkX; i1 <= i; ++i1)
    {
        for (auto j1 = chunkZ; j1 <= j; ++j1)
        {
            chunkArray(i1 - chunkX, j1 - chunkZ) = worldIn.getChunk(i1, j1);
        }
    }

    for (auto i1 = posFromIn.getx() >> 4; i1 <= posToIn.getx() >> 4; ++i1)
    {
        for (auto j1 = posFromIn.getz() >> 4; j1 <= posToIn.getz() >> 4; ++j1)
        {
            auto chunk = chunkArray(i1 - chunkX, j1 - chunkZ);
            if (chunk != nullptr && !chunk.isEmptyBetween(posFromIn.gety(), posToIn.gety()))
            {
                empty = false;
            }
        }
    }
}

bool ChunkCache::isEmpty()
{
    return empty;
}

TileEntity *ChunkCache::getTileEntity(BlockPos &pos)
{
    return getTileEntity(pos, Chunk::EnumCreateEntityType.IMMEDIATE);
}

int32_t ChunkCache::getCombinedLight(BlockPos &pos, int32_t lightValue)
{
    auto i = getLightForExt(EnumSkyBlock.SKY, pos);
    auto j = getLightForExt(EnumSkyBlock.BLOCK, pos);
    if (j < lightValue)
    {
        j = lightValue;
    }

    return i << 20 | j << 4;
}

IBlockState *ChunkCache::getBlockState(BlockPos &pos)
{
    if (pos.gety() >= 0 && pos.gety() < 256)
    {
        auto i = (pos.getx() >> 4) - chunkX;
        auto j = (pos.getz() >> 4) - chunkZ;
        if (i >= 0 && i < chunkArray.length && j >= 0 && j < chunkArray[i].length)
        {
            Chunk chunk = chunkArray[i][j];
            if (chunk != nullptr)
            {
                return chunk.getBlockState(pos);
            }
        }
    }

    return Blocks::AIR.getDefaultState();
}

Biome &ChunkCache::getBiome(BlockPos &pos)
{
    auto i = (pos.getx() >> 4) - chunkX;
    auto j = (pos.getz() >> 4) - chunkZ;
    return chunkArray(i, j).getBiome(pos, world.getBiomeProvider());
}

bool ChunkCache::isAirBlock(BlockPos &pos)
{
    return getBlockState(pos)->getMaterial() == Material::AIR;
}

TileEntity *ChunkCache::getTileEntity(BlockPos &pos, Chunk.EnumCreateEntityType createType)
{
    auto i = (pos.getX() >> 4) - chunkX;
    auto j = (pos.getZ() >> 4) - chunkZ;
    return chunkArray[i][j].getTileEntity(pos, createType);
}

int32_t ChunkCache::getLightForExt(EnumSkyBlock type, BlockPos &pos)
{
    if (type == EnumSkyBlock::SKY && !world.provider.hasSkyLight())
    {
        return 0;
    }
    else if (pos.gety() >= 0 && pos.gety() < 256)
    {
        if (getBlockState(pos).useNeighborBrightness())
        {
            auto l = 0;
            for (auto enumfacing : EnumFacing::values())
            {
                auto k = getLightFor(type, pos.offset(enumfacing));
                if (k > l)
                {
                    l = k;
                }

                if (l >= 15)
                {
                    return l;
                }
            }

            return l;
        }
        else
        {
            auto l = (pos.getx() >> 4) - chunkX;
            auto j = (pos.getz() >> 4) - chunkZ;
            return chunkArray(l, j).getLightFor(type, pos);
        }
    }
    else
    {
        return type.defaultLightValue;
    }
}

int32_t ChunkCache::getLightFor(EnumSkyBlock type, BlockPos &pos)
{
    if (pos.gety() >= 0 && pos.gety() < 256)
    {
        auto i = (pos.getx() >> 4) - chunkX;
        auto j = (pos.getz() >> 4) - chunkZ;
        return chunkArray(i, j).getLightFor(type, pos);
    }
    else
    {
        return type.defaultLightValue;
    }
}

int32_t ChunkCache::getStrongPower(BlockPos &pos, EnumFacing direction)
{
    return getBlockState(pos)->getStrongPower(this, pos, direction);
}

WorldType &ChunkCache::getWorldType()
{
    return world.getWorldType();
}
