#include "Teleporter.h"

#include "../util/Util.h"
#include "../util/math/BlockPos.h"
#include "../util/math/ChunkPos.h"
#include "../util/math/MathHelper.h"
#include "WorldServer.h"

Teleporter::Teleporter(WorldServer &worldIn) : world(worldIn), random(worldIn.getSeed())
{
}

void Teleporter::placeInPortal(Entity *entityIn, float rotationYaw)
{
    if (world.provider.getDimensionType().getId() != 1)
    {
        if (!placeInExistingPortal(entityIn, rotationYaw))
        {
            makePortal(entityIn);
            placeInExistingPortal(entityIn, rotationYaw);
        }
    }
    else
    {
        auto i = MathHelper::floor(entityIn.posX);
        auto j = MathHelper::floor(entityIn.posY) - 1;
        auto k = MathHelper::floor(entityIn.posZ);

        for (auto j1 = -2; j1 <= 2; ++j1)
        {
            for (auto k1 = -2; k1 <= 2; ++k1)
            {
                for (auto l1 = -1; l1 < 3; ++l1)
                {
                    auto i2   = i + k1 * 1 + j1 * 0;
                    auto j2   = j + l1;
                    auto k2   = k + k1 * 0 - j1 * 1;
                    bool flag = l1 < 0;
                    world.setBlockState(BlockPos(i2, j2, k2),
                                        flag ? Blocks.OBSIDIAN.getDefaultState() : Blocks.AIR.getDefaultState());
                }
            }
        }

        entityIn.setLocationAndAngles((double)i, (double)j, (double)k, entityIn.rotationYaw, 0.0F);
        entityIn.motionX = 0.0;
        entityIn.motionY = 0.0;
        entityIn.motionZ = 0.0;
    }
}

bool Teleporter::placeInExistingPortal(Entity *entityIn, float rotationYaw)
{
    double d0         = -1.0;
    auto j            = MathHelper::floor(entityIn.posX);
    auto k            = MathHelper::floor(entityIn.posZ);
    auto flag         = true;
    BlockPos blockpos = BlockPos::ORIGIN;
    auto l            = ChunkPos::asLong(j, k);
    auto ite          = destinationCoordinateCache.find(l);
    if (ite != destinationCoordinateCache.end())
    {
        PortalPosition portalposition = ite->second;
        d0                            = 0.0;
        blockpos                      = portalposition;
        portalposition.lastUpdateTime = world.getTotalWorldTime();
        flag                          = false;
    }
    else
    {
        BlockPos blockpos3(entityIn);

        for (auto i1 = -128; i1 <= 128; ++i1)
        {
            BlockPos blockpos2;
            for (auto j1 = -128; j1 <= 128; ++j1)
            {
                for (BlockPos blockpos1 = blockpos3.add(i1, world.getActualHeight() - 1 - blockpos3.gety(), j1);
                     blockpos1.gety() >= 0; blockpos1 = blockpos2)
                {
                    blockpos2 = blockpos1.down();
                    /*if (world.getBlockState(blockpos1).getBlock() == Blocks::PORTAL)
                    {
                        for (blockpos2 = blockpos1.down(); world.getBlockState(blockpos2).getBlock() == Blocks::PORTAL;
                    blockpos2 = blockpos2.down())
                        {
                            blockpos1 = blockpos2;
                        }
                    }*/
                    auto d1 = blockpos1::distanceSq(blockpos3);
                    if (d0 < 0.0 || d1 < d0)
                    {
                        d0       = d1;
                        blockpos = blockpos1;
                    }
                }
            }
        }
    }

    if (d0 >= 0.0)
    {
        if (flag)
        {
            destinationCoordinateCache.emplace(l, PortalPosition(blockpos, world.getTotalWorldTime()));
        }

        auto d5                         = blockpos.getx() + 0.5;
        auto d7                         = blockpos.getz() + 0.5;
        auto blockpattern$patternhelper = Blocks.PORTAL.createPatternHelper(world, blockpos);
        auto flag1 = blockpattern$patternhelper.getForwards().rotateY().getAxisDirection() == AxisDirection::NEGATIVE;
        auto d2    = blockpattern$patternhelper.getForwards().getAxis() == Axis::X
                         ? (double)blockpattern$patternhelper.getFrontTopLeft().getZ()
                         : blockpattern$patternhelper.getFrontTopLeft().getX();
        auto d6    = (blockpattern$patternhelper.getFrontTopLeft().getY() + 1) -
                  entityIn->getLastPortalVec().y * blockpattern$patternhelper.getHeight();
        if (flag1)
        {
            ++d2;
        }

        if (blockpattern$patternhelper.getForwards().getAxis() == Axis::X)
        {
            d7 = d2 + (1.0 - entityIn.getLastPortalVec().x) * blockpattern$patternhelper.getWidth() *
                          blockpattern$patternhelper.getForwards().rotateY().getAxisDirection().getOffset();
        }
        else
        {
            d5 = d2 + (1.0 - entityIn.getLastPortalVec().x) * blockpattern$patternhelper.getWidth() *
                          blockpattern$patternhelper.getForwards().rotateY().getAxisDirection().getOffset();
        }

        auto f  = 0.0F;
        auto f1 = 0.0F;
        auto f2 = 0.0F;
        auto f3 = 0.0F;
        if (blockpattern$patternhelper.getForwards().getOpposite() == entityIn->getTeleportDirection())
        {
            f  = 1.0F;
            f1 = 1.0F;
        }
        else if (blockpattern$patternhelper.getForwards().getOpposite() ==
                 entityIn->getTeleportDirection().getOpposite())
        {
            f  = -1.0F;
            f1 = -1.0F;
        }
        else if (blockpattern$patternhelper.getForwards().getOpposite() == entityIn->getTeleportDirection().rotateY())
        {
            f2 = 1.0F;
            f3 = -1.0F;
        }
        else
        {
            f2 = -1.0F;
            f3 = 1.0F;
        }

        auto d3              = entityIn.motionX;
        auto d4              = entityIn.motionZ;
        entityIn.motionX     = d3 * f + d4 * f3;
        entityIn.motionZ     = d3 * f2 + d4 * f1;
        entityIn.rotationYaw = rotationYaw - (entityIn.getTeleportDirection().getOpposite().getHorizontalIndex() * 90) +
                               (blockpattern$patternhelper.getForwards().getHorizontalIndex() * 90);
        if (Util:: instanceof <EntityPlayerMP>(entityIn))
        {
            ((EntityPlayerMP)entityIn)
                .connection.setPlayerLocation(d5, d6, d7, entityIn.rotationYaw, entityIn.rotationPitch);
        }
        else
        {
            entityIn.setLocationAndAngles(d5, d6, d7, entityIn.rotationYaw, entityIn.rotationPitch);
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool Teleporter::makePortal(Entity *entityIn)
{
    auto d0 = -1.0;
    auto j  = MathHelper::floor(entityIn.posX);
    auto k  = MathHelper::floor(entityIn.posY);
    auto l  = MathHelper::floor(entityIn.posZ);
    auto i1 = j;
    auto j1 = k;
    auto k1 = l;
    auto l1 = 0;
    auto i2 = random(4);
    MutableBlockPos mutableblockpos;
    for (auto l5 = j - 16; l5 <= j + 16; ++l5)
    {
        auto d3 = l5 + 0.5 - entityIn.posX;

        for (auto j6 = l - 16; j6 <= l + 16; ++j6)
        {
            auto d4 = j6 + 0.5 - entityIn.posZ;

        label291:
            for (auto i7 = world.getActualHeight() - 1; i7 >= 0; --i7)
            {
                if (world.isAirBlock(mutableblockpos.setPos(l5, i7, j6)))
                {
                    while (i7 > 0 && world.isAirBlock(mutableblockpos.setPos(l5, i7 - 1, j6)))
                    {
                        --i7;
                    }

                    for (auto k7 = i2; k7 < i2 + 4; ++k7)
                    {
                        auto j8 = k7 % 2;
                        auto j9 = 1 - j8;
                        if (k7 % 4 >= 2)
                        {
                            j8 = -j8;
                            j9 = -j9;
                        }

                        for (auto j10 = 0; j10 < 3; ++j10)
                        {
                            for (auto j11 = 0; j11 < 4; ++j11)
                            {
                                for (auto j12 = -1; j12 < 4; ++j12)
                                {
                                    auto i13 = l5 + (j11 - 1) * j8 + j10 * j9;
                                    auto j13 = i7 + j12;
                                    auto k5  = j6 + (j11 - 1) * j9 - j10 * j8;
                                    mutableblockpos.setPos(i13, j13, k5);
                                    if (j12 < 0 && !world.getBlockState(mutableblockpos).getMaterial().isSolid() ||
                                        j12 >= 0 && !world.isAirBlock(mutableblockpos))
                                    {
                                        continue label291;
                                    }
                                }
                            }
                        }

                        auto d6 = i7 + 0.5 - entityIn.posY;
                        auto d8 = d3 * d3 + d6 * d6 + d4 * d4;
                        if (d0 < 0.0 || d8 < d0)
                        {
                            d0 = d8;
                            i1 = l5;
                            j1 = i7;
                            k1 = j6;
                            l1 = k7 % 4;
                        }
                    }
                }
            }
        }
    }

    if (d0 < 0.0)
    {
        for (auto l5 = j - 16; l5 <= j + 16; ++l5)
        {
            auto d3 = l5 + 0.5 - entityIn.posX;

            for (auto j6 = l - 16; j6 <= l + 16; ++j6)
            {
                auto d4 = j6 + 0.5 - entityIn.posZ;

            label229:
                for (auto i7 = world.getActualHeight() - 1; i7 >= 0; --i7)
                {
                    if (world.isAirBlock(mutableblockpos.setPos(l5, i7, j6)))
                    {
                        while (i7 > 0 && world.isAirBlock(mutableblockpos.setPos(l5, i7 - 1, j6)))
                        {
                            --i7;
                        }

                        for (auto k7 = i2; k7 < i2 + 2; ++k7)
                        {
                            auto j8 = k7 % 2;
                            auto j9 = 1 - j8;

                            for (auto j10 = 0; j10 < 4; ++j10)
                            {
                                for (auto j11 = -1; j11 < 4; ++j11)
                                {
                                    auto j12 = l5 + (j10 - 1) * j8;
                                    auto i13 = i7 + j11;
                                    auto j13 = j6 + (j10 - 1) * j9;
                                    mutableblockpos.setPos(j12, i13, j13);
                                    if (j11 < 0 && !world.getBlockState(mutableblockpos).getMaterial().isSolid() ||
                                        j11 >= 0 && !world.isAirBlock(mutableblockpos))
                                    {
                                        continue label229;
                                    }
                                }
                            }

                            auto d6 = i7 + 0.5 - entityIn.posY;
                            auto d8 = d3 * d3 + d6 * d6 + d4 * d4;
                            if (d0 < 0.0 || d8 < d0)
                            {
                                d0 = d8;
                                i1 = l5;
                                j1 = i7;
                                k1 = j6;
                                l1 = k7 % 2;
                            }
                        }
                    }
                }
            }
        }
    }

    auto l5 = i1;
    auto k2 = j1;
    auto k6 = k1;
    auto j6 = l1 % 2;
    auto i3 = 1 - j6;
    if (l1 % 4 >= 2)
    {
        j6 = -j6;
        i3 = -i3;
    }

    if (d0 < 0.0)
    {
        j1 = MathHelper::clamp(j1, 70, world.getActualHeight() - 10);
        k2 = j1;

        for (auto j7 = -1; j7 <= 1; ++j7)
        {
            for (auto i7 = 1; i7 < 3; ++i7)
            {
                for (auto k7 = -1; k7 < 3; ++k7)
                {
                    auto j8   = l5 + (i7 - 1) * j6 + j7 * i3;
                    auto j9   = k2 + k7;
                    auto j10  = k6 + (i7 - 1) * i3 - j7 * j6;
                    auto flag = k7 < 0;
                    world.setBlockState(new BlockPos(j8, j9, j10),
                                        flag ? Blocks::OBSIDIAN.getDefaultState() : Blocks::AIR.getDefaultState());
                }
            }
        }
    }

    auto iblockstate = Blocks::PORTAL.getDefaultState().withProperty(BlockPortal::AXIS, j6 == 0 ? Axis::Z : Axis::X);

    for (auto i7 = 0; i7 < 4; ++i7)
    {
        for (auto k7 = 0; k7 < 4; ++k7)
        {
            for (auto j8 = -1; j8 < 4; ++j8)
            {
                auto j9    = l5 + (k7 - 1) * j6;
                auto j10   = k2 + j8;
                auto j11   = k6 + (k7 - 1) * i3;
                auto flag1 = k7 == 0 || k7 == 3 || j8 == -1 || j8 == 3;
                world.setBlockState(new BlockPos(j9, j10, j11),
                                    flag1 ? Blocks::OBSIDIAN.getDefaultState() : iblockstate, 2);
            }
        }

        for (auto k7 = 0; k7 < 4; ++k7)
        {
            for (auto j8 = -1; j8 < 4; ++j8)
            {
                auto j9  = l5 + (k7 - 1) * j6;
                auto j10 = k2 + j8;
                auto j11 = k6 + (k7 - 1) * i3;
                BlockPos blockpos(j9, j10, j11);
                world.notifyNeighborsOfStateChange(blockpos, world.getBlockState(blockpos).getBlock(), false);
            }
        }
    }

    return true;
}

void Teleporter::removeStalePortalLocations(int64_t worldTime)
{
    if (worldTime % 100 == 0)
    {
        int64_t i = worldTime - 300;

        auto objectiterator = destinationCoordinateCache.begin();

        while (true)
        {
            PortalPosition portalposition;
            do
            {
                if (objectiterator == destinationCoordinateCache.end())
                {
                    return;
                }

                portalposition = objectiterator->second;
            } while (portalposition.lastUpdateTime >= i);

            destinationCoordinateCache.erase(objectiterator);
        }
    }
}
