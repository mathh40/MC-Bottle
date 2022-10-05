#include "StructureMineshaftPieces.h"

#include "MapGenStructureIO.h"

void StructureMineshaftPieces::registerStructurePieces()
{
    MapGenStructureIO::registerStructureComponent(StructureMineshaftPieces::Corridor.class, "MSCorridor");
    MapGenStructureIO::registerStructureComponent(StructureMineshaftPieces::Cross.class, "MSCrossing");
    MapGenStructureIO::registerStructureComponent(StructureMineshaftPieces::Room.class, "MSRoom");
    MapGenStructureIO::registerStructureComponent(StructureMineshaftPieces::Stairs.class, "MSStairs");
}

StructureMineshaftPieces::Peice::Peice(int32_t p_i47138_1_, MapGenMineshaft::Type p_i47138_2_)
    : StructureComponent(p_i47138_1_)
{
    mineShaftType = p_i47138_2_;
}

void StructureMineshaftPieces::Peice::writeStructureToNBT(NBTTagCompound *tagCompound)
{
    tagCompound->setInteger("MST", mineShaftType.ordinal());
}

void StructureMineshaftPieces::Peice::readStructureFromNBT(NBTTagCompound *tagCompound,
                                                           const TemplateManager &p_143011_2_)
{
    mineShaftType = MapGenMineshaft::Type::byId(tagCompound->getInteger("MST"));
}

bool StructureMineshaftPieces::Peice::isSupportingBox(World *p_189918_1_, StructureBoundingBox p_189918_2_,
                                                      int32_t p_189918_3_, int32_t p_189918_4_, int32_t p_189918_5_,
                                                      int32_t p_189918_6_)
{
    for (int32_t i = p_189918_3_; i <= p_189918_4_; ++i)
    {
        if (getBlockStateFromPos(p_189918_1_, i, p_189918_5_ + 1, p_189918_6_, p_189918_2_)->getMaterial() ==
            Material::AIR)
        {
            return false;
        }
    }

    return true;
}

IBlockState *StructureMineshaftPieces::Peice::getPlanksBlock()
{
    switch (mineShaftType)
    {
    case MapGenMineshaft::NORMAL:
    default:
        return Blocks::PLANKS.getDefaultState();
    case MapGenMineshaft::MESA:
        return Blocks::PLANKS.getDefaultState().withProperty(BlockPlanks::VARIANT, BlockPlanks::EnumType::DARK_OAK);
    }
}

IBlockState *StructureMineshaftPieces::Peice::getFenceBlock()
{
    switch (mineShaftType)
    {
    case MapGenMineshaft::NORMAL:
    default:
        return Blocks::OAK_FENCE.getDefaultState();
    case MapGenMineshaft::MESA:
        return Blocks::DARK_OAK_FENCE.getDefaultState();
    }
}

StructureMineshaftPieces::Stairs::Stairs(int32_t p_i47136_1_, pcg32 &p_i47136_2_, StructureBoundingBox p_i47136_3_,
                                         EnumFacing p_i47136_4_, MapGenMineshaft::Type p_i47136_5_)
    : StructureMineshaftPieces::Peice(p_i47136_1_, p_i47136_5_)
{
    setCoordBaseMode(p_i47136_4_);
    boundingBox = p_i47136_3_;
}

void StructureMineshaftPieces::Stairs::buildComponent(StructureComponent *componentIn, std::vector<> listIn,
                                                      pcg32 &rand)
{
    auto i                = getComponentType();
    EnumFacing enumfacing = getCoordBaseMode();
    if (enumfacing != nullptr)
    {
        switch (enumfacing)
        {
        case EnumFacing::NORTH:
        default:
            StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX, boundingBox.minY,
                                                          boundingBox.minZ - 1, EnumFacing::NORTH, i);
            break;
        case EnumFacing::SOUTH:
            StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX, boundingBox.minY,
                                                          boundingBox.maxZ + 1, EnumFacing::SOUTH, i);
            break;
        case EnumFacing::WEST:
            StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX - 1,
                                                          boundingBox.minY, boundingBox.minZ, EnumFacing::WEST, i);
            break;
        case EnumFacing::EAST:
            StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX + 1,
                                                          boundingBox.minY, boundingBox.minZ, EnumFacing::EAST, i);
        }
    }
}

bool StructureMineshaftPieces::Stairs::addComponentParts(World *worldIn, pcg32 &randomIn,
                                                         StructureBoundingBox structureBoundingBoxIn)
{
    if (isLiquidInStructureBoundingBox(worldIn, structureBoundingBoxIn))
    {
        return false;
    }
    else
    {
        fillWithBlocks(worldIn, structureBoundingBoxIn, 0, 5, 0, 2, 7, 1, Blocks::AIR.getDefaultState(),
                       Blocks::AIR.getDefaultState(), false);
        fillWithBlocks(worldIn, structureBoundingBoxIn, 0, 0, 7, 2, 2, 8, Blocks::AIR.getDefaultState(),
                       Blocks::AIR.getDefaultState(), false);

        for (auto i = 0; i < 5; ++i)
        {
            fillWithBlocks(worldIn, structureBoundingBoxIn, 0, 5 - i - (i < 4 ? 1 : 0), 2 + i, 2, 7 - i, 2 + i,
                           Blocks::AIR.getDefaultState(), Blocks::AIR.getDefaultState(), false);
        }

        return true;
    }
}

StructureBoundingBox StructureMineshaftPieces::Stairs::findStairs(std::vector<> listIn, pcg32 &rand, int32_t x,
                                                                  int32_t y, int32_t z, EnumFacing facing)
{
    StructureBoundingBox structureboundingbox = StructureBoundingBox(x, y - 5, z, x, y + 2, z);
    switch (facing)
    {
    case EnumFacing::NORTH:
    default:
        structureboundingbox.maxX = x + 2;
        structureboundingbox.minZ = z - 8;
        break;
    case EnumFacing::SOUTH:
        structureboundingbox.maxX = x + 2;
        structureboundingbox.maxZ = z + 8;
        break;
    case EnumFacing::WEST:
        structureboundingbox.minX = x - 8;
        structureboundingbox.maxZ = z + 2;
        break;
    case EnumFacing::EAST:
        structureboundingbox.maxX = x + 8;
        structureboundingbox.maxZ = z + 2;
    }

    return StructureComponent::findIntersecting(listIn, structureboundingbox) != nullptr ? std::nullopt
                                                                                         : structureboundingbox;
}

StructureMineshaftPieces::Room::Room(int32_t p_i47137_1_, pcg32 &p_i47137_2_, int32_t p_i47137_3_, int32_t p_i47137_4_,
                                     MapGenMineshaft::Type p_i47137_5_)
    : StructureMineshaftPieces::Peice(p_i47137_1_, p_i47137_5_)
{
    mineShaftType = p_i47137_5_;
    boundingBox   = StructureBoundingBox(p_i47137_3_, 50, p_i47137_4_, p_i47137_3_ + 7 + p_i47137_2_(6),
                                         54 + p_i47137_2_(6), p_i47137_4_ + 7 + p_i47137_2_(6));
}

void StructureMineshaftPieces::Room::buildComponent(StructureComponent *componentIn, std::vector<> listIn, pcg32 &rand)
{
    auto i = getComponentType();
    auto j = boundingBox.getYSize() - 3 - 1;
    if (j <= 0)
    {
        j = 1;
    }

    int k;
    std::unique_ptr<StructureMineshaftPieces::Peice> structurecomponent;
    StructureBoundingBox structureboundingbox3;
    for (k = 0; k < boundingBox.getXSize(); k += 4)
    {
        k += rand(boundingBox.getXSize());
        if (k + 3 > boundingBox.getXSize())
        {
            break;
        }

        structurecomponent = StructureMineshaftPieces::generateAndAddPiece(
            componentIn, listIn, rand, boundingBox.minX + k, boundingBox.minY + rand(j) + 1, boundingBox.minZ - 1,
            EnumFacing::NORTH, i);
        if (structurecomponent != nullptr)
        {
            structureboundingbox3 = structurecomponent->getBoundingBox();
            connectedRooms.add(new StructureBoundingBox(structureboundingbox3.minX, structureboundingbox3.minY,
                                                        boundingBox.minZ, structureboundingbox3.maxX,
                                                        structureboundingbox3.maxY, boundingBox.minZ + 1));
        }
    }

    for (k = 0; k < boundingBox.getXSize(); k += 4)
    {
        k += rand(boundingBox.getXSize());
        if (k + 3 > boundingBox.getXSize())
        {
            break;
        }

        structurecomponent = StructureMineshaftPieces::generateAndAddPiece(
            componentIn, listIn, rand, boundingBox.minX + k, boundingBox.minY + rand(j) + 1, boundingBox.maxZ + 1,
            EnumFacing::SOUTH, i);
        if (structurecomponent != nullptr)
        {
            structureboundingbox3 = structurecomponent->getBoundingBox();
            connectedRooms.add(StructureBoundingBox(structureboundingbox3.minX, structureboundingbox3.minY,
                                                    boundingBox.maxZ - 1, structureboundingbox3.maxX,
                                                    structureboundingbox3.maxY, boundingBox.maxZ));
        }
    }

    for (k = 0; k < boundingBox.getZSize(); k += 4)
    {
        k += rand(boundingBox.getZSize());
        if (k + 3 > boundingBox.getZSize())
        {
            break;
        }

        structurecomponent = StructureMineshaftPieces::generateAndAddPiece(
            componentIn, listIn, rand, boundingBox.minX - 1, boundingBox.minY + rand(j) + 1, boundingBox.minZ + k,
            EnumFacing::WEST, i);
        if (structurecomponent != nullptr)
        {
            structureboundingbox3 = structurecomponent->getBoundingBox();
            connectedRooms.add(StructureBoundingBox(boundingBox.minX, structureboundingbox3.minY,
                                                    structureboundingbox3.minZ, boundingBox.minX + 1,
                                                    structureboundingbox3.maxY, structureboundingbox3.maxZ));
        }
    }

    for (k = 0; k < boundingBox.getZSize(); k += 4)
    {
        k += rand(boundingBox.getZSize());
        if (k + 3 > boundingBox.getZSize())
        {
            break;
        }

        structurecomponent = StructureMineshaftPieces::generateAndAddPiece(
            componentIn, listIn, rand, boundingBox.maxX + 1, boundingBox.minY + rand(j) + 1, boundingBox.minZ + k,
            EnumFacing::EAST, i);
        if (structurecomponent != nullptr)
        {
            structureboundingbox3 = structurecomponent->getBoundingBox();
            connectedRooms.add(StructureBoundingBox(boundingBox.maxX - 1, structureboundingbox3.minY,
                                                    structureboundingbox3.minZ, boundingBox.maxX,
                                                    structureboundingbox3.maxY, structureboundingbox3.maxZ));
        }
    }
}

bool StructureMineshaftPieces::Room::addComponentParts(World *worldIn, pcg32 &randomIn,
                                                       StructureBoundingBox structureBoundingBoxIn)
{
    if (isLiquidInStructureBoundingBox(worldIn, structureBoundingBoxIn))
    {
        return false;
    }
    else
    {
        fillWithBlocks(worldIn, structureBoundingBoxIn, boundingBox.minX, boundingBox.minY, boundingBox.minZ,
                       boundingBox.maxX, boundingBox.minY, boundingBox.maxZ, Blocks::DIRT.getDefaultState(),
                       Blocks::AIR.getDefaultState(), true);
        fillWithBlocks(worldIn, structureBoundingBoxIn, boundingBox.minX, boundingBox.minY + 1, boundingBox.minZ,
                       boundingBox.maxX, MathHelper::min(boundingBox.minY + 3, boundingBox.maxY), boundingBox.maxZ,
                       Blocks::AIR.getDefaultState(), Blocks::AIR.getDefaultState(), false);

        for (auto structureboundingbox : connectedRooms)
        {
            fillWithBlocks(worldIn, structureBoundingBoxIn, structureboundingbox.minX, structureboundingbox.maxY - 2,
                           structureboundingbox.minZ, structureboundingbox.maxX, structureboundingbox.maxY,
                           structureboundingbox.maxZ, Blocks::AIR.getDefaultState(), Blocks::AIR.getDefaultState(),
                           false);
        }

        randomlyRareFillWithBlocks(worldIn, structureBoundingBoxIn, boundingBox.minX, boundingBox.minY + 4,
                                   boundingBox.minZ, boundingBox.maxX, boundingBox.maxY, boundingBox.maxZ,
                                   Blocks::AIR.getDefaultState(), false);
        return true;
    }
}

void StructureMineshaftPieces::Room::offset(int32_t x, int32_t y, int32_t z)
{
    StructureMineshaftPieces::Peice::offset(x, y, z);

    for (auto structureboundingbox : connectedRooms)
    {
        structureboundingbox.offset(x, y, z);
    }
}

void StructureMineshaftPieces::Room::writeStructureToNBT(NBTTagCompound *tagCompound)
{
    StructureMineshaftPieces::Peice::writeStructureToNBT(tagCompound);
    std::unique_ptr<NBTTagList> nbttaglist = nullptr;

    for (auto structureboundingbox : connectedRooms)
    {
        nbttaglist->appendTag(structureboundingbox.toNBTTagIntArray());
    }

    tagCompound->setTag("Entrances", nbttaglist.get());
}

void StructureMineshaftPieces::Room::readStructureFromNBT(NBTTagCompound *tagCompound,
                                                          const TemplateManager &p_143011_2_)
{
    StructureMineshaftPieces::Peice::readStructureFromNBT(tagCompound, p_143011_2_);
    auto nbttaglist = tagCompound->getTagList("Entrances", 11);

    for (auto i = 0; i < nbttaglist->tagCount(); ++i)
    {
        connectedRooms.emplace_back(nbttaglist->getIntArrayAt(i));
    }
}

StructureMineshaftPieces::Cross::Cross(int32_t p_i47139_1_, pcg32 &p_i47139_2_, StructureBoundingBox p_i47139_3_,
                                       std::optional<EnumFacing> p_i47139_4_, MapGenMineshaft::Type p_i47139_5_)
    : Peice(p_i47139_1_, p_i47139_5_), isMultipleFloors(p_i47139_3_.getYSize() > 3), corridorDirection(EnumFacing::UP)
{
    if (p_i47139_4_)
    {
        corridorDirection = p_i47139_4_.value();
    }
    boundingBox = p_i47139_3_;
}

std::optional<StructureBoundingBox> StructureMineshaftPieces::Cross::findCrossing(std::vector<> listIn, pcg32 &rand,
                                                                                  int32_t x, int32_t y, int32_t z,
                                                                                  EnumFacing facing)
{
    StructureBoundingBox structureboundingbox(x, y, z, x, y + 2, z);
    if (rand(4) == 0)
    {
        structureboundingbox.maxY += 4;
    }
    switch (facing)
    {
    case EnumFacing::NORTH:
    default:
        structureboundingbox.minX = x - 1;
        structureboundingbox.maxX = x + 3;
        structureboundingbox.minZ = z - 4;
        break;
    case EnumFacing::SOUTH:
        structureboundingbox.minX = x - 1;
        structureboundingbox.maxX = x + 3;
        structureboundingbox.maxZ = z + 3 + 1;
        break;
    case EnumFacing::WEST:
        structureboundingbox.minX = x - 4;
        structureboundingbox.minZ = z - 1;
        structureboundingbox.maxZ = z + 3;
        break;
    case EnumFacing::EAST:
        structureboundingbox.maxX = x + 3 + 1;
        structureboundingbox.minZ = z - 1;
        structureboundingbox.maxZ = z + 3;
    }

    return StructureComponent::findIntersecting(listIn, structureboundingbox) != nullptr ? std::nullopt
                                                                                         : structureboundingbox;
}

void StructureMineshaftPieces::Cross::buildComponent(StructureComponent *componentIn, std::vector<> listIn, pcg32 &rand)
{
    auto i = getComponentType();
    switch (corridorDirection)
    {
    case EnumFacing::NORTH:
    default:
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX + 1, boundingBox.minY,
                                                      boundingBox.minZ - 1, EnumFacing::NORTH, i);
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX - 1, boundingBox.minY,
                                                      boundingBox.minZ + 1, EnumFacing::WEST, i);
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX + 1, boundingBox.minY,
                                                      boundingBox.minZ + 1, EnumFacing::EAST, i);
        break;
    case EnumFacing::SOUTH:
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX + 1, boundingBox.minY,
                                                      boundingBox.maxZ + 1, EnumFacing::SOUTH, i);
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX - 1, boundingBox.minY,
                                                      boundingBox.minZ + 1, EnumFacing::WEST, i);
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX + 1, boundingBox.minY,
                                                      boundingBox.minZ + 1, EnumFacing::EAST, i);
        break;
    case EnumFacing::WEST:
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX + 1, boundingBox.minY,
                                                      boundingBox.minZ - 1, EnumFacing::NORTH, i);
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX + 1, boundingBox.minY,
                                                      boundingBox.maxZ + 1, EnumFacing::SOUTH, i);
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX - 1, boundingBox.minY,
                                                      boundingBox.minZ + 1, EnumFacing::WEST, i);
        break;
    case EnumFacing::EAST:
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX + 1, boundingBox.minY,
                                                      boundingBox.minZ - 1, EnumFacing::NORTH, i);
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX + 1, boundingBox.minY,
                                                      boundingBox.maxZ + 1, EnumFacing::SOUTH, i);
        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX + 1, boundingBox.minY,
                                                      boundingBox.minZ + 1, EnumFacing::EAST, i);
    }

    if (isMultipleFloors)
    {
        Util::RandomizerWithSentinelShift rwss;
        if (rwss(rand))
        {
            StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX + 1,
                                                          boundingBox.minY + 3 + 1, boundingBox.minZ - 1,
                                                          EnumFacing::NORTH, i);
        }

        if (rwss(rand))
        {
            StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX - 1,
                                                          boundingBox.minY + 3 + 1, boundingBox.minZ + 1,
                                                          EnumFacing::WEST, i);
        }

        if (rwss(rand))
        {
            StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX + 1,
                                                          boundingBox.minY + 3 + 1, boundingBox.minZ + 1,
                                                          EnumFacing::EAST, i);
        }

        if (rwss(rand))
        {
            StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX + 1,
                                                          boundingBox.minY + 3 + 1, boundingBox.maxZ + 1,
                                                          EnumFacing::SOUTH, i);
        }
    }
}

bool StructureMineshaftPieces::Cross::addComponentParts(World *worldIn, pcg32 &randomIn,
                                                        StructureBoundingBox structureBoundingBoxIn)
{
    if (isLiquidInStructureBoundingBox(worldIn, structureBoundingBoxIn))
    {
        return false;
    }
    else
    {
        IBlockState *iblockstate = getPlanksBlock();
        if (isMultipleFloors)
        {
            fillWithBlocks(worldIn, structureBoundingBoxIn, boundingBox.minX + 1, boundingBox.minY, boundingBox.minZ,
                           boundingBox.maxX - 1, boundingBox.minY + 3 - 1, boundingBox.maxZ,
                           Blocks.AIR.getDefaultState(), Blocks.AIR.getDefaultState(), false);
            fillWithBlocks(worldIn, structureBoundingBoxIn, boundingBox.minX, boundingBox.minY, boundingBox.minZ + 1,
                           boundingBox.maxX, boundingBox.minY + 3 - 1, boundingBox.maxZ - 1,
                           Blocks.AIR.getDefaultState(), Blocks.AIR.getDefaultState(), false);
            fillWithBlocks(worldIn, structureBoundingBoxIn, boundingBox.minX + 1, boundingBox.maxY - 2,
                           boundingBox.minZ, boundingBox.maxX - 1, boundingBox.maxY, boundingBox.maxZ,
                           Blocks.AIR.getDefaultState(), Blocks.AIR.getDefaultState(), false);
            fillWithBlocks(worldIn, structureBoundingBoxIn, boundingBox.minX, boundingBox.maxY - 2,
                           boundingBox.minZ + 1, boundingBox.maxX, boundingBox.maxY, boundingBox.maxZ - 1,
                           Blocks.AIR.getDefaultState(), Blocks.AIR.getDefaultState(), false);
            fillWithBlocks(worldIn, structureBoundingBoxIn, boundingBox.minX + 1, boundingBox.minY + 3,
                           boundingBox.minZ + 1, boundingBox.maxX - 1, boundingBox.minY + 3, boundingBox.maxZ - 1,
                           Blocks.AIR.getDefaultState(), Blocks.AIR.getDefaultState(), false);
        }
        else
        {
            fillWithBlocks(worldIn, structureBoundingBoxIn, boundingBox.minX + 1, boundingBox.minY, boundingBox.minZ,
                           boundingBox.maxX - 1, boundingBox.maxY, boundingBox.maxZ, Blocks.AIR.getDefaultState(),
                           Blocks.AIR.getDefaultState(), false);
            fillWithBlocks(worldIn, structureBoundingBoxIn, boundingBox.minX, boundingBox.minY, boundingBox.minZ + 1,
                           boundingBox.maxX, boundingBox.maxY, boundingBox.maxZ - 1, Blocks.AIR.getDefaultState(),
                           Blocks.AIR.getDefaultState(), false);
        }

        placeSupportPillar(worldIn, structureBoundingBoxIn, boundingBox.minX + 1, boundingBox.minY,
                           boundingBox.minZ + 1, boundingBox.maxY);
        placeSupportPillar(worldIn, structureBoundingBoxIn, boundingBox.minX + 1, boundingBox.minY,
                           boundingBox.maxZ - 1, boundingBox.maxY);
        placeSupportPillar(worldIn, structureBoundingBoxIn, boundingBox.maxX - 1, boundingBox.minY,
                           boundingBox.minZ + 1, boundingBox.maxY);
        placeSupportPillar(worldIn, structureBoundingBoxIn, boundingBox.maxX - 1, boundingBox.minY,
                           boundingBox.maxZ - 1, boundingBox.maxY);

        for (auto i = boundingBox.minX; i <= boundingBox.maxX; ++i)
        {
            for (auto j = boundingBox.minZ; j <= boundingBox.maxZ; ++j)
            {
                if (getBlockStateFromPos(worldIn, i, boundingBox.minY - 1, j, structureBoundingBoxIn).getMaterial() ==
                        Material::AIR &&
                    getSkyBrightness(worldIn, i, boundingBox.minY - 1, j, structureBoundingBoxIn) < 8)
                {
                    setBlockState(worldIn, iblockstate, i, boundingBox.minY - 1, j, structureBoundingBoxIn);
                }
            }
        }

        return true;
    }
}

void StructureMineshaftPieces::Cross::writeStructureToNBT(NBTTagCompound *tagCompound)
{
    Peice::writeStructureToNBT(tagCompound);
    tagCompound->setBoolean("tf", isMultipleFloors);
    tagCompound->setInteger("D", corridorDirection.getHorizontalIndex());
}

void StructureMineshaftPieces::Cross::readStructureFromNBT(NBTTagCompound *tagCompound,
                                                           const TemplateManager &p_143011_2_)
{
    Peice::readStructureFromNBT(tagCompound, p_143011_2_);
    isMultipleFloors  = tagCompound->getBoolean("tf");
    corridorDirection = EnumFacing::byHorizontalIndex(tagCompound->getInteger("D"));
}

void StructureMineshaftPieces::Cross::placeSupportPillar(World *p_189923_1_, StructureBoundingBox p_189923_2_,
                                                         int32_t p_189923_3_, int32_t p_189923_4_, int32_t p_189923_5_,
                                                         int32_t p_189923_6_)
{
    if (getBlockStateFromPos(p_189923_1_, p_189923_3_, p_189923_6_ + 1, p_189923_5_, p_189923_2_)->getMaterial() !=
        Material::AIR)
    {
        fillWithBlocks(p_189923_1_, p_189923_2_, p_189923_3_, p_189923_4_, p_189923_5_, p_189923_3_, p_189923_6_,
                       p_189923_5_, getPlanksBlock(), Blocks.AIR.getDefaultState(), false);
    }
}

StructureMineshaftPieces::Corridor::Corridor(int32_t p_i47140_1_, pcg32 &p_i47140_2_, StructureBoundingBox p_i47140_3_,
                                             EnumFacing p_i47140_4_, MapGenMineshaft::Type p_i47140_5_)
    : Peice(p_i47140_1_, p_i47140_5_)
{
    setCoordBaseMode(p_i47140_4_);
    boundingBox = p_i47140_3_;
    hasRails    = p_i47140_2_(3) == 0;
    hasSpiders  = !hasRails && p_i47140_2_(23) == 0;
    if (getCoordBaseMode().getAxis() == Axis::Z)
    {
        sectionCount = p_i47140_3_.getZSize() / 5;
    }
    else
    {
        sectionCount = p_i47140_3_.getXSize() / 5;
    }
}

std::optional<StructureBoundingBox> StructureMineshaftPieces::Corridor::findCorridorSize(std::vector<> p_175814_0_,
                                                                                         pcg32 &rand, int32_t x,
                                                                                         int32_t y, int32_t z,
                                                                                         EnumFacing facing)
{
    StructureBoundingBox structureboundingbox(x, y, z, x, y + 2, z);
    int32_t i = 0;
    for (i = rand(3) + 2; i > 0; --i)
    {
        auto j = i * 5;
        switch (facing)
        {
        case EnumFacing::NORTH:
        default:
            structureboundingbox.maxX = x + 2;
            structureboundingbox.minZ = z - (j - 1);
            break;
        case EnumFacing::SOUTH:
            structureboundingbox.maxX = x + 2;
            structureboundingbox.maxZ = z + (j - 1);
            break;
        case EnumFacing::WEST:
            structureboundingbox.minX = x - (j - 1);
            structureboundingbox.maxZ = z + 2;
            break;
        case EnumFacing::EAST:
            structureboundingbox.maxX = x + (j - 1);
            structureboundingbox.maxZ = z + 2;
        }

        if (StructureComponent::findIntersecting(p_175814_0_, structureboundingbox) == nullptr)
        {
            break;
        }
    }

    return i > 0 ? structureboundingbox : std::nullopt;
}

void StructureMineshaftPieces::Corridor::buildComponent(StructureComponent *componentIn, std::vector<> listIn,
                                                        pcg32 &rand)
{
    int32_t i             = getComponentType();
    int32_t j             = rand(4);
    EnumFacing enumfacing = getCoordBaseMode();
    {
        switch (enumfacing)
        {
        case EnumFacing::NORTH:
        default:
            if (j <= 1)
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.minZ - 1,
                                                              enumfacing, i);
            }
            else if (j == 2)
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX - 1,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.minZ,
                                                              EnumFacing::WEST, i);
            }
            else
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX + 1,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.minZ,
                                                              EnumFacing::EAST, i);
            }
            break;
        case EnumFacing::SOUTH:
            if (j <= 1)
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.maxZ + 1,
                                                              enumfacing, i);
            }
            else if (j == 2)
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX - 1,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.maxZ - 3,
                                                              EnumFacing::WEST, i);
            }
            else
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX + 1,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.maxZ - 3,
                                                              EnumFacing::EAST, i);
            }
            break;
        case EnumFacing::WEST:
            if (j <= 1)
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX - 1,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.minZ,
                                                              enumfacing, i);
            }
            else if (j == 2)
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.minZ - 1,
                                                              EnumFacing::NORTH, i);
            }
            else
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.maxZ + 1,
                                                              EnumFacing::SOUTH, i);
            }
            break;
        case EnumFacing::EAST:
            if (j <= 1)
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX + 1,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.minZ,
                                                              enumfacing, i);
            }
            else if (j == 2)
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX - 3,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.minZ - 1,
                                                              EnumFacing::NORTH, i);
            }
            else
            {
                StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX - 3,
                                                              boundingBox.minY - 1 + rand(3), boundingBox.maxZ + 1,
                                                              EnumFacing::SOUTH, i);
            }
        }

        if (i < 8)
        {
            int k;
            int l;
            if (enumfacing != EnumFacing::NORTH && enumfacing != EnumFacing::SOUTH)
            {
                for (k = boundingBox.minX + 3; k + 3 <= boundingBox.maxX; k += 5)
                {
                    l = rand(5);
                    if (l == 0)
                    {
                        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, k, boundingBox.minY,
                                                                      boundingBox.minZ - 1, EnumFacing::NORTH, i + 1);
                    }
                    else if (l == 1)
                    {
                        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, k, boundingBox.minY,
                                                                      boundingBox.maxZ + 1, EnumFacing::SOUTH, i + 1);
                    }
                }
            }
            else
            {
                for (k = boundingBox.minZ + 3; k + 3 <= boundingBox.maxZ; k += 5)
                {
                    l = rand(5);
                    if (l == 0)
                    {
                        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.minX - 1,
                                                                      boundingBox.minY, k, EnumFacing::WEST, i + 1);
                    }
                    else if (l == 1)
                    {
                        StructureMineshaftPieces::generateAndAddPiece(componentIn, listIn, rand, boundingBox.maxX + 1,
                                                                      boundingBox.minY, k, EnumFacing::EAST, i + 1);
                    }
                }
            }
        }
    }
}

bool StructureMineshaftPieces::Corridor::addComponentParts(World *worldIn, pcg32 &randomIn,
                                                           StructureBoundingBox structureBoundingBoxIn)
{
    if (isLiquidInStructureBoundingBox(worldIn, structureBoundingBoxIn))
    {
        return false;
    }
    else
    {
        auto i1                  = sectionCount * 5 - 1;
        IBlockState *iblockstate = getPlanksBlock();
        fillWithBlocks(worldIn, structureBoundingBoxIn, 0, 0, 0, 2, 1, i1, Blocks::AIR.getDefaultState(),
                       Blocks.AIR.getDefaultState(), false);
        generateMaybeBox(worldIn, structureBoundingBoxIn, randomIn, 0.8F, 0, 2, 0, 2, 2, i1,
                         Blocks::AIR.getDefaultState(), Blocks.AIR.getDefaultState(), false, 0);
        if (hasSpiders)
        {
            generateMaybeBox(worldIn, structureBoundingBoxIn, randomIn, 0.6F, 0, 0, 0, 2, 1, i1,
                             Blocks.WEB.getDefaultState(), Blocks.AIR.getDefaultState(), false, 8);
        }

        for (auto l2 = 0; l2 < sectionCount; ++l2)
        {
            auto j3 = 2 + l2 * 5;
            placeSupport(worldIn, structureBoundingBoxIn, 0, 0, j3, 2, 2, randomIn);
            placeCobWeb(worldIn, structureBoundingBoxIn, randomIn, 0.1F, 0, 2, j3 - 1);
            placeCobWeb(worldIn, structureBoundingBoxIn, randomIn, 0.1F, 2, 2, j3 - 1);
            placeCobWeb(worldIn, structureBoundingBoxIn, randomIn, 0.1F, 0, 2, j3 + 1);
            placeCobWeb(worldIn, structureBoundingBoxIn, randomIn, 0.1F, 2, 2, j3 + 1);
            placeCobWeb(worldIn, structureBoundingBoxIn, randomIn, 0.05F, 0, 2, j3 - 2);
            placeCobWeb(worldIn, structureBoundingBoxIn, randomIn, 0.05F, 2, 2, j3 - 2);
            placeCobWeb(worldIn, structureBoundingBoxIn, randomIn, 0.05F, 0, 2, j3 + 2);
            placeCobWeb(worldIn, structureBoundingBoxIn, randomIn, 0.05F, 2, 2, j3 + 2);
            if (randomIn(100) == 0)
            {
                generateChest(worldIn, structureBoundingBoxIn, randomIn, 2, 0, j3 - 1,
                              LootTableList.CHESTS_ABANDONED_MINESHAFT);
            }

            if (randomIn(100) == 0)
            {
                generateChest(worldIn, structureBoundingBoxIn, randomIn, 0, 0, j3 + 1,
                              LootTableList.CHESTS_ABANDONED_MINESHAFT);
            }

            if (hasSpiders && !spawnerPlaced)
            {
                auto l1 = getYWithOffset(0);
                auto i2 = j3 - 1 + randomIn(3);
                auto j2 = getXWithOffset(1, i2);
                auto k2 = getZWithOffset(1, i2);
                BlockPos blockpos(j2, l1, k2);
                if (structureBoundingBoxIn.isVecInside(blockpos) &&
                    getSkyBrightness(worldIn, 1, 0, i2, structureBoundingBoxIn) < 8)
                {
                    spawnerPlaced = true;
                    worldIn->setBlockState(blockpos, Blocks.MOB_SPAWNER.getDefaultState(), 2);
                    auto tileentity = worldIn->getTileEntity(blockpos);
                    if (Util:: instanceof <TileEntityMobSpawner>(tileentity))
                    {
                        ((TileEntityMobSpawner)tileentity)
                            .getSpawnerBaseLogic()
                            .setEntityId(EntityList.getKey(EntityCaveSpider.class));
                    }
                }
            }
        }

        for (auto l2 = 0; l2 <= 2; ++l2)
        {
            for (auto j3 = 0; j3 <= i1; ++j3)
            {
                IBlockState *iblockstate3 = getBlockStateFromPos(worldIn, l2, -1, j3, structureBoundingBoxIn);
                if (iblockstate3->getMaterial() == Material::AIR &&
                    getSkyBrightness(worldIn, l2, -1, j3, structureBoundingBoxIn) < 8)
                {
                    setBlockState(worldIn, iblockstate, l2, -1, j3, structureBoundingBoxIn);
                }
            }
        }

        if (hasRails)
        {
            IBlockState *iblockstate1 = Blocks.RAIL.getDefaultState().withProperty(
                BlockRail.SHAPE, BlockRailBase.EnumRailDirection.NORTH_SOUTH);

            for (auto j3 = 0; j3 <= i1; ++j3)
            {
                IBlockState *iblockstate2 = getBlockStateFromPos(worldIn, 1, -1, j3, structureBoundingBoxIn);
                if (iblockstate2->getMaterial() != Material::AIR && iblockstate2->isFullBlock())
                {
                    float f = getSkyBrightness(worldIn, 1, 0, j3, structureBoundingBoxIn) > 8 ? 0.9f : 0.7f;
                    randomlyPlaceBlock(worldIn, structureBoundingBoxIn, randomIn, f, 1, 0, j3, iblockstate1);
                }
            }
        }

        return true;
    }
}

void StructureMineshaftPieces::Corridor::placeSupport(World *p_189921_1_, StructureBoundingBox p_189921_2_,
                                                      int32_t p_189921_3_, int32_t p_189921_4_, int32_t p_189921_5_,
                                                      int32_t p_189921_6_, int32_t p_189921_7_, pcg32 &p_189921_8_)
{
    if (isSupportingBox(p_189921_1_, p_189921_2_, p_189921_3_, p_189921_7_, p_189921_6_, p_189921_5_))
    {
        IBlockState *iblockstate  = getPlanksBlock();
        IBlockState *iblockstate1 = getFenceBlock();
        IBlockState *iblockstate2 = Blocks.AIR.getDefaultState();
        fillWithBlocks(p_189921_1_, p_189921_2_, p_189921_3_, p_189921_4_, p_189921_5_, p_189921_3_, p_189921_6_ - 1,
                       p_189921_5_, iblockstate1, iblockstate2, false);
        fillWithBlocks(p_189921_1_, p_189921_2_, p_189921_7_, p_189921_4_, p_189921_5_, p_189921_7_, p_189921_6_ - 1,
                       p_189921_5_, iblockstate1, iblockstate2, false);
        if (p_189921_8_(4) == 0)
        {
            fillWithBlocks(p_189921_1_, p_189921_2_, p_189921_3_, p_189921_6_, p_189921_5_, p_189921_3_, p_189921_6_,
                           p_189921_5_, iblockstate, iblockstate2, false);
            fillWithBlocks(p_189921_1_, p_189921_2_, p_189921_7_, p_189921_6_, p_189921_5_, p_189921_7_, p_189921_6_,
                           p_189921_5_, iblockstate, iblockstate2, false);
        }
        else
        {
            fillWithBlocks(p_189921_1_, p_189921_2_, p_189921_3_, p_189921_6_, p_189921_5_, p_189921_7_, p_189921_6_,
                           p_189921_5_, iblockstate, iblockstate2, false);
            randomlyPlaceBlock(p_189921_1_, p_189921_2_, p_189921_8_, 0.05F, p_189921_3_ + 1, p_189921_6_,
                               p_189921_5_ - 1,
                               Blocks.TORCH.getDefaultState().withProperty(BlockTorch.FACING, EnumFacing::NORTH));
            randomlyPlaceBlock(p_189921_1_, p_189921_2_, p_189921_8_, 0.05F, p_189921_3_ + 1, p_189921_6_,
                               p_189921_5_ + 1,
                               Blocks.TORCH.getDefaultState().withProperty(BlockTorch.FACING, EnumFacing::SOUTH));
        }
    }
}

std::unique_ptr<StructureMineshaftPieces::Peice> StructureMineshaftPieces::createRandomShaftPiece(
    std::vector<> p_189940_0_, pcg32 &p_189940_1_, int32_t p_189940_2_, int32_t p_189940_3_, int32_t p_189940_4_,
    std::optional<EnumFacing> p_189940_5_, int32_t p_189940_6_, MapGenMineshaft::Type p_189940_7_)
{
    int i = p_189940_1_(100);
    std::optional<StructureBoundingBox> structureboundingbox1;
    if (i >= 80)
    {
        structureboundingbox1 = StructureMineshaftPieces::Cross::findCrossing(p_189940_0_, p_189940_1_, p_189940_2_,
                                                                              p_189940_3_, p_189940_4_, p_189940_5_);
        if (structureboundingbox1 != std::nullopt)
        {
            return std::make_unique<StructureMineshaftPieces::Cross>(p_189940_6_, p_189940_1_, structureboundingbox1,
                                                                     p_189940_5_, p_189940_7_);
        }
    }
    else if (i >= 70)
    {
        structureboundingbox1 = StructureMineshaftPieces::Stairs::findStairs(p_189940_0_, p_189940_1_, p_189940_2_,
                                                                             p_189940_3_, p_189940_4_, p_189940_5_);
        if (structureboundingbox1 != std::nullopt)
        {
            return std::make_unique<StructureMineshaftPieces::Stairs>(p_189940_6_, p_189940_1_, structureboundingbox1,
                                                                      p_189940_5_, p_189940_7_);
        }
    }
    else
    {
        structureboundingbox1 = StructureMineshaftPieces::Corridor::findCorridorSize(
            p_189940_0_, p_189940_1_, p_189940_2_, p_189940_3_, p_189940_4_, p_189940_5_);
        if (structureboundingbox1 != std::nullopt)
        {
            return std::make_unique<StructureMineshaftPieces::Corridor>(p_189940_6_, p_189940_1_, structureboundingbox1,
                                                                        p_189940_5_, p_189940_7_);
        }
    }

    return nullptr;
}

std::unique_ptr<StructureMineshaftPieces::Peice> StructureMineshaftPieces::generateAndAddPiece(
    StructureComponent *p_189938_0_, std::vector<> p_189938_1_, pcg32 &p_189938_2_, int32_t p_189938_3_,
    int32_t p_189938_4_, int32_t p_189938_5_, EnumFacing p_189938_6_, int32_t p_189938_7_)
{
    if (p_189938_7_ > 8)
    {
        return nullptr;
    }
    else if (MathHelper::abs(p_189938_3_ - p_189938_0_->getBoundingBox().minX) <= 80 &&
             MathHelper::abs(p_189938_5_ - p_189938_0_->getBoundingBox().minZ) <= 80)
    {
        auto mapgenmineshaft$type = ((StructureMineshaftPieces::Peice)p_189938_0_)->mineShaftType;
        auto structuremineshaftpieces$peice =
            createRandomShaftPiece(p_189938_1_, p_189938_2_, p_189938_3_, p_189938_4_, p_189938_5_, p_189938_6_,
                                   p_189938_7_ + 1, mapgenmineshaft$type);
        if (structuremineshaftpieces$peice != nullptr)
        {
            p_189938_1_.add(structuremineshaftpieces$peice);
            structuremineshaftpieces$peice->buildComponent(p_189938_0_, p_189938_1_, p_189938_2_);
        }

        return structuremineshaftpieces$peice;
    }
    else
    {
        return nullptr;
    }
}

bool StructureMineshaftPieces::Corridor::generateChest(World *worldIn, StructureBoundingBox structurebb,
                                                       pcg32 &randomIn, int32_t x, int32_t y, int32_t z,
                                                       ResourceLocation loot)
{
    BlockPos blockpos(getXWithOffset(x, z), getYWithOffset(y), getZWithOffset(x, z));
    if (structurebb.isVecInside(blockpos) && worldIn->getBlockState(blockpos).getMaterial() == Material::AIR &&
        worldIn->getBlockState(blockpos.down()).getMaterial() != Material::AIR)
    {
        IBlockState *iblockstate = Blocks.RAIL.getDefaultState().withProperty(
            BlockRail.SHAPE, randomIn.nextBoolean() ? BlockRailBase.EnumRailDirection.NORTH_SOUTH
                                                    : BlockRailBase.EnumRailDirection.EAST_WEST);
        setBlockState(worldIn, iblockstate, x, y, z, structurebb);
        EntityMinecartChest entityminecartchest =
            new EntityMinecartChest(worldIn, (double)((float)blockpos.getX() + 0.5F),
                                    (double)((float)blockpos.getY() + 0.5F), (double)((float)blockpos.getZ() + 0.5F));
        entityminecartchest.setLootTable(loot, MathHelper::nextLong(randomIn));
        worldIn->spawnEntity(entityminecartchest);
        return true;
    }
    return false;
}

void StructureMineshaftPieces::Corridor::writeStructureToNBT(NBTTagCompound *tagCompound)
{
    Peice::writeStructureToNBT(tagCompound);
    tagCompound->setBoolean("hr", hasRails);
    tagCompound->setBoolean("sc", hasSpiders);
    tagCompound->setBoolean("hps", spawnerPlaced);
    tagCompound->setInteger("Num", sectionCount);
}

void StructureMineshaftPieces::Corridor::readStructureFromNBT(NBTTagCompound *tagCompound,
                                                              const TemplateManager &p_143011_2_)
{
    Peice::readStructureFromNBT(tagCompound, p_143011_2_);
    hasRails      = tagCompound->getBoolean("hr");
    hasSpiders    = tagCompound->getBoolean("sc");
    spawnerPlaced = tagCompound->getBoolean("hps");
    sectionCount  = tagCompound->getInteger("Num");
}
