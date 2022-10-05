#pragma once
#include "WorldGenerator.h"

#include <array>

class ResourceLocation;
class WorldGenFossils : public WorldGenerator
{
  public:
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    static ResourceLocation STRUCTURE_SPINE_01;
    static ResourceLocation STRUCTURE_SPINE_02;
    static ResourceLocation STRUCTURE_SPINE_03;
    static ResourceLocation STRUCTURE_SPINE_04;
    static ResourceLocation STRUCTURE_SPINE_01_COAL;
    static ResourceLocation STRUCTURE_SPINE_02_COAL;
    static ResourceLocation STRUCTURE_SPINE_03_COAL;
    static ResourceLocation STRUCTURE_SPINE_04_COAL;
    static ResourceLocation STRUCTURE_SKULL_01;
    static ResourceLocation STRUCTURE_SKULL_02;
    static ResourceLocation STRUCTURE_SKULL_03;
    static ResourceLocation STRUCTURE_SKULL_04;
    static ResourceLocation STRUCTURE_SKULL_01_COAL;
    static ResourceLocation STRUCTURE_SKULL_02_COAL;
    static ResourceLocation STRUCTURE_SKULL_03_COAL;
    static ResourceLocation STRUCTURE_SKULL_04_COAL;
    static std::array<ResourceLocation, 8> FOSSILS;
    static std::array<ResourceLocation, 8> FOSSILS_COAL;
};
