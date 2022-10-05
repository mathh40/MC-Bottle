#include "StructureComponent.h"

#include "EnumSkyBlock.h"
#include "math/BlockPos.h"
#include "math/MathHelper.h"

std::unique_ptr<NBTTagCompound> StructureComponent::createStructureBaseNBT()
{
    auto nbttagcompound = std::make_unique<NBTTagCompound>();
    nbttagcompound->setString("id", MapGenStructureIO.getStructureComponentName(this));
    nbttagcompound->setTag("BB", boundingBox.toNBTTagIntArray());
    EnumFacing enumfacing = getCoordBaseMode();
    nbttagcompound->setInteger("O", enumfacing == nullptr ? -1 : enumfacing.getHorizontalIndex());
    nbttagcompound->setInteger("GD", componentType);
    writeStructureToNBT(nbttagcompound.get());
    return nbttagcompound;
}

void StructureComponent::readStructureBaseNBT(World *worldIn, NBTTagCompound *tagCompound)
{
    if (tagCompound->hasKey("BB"))
    {
        boundingBox = StructureBoundingBox(tagCompound->getIntArray("BB"));
    }

    auto i = tagCompound->getInteger("O");
    setCoordBaseMode(i == -1 ? nullptr : EnumFacing::byHorizontalIndex(i));
    componentType = tagCompound->getInteger("GD");
    readStructureFromNBT(tagCompound, worldIn->getSaveHandler().getStructureTemplateManager());
}

void StructureComponent::buildComponent(StructureComponent *componentIn,
                                        const std::vector<StructureComponent *> &listIn, pcg32 &rand)
{
}

StructureComponent::StructureComponent(int32_t type) : componentType(type)
{
}

StructureBoundingBox StructureComponent::getBoundingBox() const
{
    return boundingBox;
}

int32_t StructureComponent::getComponentType() const
{
    return componentType;
}

StructureComponent *StructureComponent::findIntersecting(const std::vector<StructureComponent *> &listIn,
                                                         std::optional<StructureBoundingBox> boundingboxIn)
{
    auto var2 = listIn.begin();

    StructureComponent *structurecomponent;
    do
    {
        if (var2 == listIn.end())
        {
            return nullptr;
        }

        structurecomponent = *var2;
    } while (structurecomponent->getBoundingBox() == nullptr ||
             !structurecomponent->getBoundingBox().intersectsWith(boundingboxIn.value()));

    return structurecomponent;
}

void StructureComponent::offset(int32_t x, int32_t y, int32_t z)
{
    boundingBox.offset(x, y, z);
}

EnumFacing StructureComponent::getCoordBaseMode() const
{
    return coordBaseMode;
}

void StructureComponent::setCoordBaseMode(EnumFacing facing)
{
    coordBaseMode = facing;
    if (facing == fmt::internal::null)
    {
        rotation = Rotation::NONE;
        mirror   = Mirror::NONE;
    }
    else
    {
        switch (facing)
        {
        case EnumFacing::SOUTH:
            mirror   = Mirror::LEFT_RIGHT;
            rotation = Rotation::NONE;
            break;
        case EnumFacing::WEST:
            mirror   = Mirror::LEFT_RIGHT;
            rotation = Rotation::CLOCKWISE_90;
            break;
        case EnumFacing::EAST:
            mirror   = Mirror::NONE;
            rotation = Rotation::CLOCKWISE_90;
            break;
        default:
            mirror   = Mirror::NONE;
            rotation = Rotation::NONE;
        }
    }
}

StructureComponent::BlockSelector::BlockSelector()
{
    blockstate = Blocks::AIR.getDefaultState();
}

IBlockState *StructureComponent::BlockSelector::getBlockState() const
{
    return blockstate;
}

bool StructureComponent::isLiquidInStructureBoundingBox(World *worldIn, StructureBoundingBox boundingboxIn)
{
    auto i  = MathHelper::max(boundingBox.minX - 1, boundingboxIn.minX);
    auto j  = MathHelper::max(boundingBox.minY - 1, boundingboxIn.minY);
    auto k  = MathHelper::max(boundingBox.minZ - 1, boundingboxIn.minZ);
    auto l  = MathHelper::min(boundingBox.maxX + 1, boundingboxIn.maxX);
    auto i1 = MathHelper::min(boundingBox.maxY + 1, boundingboxIn.maxY);
    auto j1 = MathHelper::min(boundingBox.maxZ + 1, boundingboxIn.maxZ);
    MutableBlockPos blockpos$mutableblockpos;

    for (auto j2 = i; j2 <= l; ++j2)
    {
        for (auto l2 = k; l2 <= j1; ++l2)
        {
            if (worldIn->getBlockState(blockpos$mutableblockpos.setPos(j2, j, l2)).getMaterial().isLiquid())
            {
                return true;
            }

            if (worldIn->getBlockState(blockpos$mutableblockpos.setPos(j2, i1, l2)).getMaterial().isLiquid())
            {
                return true;
            }
        }
    }

    for (auto j2 = i; j2 <= l; ++j2)
    {
        for (auto l2 = j; l2 <= i1; ++l2)
        {
            if (worldIn->getBlockState(blockpos$mutableblockpos.setPos(j2, l2, k)).getMaterial().isLiquid())
            {
                return true;
            }

            if (worldIn->getBlockState(blockpos$mutableblockpos.setPos(j2, l2, j1)).getMaterial().isLiquid())
            {
                return true;
            }
        }
    }

    for (auto j2 = k; j2 <= j1; ++j2)
    {
        for (auto l2 = j; l2 <= i1; ++l2)
        {
            if (worldIn->getBlockState(blockpos$mutableblockpos.setPos(i, l2, j2)).getMaterial().isLiquid())
            {
                return true;
            }

            if (worldIn->getBlockState(blockpos$mutableblockpos.setPos(l, l2, j2)).getMaterial().isLiquid())
            {
                return true;
            }
        }
    }

    return false;
}

int32_t StructureComponent::getXWithOffset(int32_t x, int32_t z)
{
    EnumFacing enumfacing = getCoordBaseMode();
    if (enumfacing == fmt::internal::null)
    {
        return x;
    }
    else
    {
        switch (enumfacing)
        {
        case EnumFacing::NORTH:
        case EnumFacing::SOUTH:
            return boundingBox.minX + x;
        case EnumFacing::WEST:
            return boundingBox.maxX - z;
        case EnumFacing::EAST:
            return boundingBox.minX + z;
        default:
            return x;
        }
    }
}

int32_t StructureComponent::getYWithOffset(int32_t y)
{
    return getCoordBaseMode() == fmt::internal::null ? y : y + boundingBox.minY;
}

int32_t StructureComponent::getZWithOffset(int32_t x, int32_t z)
{
    EnumFacing enumfacing = getCoordBaseMode();
    if (enumfacing == fmt::internal::null)
    {
        return z;
    }
    else
    {
        switch (enumfacing)
        {
        case EnumFacing::NORTH:
            return boundingBox.maxZ - z;
        case EnumFacing::SOUTH:
            return boundingBox.minZ + z;
        case EnumFacing::WEST:
        case EnumFacing::EAST:
            return boundingBox.minZ + x;
        default:
            return z;
        }
    }
}

void StructureComponent::setBlockState(World *worldIn, IBlockState *blockstateIn, int32_t x, int32_t y, int32_t z,
                                       StructureBoundingBox boundingboxIn)
{
    BlockPos blockpos(getXWithOffset(x, z), getYWithOffset(y), getZWithOffset(x, z));
    if (boundingboxIn.isVecInside(blockpos))
    {
        if (mirror != Mirror::NONE)
        {
            blockstateIn = blockstateIn->withMirror(mirror);
        }

        if (rotation != Rotation::NONE)
        {
            blockstateIn = blockstateIn->withRotation(rotation);
        }

        worldIn->setBlockState(blockpos, blockstateIn, 2);
    }
}

IBlockState *StructureComponent::getBlockStateFromPos(World *worldIn, int32_t x, int32_t y, int32_t z,
                                                      StructureBoundingBox boundingboxIn)
{
    auto i = getXWithOffset(x, z);
    auto j = getYWithOffset(y);
    auto k = getZWithOffset(x, z);
    BlockPos blockpos(i, j, k);
    return !boundingboxIn.isVecInside(blockpos) ? Blocks::AIR::getDefaultState() : worldIn->getBlockState(blockpos);
}

int32_t StructureComponent::getSkyBrightness(World *worldIn, int32_t x, int32_t y, int32_t z,
                                             StructureBoundingBox boundingboxIn)
{
    auto i = getXWithOffset(x, z);
    auto j = getYWithOffset(y + 1);
    auto k = getZWithOffset(x, z);
    BlockPos blockpos(i, j, k);
    return !boundingboxIn.isVecInside(blockpos) ? EnumSkyBlock::SKY::defaultLightValue
                                                : worldIn->getLightFor(EnumSkyBlock::SKY, blockpos);
}

void StructureComponent::fillWithAir(World *worldIn, StructureBoundingBox structurebb, int32_t minX, int32_t minY,
                                     int32_t minZ, int32_t maxX, int32_t maxY, int32_t maxZ)
{
    for (auto i = minY; i <= maxY; ++i)
    {
        for (auto j = minX; j <= maxX; ++j)
        {
            for (auto k = minZ; k <= maxZ; ++k)
            {
                setBlockState(worldIn, Blocks::AIR::getDefaultState(), j, i, k, structurebb);
            }
        }
    }
}

void StructureComponent::fillWithBlocks(World *worldIn, StructureBoundingBox boundingboxIn, int32_t xMin, int32_t yMin,
                                        int32_t zMin, int32_t xMax, int32_t yMax, int32_t zMax,
                                        IBlockState *boundaryBlockState, IBlockState *insideBlockState,
                                        bool existingOnly)
{
    for (auto i = yMin; i <= yMax; ++i)
    {
        for (auto j = xMin; j <= xMax; ++j)
        {
            for (auto k = zMin; k <= zMax; ++k)
            {
                if (!existingOnly ||
                    getBlockStateFromPos(worldIn, j, i, k, boundingboxIn)->getMaterial() != Material::AIR)
                {
                    if (i != yMin && i != yMax && j != xMin && j != xMax && k != zMin && k != zMax)
                    {
                        setBlockState(worldIn, insideBlockState, j, i, k, boundingboxIn);
                    }
                    else
                    {
                        setBlockState(worldIn, boundaryBlockState, j, i, k, boundingboxIn);
                    }
                }
            }
        }
    }
}

void StructureComponent::fillWithRandomizedBlocks(World *worldIn, StructureBoundingBox boundingboxIn, int32_t minX,
                                                  int32_t minY, int32_t minZ, int32_t maxX, int32_t maxY, int32_t maxZ,
                                                  bool alwaysReplace, pcg32 &rand,
                                                  StructureComponent::BlockSelector blockselector)
{
    for (auto i = minY; i <= maxY; ++i)
    {
        for (auto j = minX; j <= maxX; ++j)
        {
            for (auto k = minZ; k <= maxZ; ++k)
            {
                if (!alwaysReplace ||
                    getBlockStateFromPos(worldIn, j, i, k, boundingboxIn)->getMaterial() != Material::AIR)
                {
                    blockselector.selectBlocks(
                        rand, j, i, k, i == minY || i == maxY || j == minX || j == maxX || k == minZ || k == maxZ);
                    setBlockState(worldIn, blockselector.getBlockState(), j, i, k, boundingboxIn);
                }
            }
        }
    }
}

void StructureComponent::generateMaybeBox(World *worldIn, StructureBoundingBox sbb, pcg32 &rand, float chance,
                                          int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2,
                                          IBlockState *edgeState, IBlockState *state, bool requireNonAir,
                                          int32_t requiredSkylight)
{
    for (auto i = y1; i <= y2; ++i)
    {
        for (auto j = x1; j <= x2; ++j)
        {
            for (auto k = z1; k <= z2; ++k)
            {
                if (MathHelper::nextFloat(rand) <= chance &&
                    (!requireNonAir || getBlockStateFromPos(worldIn, j, i, k, sbb)->getMaterial() != Material::AIR) &&
                    (requiredSkylight <= 0 || getSkyBrightness(worldIn, j, i, k, sbb) < requiredSkylight))
                {
                    if (i != y1 && i != y2 && j != x1 && j != x2 && k != z1 && k != z2)
                    {
                        setBlockState(worldIn, state, j, i, k, sbb);
                    }
                    else
                    {
                        setBlockState(worldIn, edgeState, j, i, k, sbb);
                    }
                }
            }
        }
    }
}

void StructureComponent::randomlyPlaceBlock(World *worldIn, StructureBoundingBox boundingboxIn, pcg32 &rand,
                                            float chance, int32_t x, int32_t y, int32_t z, IBlockState *blockstateIn)
{
    if (MathHelper::nextFloat(rand) < chance)
    {
        setBlockState(worldIn, blockstateIn, x, y, z, boundingboxIn);
    }
}

void StructureComponent::randomlyRareFillWithBlocks(World *worldIn, StructureBoundingBox boundingboxIn, int32_t minX,
                                                    int32_t minY, int32_t minZ, int32_t maxX, int32_t maxY,
                                                    int32_t maxZ, IBlockState *blockstateIn, bool excludeAir)
{
    const auto f  = (maxX - minX + 1);
    const auto f1 = (maxY - minY + 1);
    const auto f2 = (maxZ - minZ + 1);
    const auto f3 = minX + f / 2.0F;
    const auto f4 = minZ + f2 / 2.0F;

    for (auto i = minY; i <= maxY; ++i)
    {
        const auto f5 = (i - minY) / (float)f1;

        for (auto j = minX; j <= maxX; ++j)
        {
            const auto f6 = (j - f3) / (f * 0.5F);

            for (auto k = minZ; k <= maxZ; ++k)
            {
                const auto f7 = (k - f4) / (f2 * 0.5F);
                if (!excludeAir ||
                    getBlockStateFromPos(worldIn, j, i, k, boundingboxIn)->getMaterial() != Material::AIR)
                {
                    const auto f8 = f6 * f6 + f5 * f5 + f7 * f7;
                    if (f8 <= 1.05F)
                    {
                        setBlockState(worldIn, blockstateIn, j, i, k, boundingboxIn);
                    }
                }
            }
        }
    }
}

void StructureComponent::clearCurrentPositionBlocksUpwards(World *worldIn, int32_t x, int32_t y, int32_t z,
                                                           StructureBoundingBox structurebb)
{
    BlockPos blockpos(getXWithOffset(x, z), getYWithOffset(y), getZWithOffset(x, z));
    if (structurebb.isVecInside(blockpos))
    {
        while (!worldIn->isAirBlock(blockpos) && blockpos.gety() < 255)
        {
            worldIn->setBlockState(blockpos, Blocks::AIR::getDefaultState(), 2);
            blockpos = blockpos.up();
        }
    }
}

void StructureComponent::replaceAirAndLiquidDownwards(World *worldIn, IBlockState *blockstateIn, int32_t x, int32_t y,
                                                      int32_t z, StructureBoundingBox boundingboxIn)
{
    auto i = getXWithOffset(x, z);
    auto j = getYWithOffset(y);
    auto k = getZWithOffset(x, z);
    if (boundingboxIn.isVecInside(BlockPos(i, j, k)))
    {
        while ((worldIn->isAirBlock(BlockPos(i, j, k)) ||
                worldIn->getBlockState(BlockPos(i, j, k))->getMaterial().isLiquid()) &&
               j > 1)
        {
            worldIn->setBlockState(BlockPos(i, j, k), blockstateIn, 2);
            --j;
        }
    }
}

bool StructureComponent::generateChest(World *worldIn, StructureBoundingBox structurebb, pcg32 &randomIn, int32_t x,
                                       int32_t y, int32_t z, ResourceLocation loot)
{
    const BlockPos blockpos(getXWithOffset(x, z), getYWithOffset(y), getZWithOffset(x, z));
    return generateChest(worldIn, structurebb, randomIn, blockpos, loot, nullptr);
}

bool StructureComponent::generateChest(World *p_191080_1_, StructureBoundingBox p_191080_2_, pcg32 &p_191080_3_,
                                       BlockPos p_191080_4_, ResourceLocation p_191080_5_, IBlockState *p_191080_6_)
{
    if (p_191080_2_.isVecInside(p_191080_4_) && p_191080_1_->getBlockState(p_191080_4_).getBlock() != Blocks::CHEST)
    {
        if (p_191080_6_ == nullptr)
        {
            p_191080_6_ = Blocks::CHEST.correctFacing(p_191080_1_, p_191080_4_, Blocks::CHEST.getDefaultState());
        }

        p_191080_1_->setBlockState(p_191080_4_, p_191080_6_, 2);
        auto tileentity = p_191080_1_->getTileEntity(p_191080_4_);
        if (Util:: instanceof <TileEntityChest>(tileentity))
        {
            ((TileEntityChest)tileentity).setLootTable(p_191080_5_, MathHelper::nextLong(p_191080_3_));
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool StructureComponent::createDispenser(World *worldIn, StructureBoundingBox sbb, pcg32 &rand, int32_t x, int32_t y,
                                         int32_t z, EnumFacing facing, ResourceLocation lootTableIn)
{
    BlockPos blockpos(getXWithOffset(x, z), getYWithOffset(y), getZWithOffset(x, z));
    if (sbb.isVecInside(blockpos) && worldIn->getBlockState(blockpos).getBlock() != Blocks::DISPENSER)
    {
        setBlockState(worldIn, Blocks::DISPENSER.getDefaultState().withProperty(BlockDispenser::FACING, facing), x, y,
                      z, sbb);
        auto tileentity = worldIn->getTileEntity(blockpos);
        if (Util:: instanceof <TileEntityDispenser>(tileentity))
        {
            ((TileEntityDispenser)tileentity).setLootTable(lootTableIn, MathHelper::nextLong(rand));
        }

        return true;
    }
    else
    {
        return false;
    }
}

void StructureComponent::generateDoor(World *worldIn, StructureBoundingBox sbb, pcg32 &rand, int32_t x, int32_t y,
                                      int32_t z, EnumFacing facing, BlockDoor door)
{
    setBlockState(worldIn, door.getDefaultState().withProperty(BlockDoor::FACING, facing), x, y, z, sbb);
    setBlockState(worldIn,
                  door.getDefaultState()
                      .withProperty(BlockDoor::FACING, facing)
                      .withProperty(BlockDoor::HALF, BlockDoor::EnumDoorHalf::UPPER),
                  x, y + 1, z, sbb);
}
