#pragma once
#include "StructureComponent.h"

class ComponentScatteredFeaturePieces
{
  public:
    void registerScatteredFeaturePieces();

    class Feature : public StructureComponent
    {
      public:
        Feature() = default;

      protected:
        Feature(pcg32 &rand, int32_t x, int32_t y, int32_t z, int32_t sizeX, int32_t sizeY, int32_t sizeZ);
        void writeStructureToNBT(NBTTagCompound *tagCompound) override;
        void readStructureFromNBT(NBTTagCompound *tagCompound, TemplateManager p_143011_2_) override;
        bool offsetToAverageGroundLevel(World *worldIn, StructureBoundingBox structurebb, int32_t yOffset);

        int32_t width;
        int32_t height;
        int32_t depth;
        int32_t horizontalPos = -1;
    };

    class SwampHut : public Feature
    {
      public:
        SwampHut() = default;
        SwampHut(pcg32 &rand, int32_t x, int32_t z);
        bool addComponentParts(World *worldIn, pcg32 &randomIn, StructureBoundingBox structureBoundingBoxIn) override;

      protected:
        void writeStructureToNBT(NBTTagCompound *tagCompound) override;
        void readStructureFromNBT(NBTTagCompound *tagCompound, TemplateManager p_143011_2_) override;

      private:
        bool hasWitch;
    };

    class JunglePyramid : public Feature
    {
      public:
        JunglePyramid() = default;
        JunglePyramid(pcg32 &rand, int32_t x, int32_t z);
        bool addComponentParts(World *worldIn, pcg32 &randomIn, StructureBoundingBox structureBoundingBoxIn) override;

        class Stones : public BlockSelector
        {
          private:
            Stones() = default;

          public:
            void selectBlocks(pcg32 &rand, int32_t x, int32_t y, int32_t z, bool wall) override;
        };

      protected:
        void writeStructureToNBT(NBTTagCompound *tagCompound) override;
        void readStructureFromNBT(NBTTagCompound *tagCompound, TemplateManager p_143011_2_) override;

      private:
        bool placedMainChest;
        bool placedHiddenChest;
        bool placedTrap1;
        bool placedTrap2;
        static JunglePyramid::Stones cobblestoneSelector;
    };

    class Igloo : public Feature
    {
      public:
        Igloo() = default;
        Igloo(pcg32 &rand, int32_t x, int32_t z);
        bool addComponentParts(World *worldIn, pcg32 &randomIn, StructureBoundingBox structureBoundingBoxIn) override;

      private:
        static ResourceLocation IGLOO_TOP_ID;
        static ResourceLocation IGLOO_MIDDLE_ID;
        static ResourceLocation IGLOO_BOTTOM_ID;
    };

    class DesertPyramid : public Feature
    {
      public:
        DesertPyramid() = default;
        DesertPyramid(pcg32 &rand, int32_t x, int32_t z);
        bool addComponentParts(World *worldIn, pcg32 &randomIn, StructureBoundingBox structureBoundingBoxIn) override;

      protected:
        void writeStructureToNBT(NBTTagCompound *tagCompound) override;
        void readStructureFromNBT(NBTTagCompound *tagCompound, TemplateManager p_143011_2_) override;

      private:
        std::array<bool, 4> hasPlacedChest;
    };
};
