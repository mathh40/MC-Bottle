#include "MapGenMineshaft.h"
#include "biome/BiomeMesa.h"

std::string MapGenMineshaft::getStructureName()
{
	return "Mineshaft";
}

MapGenMineshaft::MapGenMineshaft(const std::unordered_map<std::string,std::string>& p_i2034_1_)
{
	for(auto entry : p_i2034_1_)
	{
		if (entry.first == "chance") 
		{
			chance = MathHelper::getDouble(entry.second, chance);
		}
	}
}

std::optional<BlockPos> MapGenMineshaft::getNearestStructurePos(World* worldIn, const BlockPos& pos, bool findUnexplored)
{
	auto j = pos.getx() >> 4;
	auto k = pos.getz() >> 4;

	for(auto l = 0; l <= 1000; ++l) 
	{
		for(auto i1 = -l; i1 <= l; ++i1) 
		{
			auto flag = i1 == -l || i1 == l;

			for(int j1 = -l; j1 <= l; ++j1) 
			{
				auto flag1 = j1 == -l || j1 == l;
				if (flag || flag1) 
				{
					auto k1 = j + i1;
					auto l1 = k + j1;
					rand.seed((long)(k1 ^ l1) ^ worldIn->getSeed());
					rand();
					if (canSpawnStructureAtCoords(k1, l1) && (!findUnexplored || !worldIn->isChunkGeneratedAt(k1, l1))) 
					{
						return BlockPos((k1 << 4) + 8, 64, (l1 << 4) + 8);
					}
				}
			}
		}
	}

	return std::nullopt;
}

bool MapGenMineshaft::canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ)
{
	return MathHelper::nextDouble(rand) < chance && rand(80) < MathHelper::max(MathHelper::abs(chunkX), MathHelper::abs(chunkZ));
}

StructureStart MapGenMineshaft::getStructureStart(int32_t chunkX, int32_t chunkZ)
{
	auto biome = world->getBiome(BlockPos((chunkX << 4) + 8, 64, (chunkZ << 4) + 8));
	Type mapgenmineshaft$type = Util::instanceof<BiomeMesa>(biome) ? Type::MESA : Type::NORMAL;
	return StructureMineshaftStart(world, rand, chunkX, chunkZ, mapgenmineshaft$type);
}
