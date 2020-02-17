#pragma once
#include "MapGenStructure.h"
class MapGenVillage :public MapGenStructure
{
public:
	static std::vector<Biome*> VILLAGE_SPAWN_BIOMES;

	MapGenVillage() = default;
	MapGenVillage(const std::unordered_map<std::string,std::string>& map);

	std::string getStructureName() override;
	std::optional<BlockPos> getNearestStructurePos(World* worldIn, BlockPos& pos, bool findUnexplored) override;

	class Start :public StructureStart
	{
	public:
		Start() = default;
		Start(World* worldIn, pcg32& rand, int32_t x, int32_t z, int32_t size);
		bool isSizeableStructure() override;
		void writeToNBT(NBTTagCompound* tagCompound) override;
		void readFromNBT(NBTTagCompound* tagCompound) override;
	private:
		bool hasMoreThanTwoComponents;
	};
protected:
	bool canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ) override;
	StructureStart getStructureStart(int32_t chunkX, int32_t chunkZ) override;	
private:
	int32_t size = 0;
	int32_t distance = 32;
	const int32_t minTownSeparation = 8;
};