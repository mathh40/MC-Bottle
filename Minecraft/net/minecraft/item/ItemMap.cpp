#include "ItemMap.h"

#include "../world/World.h"
#include "../world/storage/MapData.h"
#include "../world/storage/WorldInfo.h"
#include "ItemStack.h"
#include "text/translation/I18n.h"

#include <unordered_set>

ItemStack ItemMap::setupNewMap(World *worldIn, double worldX, double worldZ, uint8_t scale, bool trackingPosition,
                               bool unlimitedTracking)
{
    ItemStack itemstack = ItemStack(Items::FILLED_MAP, 1, worldIn->getUniqueDataId("map"));
    auto s              = "map_" + itemstack.getMetadata();
    MapData mapdata     = MapData(s);
    worldIn->setData(s, &mapdata);
    mapdata.scale = scale;
    mapdata.calculateMapCenter(worldX, worldZ, mapdata.scale);
    mapdata.dimension         = worldIn->provider->getDimensionType().getId();
    mapdata.trackingPosition  = trackingPosition;
    mapdata.unlimitedTracking = unlimitedTracking;
    mapdata.markDirty();
    return itemstack;
}

MapData ItemMap::loadMapData(int32_t mapId, World *worldIn)
{
    auto s = "map_" + mapId;
    return worldIn->loadData<MapData>(s);
}

MapData ItemMap::getMapData(ItemStack stack, World *worldIn)
{
    auto s          = "map_" + stack.getMetadata();
    MapData mapdata = worldIn->loadData<MapData>(s);
    if (mapdata == nullptr && !worldIn->isRemote)
    {
        stack.setItemDamage(worldIn->getUniqueDataId("map"));
        s             = "map_" + stack.getMetadata();
        mapdata       = MapData(s);
        mapdata.scale = 3;
        mapdata.calculateMapCenter((double)worldIn->getWorldInfo().getSpawnX(),
                                   (double)worldIn->getWorldInfo().getSpawnZ(), mapdata.scale);
        mapdata.dimension = worldIn->provider.getDimensionType().getId();
        mapdata.markDirty();
        worldIn->setData(s, mapdata);
    }

    return mapdata;
}

void ItemMap::updateMapData(World *worldIn, Entity *viewer, MapData data)
{
    if (worldIn->provider->getDimensionType().getId() == data.dimension && Util:: instanceof <EntityPlayer>(viewer))
    {
        auto i  = 1 << data.scale;
        auto j  = data.xCenter;
        auto k  = data.zCenter;
        auto l  = MathHelper::floor(viewer->posX - (double)j) / i + 64;
        auto i1 = MathHelper::floor(viewer->posZ - (double)k) / i + 64;
        auto j1 = 128 / i;
        if (worldIn->provider->isNether())
        {
            j1 /= 2;
        }

        MapData::MapInfo mapdata$mapinfo = data.getMapInfo((EntityPlayer *)viewer);
        ++mapdata$mapinfo.step;
        bool flag = false;

        for (auto k1 = l - j1 + 1; k1 < l + j1; ++k1)
        {
            if ((k1 & 15) == (mapdata$mapinfo.step & 15) || flag)
            {
                flag      = false;
                double d0 = 0.0;

                for (auto l1 = i1 - j1 - 1; l1 < i1 + j1; ++l1)
                {
                    if (k1 >= 0 && l1 >= -1 && k1 < 128 && l1 < 128)
                    {
                        auto i2    = k1 - l;
                        auto j2    = l1 - i1;
                        bool flag1 = i2 * i2 + j2 * j2 > (j1 - 2) * (j1 - 2);
                        auto k2    = (j / i + k1 - 64) * i;
                        auto l2    = (k / i + l1 - 64) * i;
                        std::unordered_multiset<MapColor, int32_t> multiset;
                        Chunk chunk = worldIn->getChunk(BlockPos(k2, 0, l2));
                        if (!chunk.isEmpty())
                        {
                            auto i3   = k2 & 15;
                            auto j3   = l2 & 15;
                            auto k3   = 0;
                            double d1 = 0.0;
                            if (worldIn->provider->isNether())
                            {
                                auto l3 = k2 + l2 * 231871;
                                l3      = l3 * l3 * 31287121 + l3 * 11;
                                if ((l3 >> 20 & 1) == 0)
                                {
                                    multiset.emplace(Blocks::DIRT::getDefaultState()
                                                         .withProperty(BlockDirt::VARIANT, BlockDirt::DirtType::DIRT)
                                                         .getMapColor(worldIn, BlockPos::ORIGIN),
                                                     10);
                                }
                                else
                                {
                                    multiset.emplace(Blocks.STONE.getDefaultState()
                                                         .withProperty(BlockStone.VARIANT, BlockStone.EnumType.STONE)
                                                         .getMapColor(worldIn, BlockPos::ORIGIN),
                                                     100);
                                }

                                d1 = 100.0;
                            }
                            else
                            {
                                MutableBlockPos blockpos$mutableblockpos;

                                for (auto i4 = 0; i4 < i; ++i4)
                                {
                                    for (auto j4 = 0; j4 < i; ++j4)
                                    {
                                        auto k4                  = chunk.getHeightValue(i4 + i3, j4 + j3) + 1;
                                        IBlockState *iblockstate = Blocks::AIR.getDefaultState();
                                        if (k4 <= 1)
                                        {
                                            iblockstate = Blocks::BEDROCK.getDefaultState();
                                        }
                                        else
                                        {
                                            do
                                            {
                                                --k4;
                                                iblockstate = chunk.getBlockState(i4 + i3, k4, j4 + j3);
                                                blockpos$mutableblockpos.setPos((chunk.x << 4) + i4 + i3, k4,
                                                                                (chunk.z << 4) + j4 + j3);
                                            } while (iblockstate->getMapColor(worldIn, blockpos$mutableblockpos) ==
                                                         MapColor::AIR &&
                                                     k4 > 0);

                                            if (k4 > 0 && iblockstate->getMaterial()->isLiquid())
                                            {
                                                auto l4 = k4 - 1;

                                                IBlockState *iblockstate1 = nullptr;
                                                do
                                                {
                                                    iblockstate1 = chunk.getBlockState(i4 + i3, l4--, j4 + j3);
                                                    ++k3;
                                                } while (l4 > 0 && iblockstate1->getMaterial()->isLiquid());
                                            }
                                        }

                                        d1 += (double)k4 / (double)(i * i);
                                        multiset.emplace(iblockstate->getMapColor(worldIn, blockpos$mutableblockpos));
                                    }
                                }
                            }

                            k3 /= i * i;
                            auto d2 = (d1 - d0) * 4.0 / (double)(i + 4) + ((double)(k1 + l1 & 1) - 0.5) * 0.4;
                            auto i5 = 1;
                            if (d2 > 0.6)
                            {
                                i5 = 2;
                            }

                            if (d2 < -0.6)
                            {
                                i5 = 0;
                            }

                            MapColor mapcolor =
                                (MapColor)Iterables.getFirst(Multisets.copyHighestCountFirst(multiset), MapColor::AIR);
                            if (mapcolor == MapColor::WATER)
                            {
                                d2 = (double)k3 * 0.1 + (double)(k1 + l1 & 1) * 0.2;
                                i5 = 1;
                                if (d2 < 0.5)
                                {
                                    i5 = 2;
                                }

                                if (d2 > 0.9)
                                {
                                    i5 = 0;
                                }
                            }

                            d0 = d1;
                            if (l1 >= 0 && i2 * i2 + j2 * j2 < j1 * j1 && (!flag1 || (k1 + l1 & 1) != 0))
                            {
                                auto b0 = data.colors[k1 + l1 * 128];
                                auto b1 = mapcolor.colorIndex * 4 + i5;
                                if (b0 != b1)
                                {
                                    data.colors[k1 + l1 * 128] = b1;
                                    data.updateMapData(k1, l1);
                                    flag = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ItemMap::renderBiomePreviewMap(World *worldIn, ItemStack map)
{
    if (map.getItem() == Items::FILLED_MAP)
    {
        MapData mapdata = Items.FILLED_MAP.getMapData(map, worldIn);
        if (mapdata != nullptr && worldIn->provider->getDimensionType().getId() == mapdata.dimension)
        {
            auto i = 1 << mapdata.scale;
            auto j = mapdata.xCenter;
            auto k = mapdata.zCenter;
            std::vector<Biome *> abiome;
            abiome = worldIn->getBiomeProvider().getBiomes(abiome, (j / i - 64) * i, (k / i - 64) * i, 128 * i, 128 * i,
                                                           false);

            for (auto l = 0; l < 128; ++l)
            {
                for (auto i1 = 0; i1 < 128; ++i1)
                {
                    auto j1           = l * i;
                    auto k1           = i1 * i;
                    auto biome        = abiome[j1 + k1 * 128 * i];
                    MapColor mapcolor = MapColor::AIR;
                    auto l1           = 3;
                    auto i2           = 8;
                    if (l > 0 && i1 > 0 && l < 127 && i1 < 127)
                    {
                        if (abiome[(l - 1) * i + (i1 - 1) * i * 128 * i]->getBaseHeight() >= 0.0F)
                        {
                            --i2;
                        }

                        if (abiome[(l - 1) * i + (i1 + 1) * i * 128 * i]->getBaseHeight() >= 0.0F)
                        {
                            --i2;
                        }

                        if (abiome[(l - 1) * i + i1 * i * 128 * i]->getBaseHeight() >= 0.0F)
                        {
                            --i2;
                        }

                        if (abiome[(l + 1) * i + (i1 - 1) * i * 128 * i]->getBaseHeight() >= 0.0F)
                        {
                            --i2;
                        }

                        if (abiome[(l + 1) * i + (i1 + 1) * i * 128 * i]->getBaseHeight() >= 0.0F)
                        {
                            --i2;
                        }

                        if (abiome[(l + 1) * i + i1 * i * 128 * i]->getBaseHeight() >= 0.0F)
                        {
                            --i2;
                        }

                        if (abiome[l * i + (i1 - 1) * i * 128 * i]->getBaseHeight() >= 0.0F)
                        {
                            --i2;
                        }

                        if (abiome[l * i + (i1 + 1) * i * 128 * i]->getBaseHeight() >= 0.0F)
                        {
                            --i2;
                        }

                        if (biome->getBaseHeight() < 0.0F)
                        {
                            mapcolor = MapColor::ADOBE;
                            if (i2 > 7 && i1 % 2 == 0)
                            {
                                l1 = (l + (int)(MathHelper::sin((float)i1 + 0.0F) * 7.0F)) / 8 % 5;
                                if (l1 == 3)
                                {
                                    l1 = 1;
                                }
                                else if (l1 == 4)
                                {
                                    l1 = 0;
                                }
                            }
                            else if (i2 > 7)
                            {
                                mapcolor = MapColor::AIR;
                            }
                            else if (i2 > 5)
                            {
                                l1 = 1;
                            }
                            else if (i2 > 3)
                            {
                                l1 = 0;
                            }
                            else if (i2 > 1)
                            {
                                l1 = 0;
                            }
                        }
                        else if (i2 > 0)
                        {
                            mapcolor = MapColor::BROWN;
                            if (i2 > 3)
                            {
                                l1 = 1;
                            }
                            else
                            {
                                l1 = 3;
                            }
                        }
                    }

                    if (mapcolor != MapColor::AIR)
                    {
                        mapdata.colors[l + i1 * 128] = mapcolor.colorIndex * 4 + l1;
                        mapdata.updateMapData(l, i1);
                    }
                }
            }
        }
    }
}

void ItemMap::onUpdate(ItemStack stack, World *worldIn, Entity *entityIn, int32_t itemSlot, bool isSelected)
{
    if (!worldIn->isRemote)
    {
        MapData mapdata = getMapData(stack, worldIn);
        if (Util:: instanceof <EntityPlayer>(entityIn))
        {
            EntityPlayer *entityplayer = (EntityPlayer *)entityIn;
            mapdata.updateVisiblePlayers(entityplayer, stack);
        }

        if (isSelected || Util:: instanceof <EntityPlayer>(entityIn) &&
                                                ((EntityPlayer *)entityIn).getHeldItemOffhand() == stack)
        {
            updateMapData(worldIn, entityIn, mapdata);
        }
    }
}

Packet ItemMap::createMapDataPacket(ItemStack stack, World *worldIn, EntityPlayer *player)
{
    return getMapData(stack, worldIn).getMapPacket(stack, worldIn, player);
}

void ItemMap::onCreated(ItemStack stack, World *worldIn, EntityPlayer *playerIn)
{
    NBTTagCompound *nbttagcompound = stack.getTagCompound();
    if (nbttagcompound != nullptr)
    {
        if (nbttagcompound->hasKey("map_scale_direction", 99))
        {
            scaleMap(stack, worldIn, nbttagcompound->getInteger("map_scale_direction"));
            nbttagcompound->removeTag("map_scale_direction");
        }
        else if (nbttagcompound->getBoolean("map_tracking_position"))
        {
            enableMapTracking(stack, worldIn);
            nbttagcompound->removeTag("map_tracking_position");
        }
    }
}

int32_t ItemMap::getColor(ItemStack p_190907_0_)
{
    NBTTagCompound *nbttagcompound = p_190907_0_.getSubCompound("display");
    if (nbttagcompound != nullptr && nbttagcompound->hasKey("MapColor", 99))
    {
        int32_t i = nbttagcompound->getInteger("MapColor");
        return -16777216 | i & 16777215;
    }
    else
    {
        return -12173266;
    }
}

ItemMap::ItemMap()
{
    setHasSubtypes(true);
}

void ItemMap::scaleMap(ItemStack p_185063_0_, World *p_185063_1_, int32_t p_185063_2_)
{
    MapData mapdata = Items::FILLED_MAP.getMapData(p_185063_0_, p_185063_1_);
    p_185063_0_.setItemDamage(p_185063_1_->getUniqueDataId("map"));
    MapData mapdata1 = MapData("map_" + p_185063_0_.getMetadata());
    if (mapdata != nullptr)
    {
        mapdata1.scale            = MathHelper::clamp(mapdata.scale + p_185063_2_, 0, 4);
        mapdata1.trackingPosition = mapdata.trackingPosition;
        mapdata1.calculateMapCenter((double)mapdata.xCenter, (double)mapdata.zCenter, mapdata1.scale);
        mapdata1.dimension = mapdata.dimension;
        mapdata1.markDirty();
        p_185063_1_->setData("map_" + p_185063_0_.getMetadata(), mapdata1);
    }
}

void ItemMap::enableMapTracking(ItemStack p_185064_0_, World *p_185064_1_)
{
    MapData mapdata = Items::FILLED_MAP.getMapData(p_185064_0_, p_185064_1_);
    p_185064_0_.setItemDamage(p_185064_1_->getUniqueDataId("map"));
    MapData mapdata1          = MapData("map_" + p_185064_0_.getMetadata());
    mapdata1.trackingPosition = true;
    if (mapdata != nullptr)
    {
        mapdata1.xCenter   = mapdata.xCenter;
        mapdata1.zCenter   = mapdata.zCenter;
        mapdata1.scale     = mapdata.scale;
        mapdata1.dimension = mapdata.dimension;
        mapdata1.markDirty();
        p_185064_1_->setData("map_" + p_185064_0_.getMetadata(), mapdata1);
    }
}

void ItemMap::addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip, ITooltipFlag *flagIn)
{
    if (flagIn->isAdvanced())
    {
        MapData mapdata = worldIn == nullptr ? nullptr : getMapData(stack, worldIn);
        if (mapdata != nullptr)
        {
            tooltip.emplace_back(I18n::translateToLocalFormatted("filled_map.scale", 1 << mapdata.scale));
            tooltip.emplace_back(I18n::translateToLocalFormatted("filled_map.level", mapdata.scale, 4));
        }
        else
        {
            tooltip.emplace_back(I18n::translateToLocal("filled_map.unknown"));
        }
    }
}
