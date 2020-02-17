#include "ChunkGeneratorDebug.h"
#include "chunk/ChunkPrimer.h"
#include "biome/Biome.h"
#include "chunk/Chunk.h"


std::vector<IBlockState*> getStates()
{
	std::vector<IBlockState*> temp;
	for (auto block : Block::REGISTRY)
	{
		auto states = block->getBlockState().getValidStates();
		temp.assign(states.begin(), states.end());
	}
}


std::vector<IBlockState*> ChunkGeneratorDebug::ALL_VALID_STATES = getStates();

IBlockState* ChunkGeneratorDebug::AIR = Blocks::AIR.getDefaultState();
IBlockState* ChunkGeneratorDebug::BARRIER = Blocks::BARRIER.getDefaultState();

int32_t ChunkGeneratorDebug::GRID_WIDTH = MathHelper::ceil(MathHelper::sqrt((float)ALL_VALID_STATES.size()));
int32_t ChunkGeneratorDebug::GRID_HEIGHT = MathHelper::ceil((float)ALL_VALID_STATES.size() / (float)GRID_WIDTH);

ChunkGeneratorDebug::ChunkGeneratorDebug(World* worldIn)
	:world(worldIn)
{
	
}

Chunk ChunkGeneratorDebug::generateChunk(int32_t x, int32_t z)
{
	ChunkPrimer chunkprimer;

	for (auto i = 0; i < 16; ++i) 
	{
		for (auto j = 0; j < 16; ++j) 
		{
			auto k = x * 16 + i;
			auto i1 = z * 16 + j;
			chunkprimer.setBlockState(i, 60, j, BARRIER);
			auto iblockstate = getBlockStateFor(k, i1);
			if (iblockstate != nullptr) 
			{
				chunkprimer.setBlockState(i, 70, j, iblockstate);
			}
		}
	}

	Chunk chunk(world, chunkprimer, x, z);
	chunk.generateSkylightMap();
	auto abiome = world->getBiomeProvider().getBiomes(std::vector<Biome*>(), x * 16, z * 16, 16, 16);
	auto abyte = chunk.getBiomeArray();

	for (auto i1 = 0; i1 < abyte.size(); ++i1) 
	{
		abyte[i1] = Biome::getIdForBiome(abiome[i1]);
	}

	chunk.generateSkylightMap();
	return chunk;
}

IBlockState* ChunkGeneratorDebug::getBlockStateFor(int32_t p_177461_0_, int32_t p_177461_1_) const
{
	IBlockState* iblockstate = AIR;
	if (p_177461_0_ > 0 && p_177461_1_ > 0 && p_177461_0_ % 2 != 0 && p_177461_1_ % 2 != 0) 
	{
		p_177461_0_ /= 2;
		p_177461_1_ /= 2;
		if (p_177461_0_ <= GRID_WIDTH && p_177461_1_ <= GRID_HEIGHT) {
			auto i = MathHelper::abs(p_177461_0_ * GRID_WIDTH + p_177461_1_);
			if (i < ALL_VALID_STATES.size()) 
			{
				iblockstate = ALL_VALID_STATES[i];
			}
		}
	}

	return iblockstate;
}

void ChunkGeneratorDebug::populate(int32_t x, int32_t z)
{
}

bool ChunkGeneratorDebug::generateStructures(Chunk& chunkIn, int32_t x, int32_t z)
{
	return false;
}

std::vector<SpawnListEntry> ChunkGeneratorDebug::getPossibleCreatures(EnumCreatureType creatureType, BlockPos& pos)
{
	auto biome = world->getBiome(pos);
	return biome.getSpawnableList(creatureType);
}

std::optional<BlockPos> ChunkGeneratorDebug::getNearestStructurePos(World* worldIn, std::string_view structureName,
	BlockPos& position, bool findUnexplored)
{
	return std::nullopt;
}

bool ChunkGeneratorDebug::isInsideStructure(World* worldIn, std::string_view structureName, BlockPos& pos)
{
	return false;
}

void ChunkGeneratorDebug::recreateStructures(Chunk& chunkIn, int32_t x, int32_t z)
{
}
