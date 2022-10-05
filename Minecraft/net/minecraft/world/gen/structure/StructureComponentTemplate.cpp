#include "StructureComponentTemplate.h"

StructureComponentTemplate::StructureComponentTemplate() : StructureComponent()
{
    placeSettings = DEFAULT_PLACE_SETTINGS.setIgnoreEntities(true).setReplacedBlock(Blocks::AIR);
}

StructureComponentTemplate::StructureComponentTemplate(int32_t type) : StructureComponent(type)
{
    placeSettings = DEFAULT_PLACE_SETTINGS.setIgnoreEntities(true).setReplacedBlock(Blocks::AIR);
}

void StructureComponentTemplate::offset(int32_t x, int32_t y, int32_t z)
{
    StructureComponent::offset(x, y, z);
    templatePosition = templatePosition.add(x, y, z);
}

void StructureComponentTemplate::setup(const Template &templateIn, BlockPos &pos, const PlacementSettings &settings)
{
    templates = templateIn;
    setCoordBaseMode(EnumFacing::NORTH);
    templatePosition = pos;
    placeSettings    = settings;
    setBoundingBoxFromTemplate();
}

void StructureComponentTemplate::writeStructureToNBT(NBTTagCompound *tagCompound)
{
    tagCompound->setInteger("TPX", templatePosition.getx());
    tagCompound->setInteger("TPY", templatePosition.gety());
    tagCompound->setInteger("TPZ", templatePosition.getz());
}

void StructureComponentTemplate::readStructureFromNBT(NBTTagCompound *tagCompound, const TemplateManager &p_143011_2_)
{
    templatePosition =
        BlockPos(tagCompound->getInteger("TPX"), tagCompound->getInteger("TPY"), tagCompound->getInteger("TPZ"));
}

bool StructureComponentTemplate::addComponentParts(World *worldIn, pcg32 &randomIn,
                                                   StructureBoundingBox structureBoundingBoxIn)
{
    placeSettings.setBoundingBox(structureBoundingBoxIn);
    templates.addBlocksToWorld(worldIn, templatePosition, placeSettings, 18);
    auto map = templates.getDataBlocks(templatePosition, placeSettings);

    for (auto &entry : map)
    {
        auto s = entry.second;
        handleDataMarker(s, entry.first, worldIn, randomIn, structureBoundingBoxIn);
    }

    return true;
}

void StructureComponentTemplate::setBoundingBoxFromTemplate()
{
    Rotation rotation = placeSettings.getRotation();
    BlockPos blockpos = templates.transformedSize(rotation);
    Mirror mirror     = placeSettings.getMirror();
    boundingBox       = StructureBoundingBox(0, 0, 0, blockpos.getx(), blockpos.gety() - 1, blockpos.getz());
    switch (rotation)
    {
    case Rotation::NONE:
    default:
        break;
    case Rotation::CLOCKWISE_90:
        boundingBox.offset(-blockpos.getx(), 0, 0);
        break;
    case Rotation::COUNTERCLOCKWISE_90:
        boundingBox.offset(0, 0, -blockpos.getz());
        break;
    case Rotation::CLOCKWISE_180:
        boundingBox.offset(-blockpos.getx(), 0, -blockpos.getz());
    }

    switch (mirror)
    {
    case Mirror::NONE:
    default:
        break;
    case Mirror::FRONT_BACK:
        MutableBlockPos blockpos2 = BlockPos::ORIGIN;
        if (rotation != Rotation::CLOCKWISE_90 && rotation != Rotation::COUNTERCLOCKWISE_90)
        {
            if (rotation == Rotation::CLOCKWISE_180)
            {
                blockpos2 = blockpos2.offset(EnumFacing::EAST, blockpos.getx());
            }
            else
            {
                blockpos2 = blockpos2.offset(EnumFacing::WEST, blockpos.getx());
            }
        }
        else
        {
            blockpos2 = blockpos2.offset(rotation.rotate(EnumFacing::WEST), blockpos.getz());
        }

        boundingBox.offset(blockpos2.getx(), 0, blockpos2.getz());
        break;
    case Mirror::LEFT_RIGHT:
        MutableBlockPos blockpos1 = BlockPos::ORIGIN;
        if (rotation != Rotation::CLOCKWISE_90 && rotation != Rotation::COUNTERCLOCKWISE_90)
        {
            if (rotation == Rotation::CLOCKWISE_180)
            {
                blockpos1 = blockpos1.offset(EnumFacing::SOUTH, blockpos.getz());
            }
            else
            {
                blockpos1 = blockpos1.offset(EnumFacing::NORTH, blockpos.getz());
            }
        }
        else
        {
            blockpos1 = blockpos1.offset(rotation.rotate(EnumFacing::NORTH), blockpos.getx());
        }

        boundingBox.offset(blockpos1.getx(), 0, blockpos1.getz());
    }

    boundingBox.offset(templatePosition.getx(), templatePosition.gety(), templatePosition.getz());
}
