#pragma once
#include "../../../../../../pcg-cpp/pcg_random.hpp"
#include "NBTTagCompound.h"
#include "StructureComponent.h"

#include <vector>

class ChunkPos;
class World;

class StructureStart
{
  public:
    StructureStart();
    StructureStart(int32_t chunkX, int32_t chunkZ);
    StructureBoundingBox getBoundingBox();
    std::vector<StructureComponent> getComponents();
    void generateStructure(World *worldIn, pcg32 &rand, StructureBoundingBox structurebb);
    std::unique_ptr<NBTTagCompound> writeStructureComponentsToNBT(int32_t chunkX, int32_t chunkZ);
    virtual void writeToNBT(NBTTagCompound *tagCompound);
    void readStructureComponentsFromNBT(World *worldIn, NBTTagCompound *tagCompound);
    virtual void readFromNBT(NBTTagCompound *tagCompound);
    virtual bool isSizeableStructure();
    bool isValidForPostProcess(const ChunkPos &pair);
    virtual void notifyPostProcessAt(const ChunkPos &pair);
    int32_t getChunkPosX() const;
    int32_t getChunkPosZ() const;

  protected:
    std::vector<StructureComponent> components;
    StructureBoundingBox boundingBox;

    void updateBoundingBox();
    void markAvailableHeight(World *worldIn, pcg32 &rand, int32_t p_75067_3_);
    void setRandomHeight(World *worldIn, pcg32 &rand, int32_t p_75070_3_, int32_t p_75070_4_);

  private:
    int32_t chunkPosX;
    int32_t chunkPosZ;
};
