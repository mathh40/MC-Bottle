#include "StructureStart.h"

#include "World.h"

StructureStart::StructureStart()
{
}

StructureStart::StructureStart(int32_t chunkX, int32_t chunkZ)
{
    chunkPosX = chunkX;
    chunkPosZ = chunkZ;
}

StructureBoundingBox StructureStart::getBoundingBox()
{
    return boundingBox;
}

std::vector<StructureComponent> StructureStart::getComponents()
{
    return components;
}

void StructureStart::generateStructure(World *worldIn, pcg32 &rand, StructureBoundingBox structurebb)
{
    for (auto iterator = components.begin(); iterator != components.end(), ++iterator)
    {
        auto structurecomponent = *iterator;
        if (structurecomponent.getBoundingBox().intersectsWith(structurebb) &&
            !structurecomponent.addComponentParts(worldIn, rand, structurebb))
        {
            iterator = components.erase(iterator);
        }
    }
}

std::unique_ptr<NBTTagCompound> StructureStart::writeStructureComponentsToNBT(int32_t chunkX, int32_t chunkZ)
{
    auto nbttagcompound = std::make_unique<NBTTagCompound>();
    nbttagcompound->setString("id", MapGenStructureIO::getStructureStartName(this));
    nbttagcompound->setInteger("ChunkX", chunkX);
    nbttagcompound->setInteger("ChunkZ", chunkZ);
    nbttagcompound->setTag("BB", boundingBox.toNBTTagIntArray());
    auto nbttaglist = std::make_unique<NBTTagList>();

    for (auto structurecomponent : components)
    {
        nbttaglist->appendTag(structurecomponent.createStructureBaseNBT());
    }

    nbttagcompound->setTag("Children", nbttaglist);
    writeToNBT(nbttagcompound.get());
    return nbttagcompound;
}

void StructureStart::writeToNBT(NBTTagCompound *tagCompound)
{
}

void StructureStart::readStructureComponentsFromNBT(World *worldIn, NBTTagCompound *tagCompound)
{
    chunkPosX = tagCompound->getInteger("ChunkX");
    chunkPosZ = tagCompound->getInteger("ChunkZ");
    if (tagCompound->hasKey("BB"))
    {
        boundingBox = StructureBoundingBox(tagCompound->getIntArray("BB"));
    }

    auto nbttaglist = tagCompound->getTagList("Children", 10);

    for (auto i = 0; i < nbttaglist->tagCount(); ++i)
    {
        components.emplace(MapGenStructureIO.getStructureComponent(nbttaglist->getCompoundTagAt(i), worldIn));
    }

    readFromNBT(tagCompound);
}

void StructureStart::readFromNBT(NBTTagCompound *tagCompound)
{
}

bool StructureStart::isSizeableStructure()
{
    return true;
}

bool StructureStart::isValidForPostProcess(const ChunkPos &pair)
{
    return true;
}

void StructureStart::notifyPostProcessAt(const ChunkPos &pair)
{
}

int32_t StructureStart::getChunkPosX() const
{
    return chunkPosX;
}

int32_t StructureStart::getChunkPosZ() const
{
    return chunkPosZ;
}

void StructureStart::updateBoundingBox()
{
    boundingBox = StructureBoundingBox::getNewBoundingBox();

    for (auto structurecomponent : components)
    {
        boundingBox.expandTo(structurecomponent.getBoundingBox());
    }
}

void StructureStart::markAvailableHeight(World *worldIn, pcg32 &rand, int32_t p_75067_3_)
{
    auto i = worldIn->getSeaLevel() - p_75067_3_;
    auto j = boundingBox.getYSize() + 1;
    if (j < i)
    {
        j += rand(i - j);
    }

    auto k = j - boundingBox.maxY;
    boundingBox.offset(0, k, 0);

    for (auto structurecomponent : components)
    {
        structurecomponent.offset(0, k, 0);
    }
}

void StructureStart::setRandomHeight(World *worldIn, pcg32 &rand, int32_t p_75070_3_, int32_t p_75070_4_)
{
    auto i = p_75070_4_ - p_75070_3_ + 1 - boundingBox.getYSize();
    auto j = 0;
    if (i > 1)
    {
        j = p_75070_3_ + rand(i);
    }
    else
    {
        j = p_75070_3_;
    }

    auto k = j - boundingBox.minY;
    boundingBox.offset(0, k, 0);

    for (auto structurecomponent : components)
    {
        structurecomponent.offset(0, k, 0);
    }
}
