#pragma once
#include "MapGenStructure.h"
#include "gen/ChunkGeneratorEnd.h"

class MapGenEndCity :public MapGenStructure
{
public:
   MapGenEndCity(ChunkGeneratorEnd endProviderIn);
   std::string getStructureName() override;
   BlockPos getNearestStructurePos(World* worldIn, BlockPos pos, bool findUnexplored);

   class Start :public StructureStart
   {
   private:
      void create(World* worldIn, ChunkGeneratorEnd chunkProvider, pcg32& rnd, int32_t chunkX, int32_t chunkZ);
      bool isSizeable;
   public:
      Start() = default;
      Start(World* worldIn, ChunkGeneratorEnd chunkProvider, pcg32& random, int32_t chunkX, int32_t chunkZ);
      bool isSizeableStructure() override;
   };

protected:
   bool canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ) override;
   StructureStart getStructureStart(int32_t chunkX, int32_t chunkZ) override;
private:
   int32_t citySpacing = 20;
   int32_t minCitySeparation = 11;
   ChunkGeneratorEnd endProvider;

   static int32_t getYPosForStructure(int32_t chunkX, int32_t chunkY, ChunkGeneratorEnd generatorIn);
};
