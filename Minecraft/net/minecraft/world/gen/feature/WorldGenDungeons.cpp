#include "WorldGenDungeons.h"

#include "World.h"
#include "material/Material.h"
#include "math/BlockPos.h"
#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> WorldGenDungeons::LOGGER = spdlog::get("Minecraft")->clone("WorldGenDungeons");

bool WorldGenDungeons::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    auto j  = rand(2) + 2;
    auto k  = -j - 1;
    auto l  = j + 1;
    auto k1 = rand(2) + 2;
    auto l1 = -k1 - 1;
    auto i2 = k1 + 1;
    auto j2 = 0;

    BlockPos blockpos1;
    for (auto k3 = k; k3 <= l; ++k3)
    {
        for (auto i4 = -1; i4 <= 4; ++i4)
        {
            for (auto k4 = l1; k4 <= i2; ++k4)
            {
                blockpos1         = position.add(k3, i4, k4);
                Material material = worldIn->getBlockState(blockpos1).getMaterial();
                auto flag         = material.isSolid();
                if (i4 == -1 && !flag)
                {
                    return false;
                }

                if (i4 == 4 && !flag)
                {
                    return false;
                }

                if ((k3 == k || k3 == l || k4 == l1 || k4 == i2) && i4 == 0 && worldIn->isAirBlock(blockpos1) &&
                    worldIn->isAirBlock(blockpos1.up()))
                {
                    ++j2;
                }
            }
        }
    }

    if (j2 >= 1 && j2 <= 5)
    {
        for (auto k3 = k; k3 <= l; ++k3)
        {
            for (auto i4 = 3; i4 >= -1; --i4)
            {
                for (auto k4 = l1; k4 <= i2; ++k4)
                {
                    blockpos1 = position.add(k3, i4, k4);
                    if (k3 != k && i4 != -1 && k4 != l1 && k3 != l && i4 != 4 && k4 != i2)
                    {
                        if (worldIn->getBlockState(blockpos1).getBlock() != Blocks::CHEST)
                        {
                            worldIn->setBlockToAir(blockpos1);
                        }
                    }
                    else if (blockpos1.gety() >= 0 && !worldIn->getBlockState(blockpos1.down()).getMaterial().isSolid())
                    {
                        worldIn.setBlockToAir(blockpos1);
                    }
                    else if (worldIn.getBlockState(blockpos1).getMaterial().isSolid() &&
                             worldIn.getBlockState(blockpos1).getBlock() != Blocks.CHEST)
                    {
                        if (i4 == -1 && rand.nextInt(4) != 0)
                        {
                            worldIn.setBlockState(blockpos1, Blocks.MOSSY_COBBLESTONE.getDefaultState(), 2);
                        }
                        else
                        {
                            worldIn.setBlockState(blockpos1, Blocks.COBBLESTONE.getDefaultState(), 2);
                        }
                    }
                }
            }
        }

        for (auto k3 = 0; k3 < 2; ++k3)
        {
            for (auto i4 = 0; i4 < 3; ++i4)
            {
                auto k4 = position.getx() + rand(j * 2 + 1) - j;
                auto i5 = position.gety();
                auto j5 = position.getz() + rand(k1 * 2 + 1) - k1;
                BlockPos blockpos2(k4, i5, j5);
                if (worldIn->isAirBlock(blockpos2))
                {
                    auto j3        = 0;
                    Iterator var21 = EnumFacing.Plane.HORIZONTAL.iterator();

                    for (auto enumfacing : EnumFacing::Plane::HORIZONTAL)
                    {
                        if (worldIn->getBlockState(blockpos2.offset(enumfacing)).getMaterial().isSolid())
                        {
                            ++j3;
                        }
                    }

                    if (j3 == 1)
                    {
                        worldIn.setBlockState(
                            blockpos2, Blocks.CHEST.correctFacing(worldIn, blockpos2, Blocks.CHEST.getDefaultState()),
                            2);
                        auto tileentity1 = worldIn.getTileEntity(blockpos2);
                        if (Util:: instanceof <TileEntityChest>(tileentity1))
                        {
                            ((TileEntityChest)tileentity1)
                                .setLootTable(LootTableList.CHESTS_SIMPLE_DUNGEON, MathHelper::nextLong(rand));
                        }
                        break;
                    }
                }
            }
        }

        worldIn.setBlockState(position, Blocks.MOB_SPAWNER.getDefaultState(), 2);
        auto tileentity = worldIn.getTileEntity(position);
        if (Util:: instanceof <TileEntityMobSpawner>(tileentity))
        {
            ((TileEntityMobSpawner)tileentity).getSpawnerBaseLogic().setEntityId(pickMobSpawner(rand));
        }
        else
        {
            LOGGER->error("Failed to fetch mob spawner entity at ({}, {}, {})", position.getx(), position.gety(),
                          position.getz());
        }

        return true;
    }
    else
    {
        return false;
    }
}

ResourceLocation WorldGenDungeons::pickMobSpawner(pcg32 &rand)
{
    return SPAWNERTYPES[rand(SPAWNERTYPES.size())];
}
