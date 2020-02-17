#pragma once
#include "Biome.h"

class BiomeForest :public Biome
{
public:
	enum class Type
	{
		NORMAL,
		FLOWER,
		BIRCH,
		ROOFED
	};

	BiomeForest(Type typeIn, BiomeProperties properties);
	WorldGenAbstractTree getRandomTreeFeature(pcg32& rand) override;
	BlockFlower.EnumFlowerType pickRandomFlower(pcg32& rand, BlockPos& pos) override;
	void decorate(World* worldIn, pcg32& rand, BlockPos& pos) override;
	int getGrassColorAtPos(BlockPos& pos) override;
protected:
	static WorldGenBirchTree SUPER_BIRCH_TREE = new WorldGenBirchTree(false, true);
	static WorldGenBirchTree BIRCH_TREE = new WorldGenBirchTree(false, false);
	static WorldGenCanopyTree ROOF_TREE = new WorldGenCanopyTree(false);

	void addMushrooms(World* p_185379_1_, pcg32& p_185379_2_, BlockPos& p_185379_3_);
	void addDoublePlants(World* p_185378_1_, pcg32& p_185378_2_, BlockPos& p_185378_3_, int32_t p_185378_4_);
private:
	Type type;
};
