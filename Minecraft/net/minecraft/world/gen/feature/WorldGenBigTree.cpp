#include "WorldGenBigTree.h"
#include "math/MathHelper.h"
#include "material/Material.h"
#include "World.h"

WorldGenBigTree::WorldGenBigTree(bool notify)
	:WorldGenAbstractTree(notify), basePos(BlockPos::ORIGIN), heightAttenuation(0.618), branchSlope(0.381)
	 , scaleWidth(1.0), leafDensity(1.0), trunkSize(1), heightLimitLimit(12), leafDistanceLimit(4)
{
}

void WorldGenBigTree::setDecorationDefaults()
{
	leafDistanceLimit = 5;
}

bool WorldGenBigTree::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	world = worldIn;
	basePos = position;
	rand = pcg32(MathHelper::nextLong(rand));
	if (heightLimit == 0) 
	{
		heightLimit = 5 + rand(heightLimitLimit);
	}

	if (!validTreeLocation()) 
	{
		return false;
	}
	else 
	{
		generateLeafNodeList();
		generateLeaves();
		generateTrunk();
		generateLeafNodeBases();
		return true;
	}
}

void WorldGenBigTree::generateLeafNodeList()
{
	height = (int)((double)heightLimit * heightAttenuation);
	if (height >= heightLimit) 
	{
		height = heightLimit - 1;
	}

	auto i = (int)(1.382 + MathHelper::pow(leafDensity * (double)heightLimit / 13.0, 2.0));
	if (i < 1) 
	{
		i = 1;
	}

	auto j = basePos.gety() + height;
	auto k = heightLimit - leafDistanceLimit;
	foliageCoords.emplace_back(FoliageCoordinates(basePos.up(k), j));

	for (; k >= 0; --k) 
	{
		auto f = layerSize(k);
		if (f >= 0.0F) 
		{
			for (auto l = 0; l < i; ++l) 
			{
				auto d0 = scaleWidth * (double)f * ((double)MathHelper::nextDouble(rand) + 0.328);

				auto d1 = (double)(MathHelper::nextFloat(rand) * 2.0F) * 3.141592653589793;
				auto d2 = d0 * MathHelper::sin(d1) + 0.5;
				auto d3 = d0 * MathHelper::cos(d1) + 0.5;
				auto blockpos = basePos.add(d2, (double)(k - 1), d3);
				auto blockpos1 = blockpos.up(leafDistanceLimit);
				if (checkBlockLine(blockpos, blockpos1) == -1) 
				{
					auto i1 = basePos.getx() - blockpos.getx();
					auto j1 = basePos.getz() - blockpos.getz();
					auto d4 = (double)blockpos.gety() - MathHelper::sqrt((double)(i1 * i1 + j1 * j1)) * branchSlope;
					auto k1 = d4 > (double)j ? j : (int)d4;
					BlockPos blockpos2(basePos.getx(), k1, basePos.getz());
					if (checkBlockLine(blockpos2, blockpos) == -1) 
					{
						foliageCoords.emplace_back(FoliageCoordinates(blockpos, blockpos2.gety()));
					}
				}
			}
		}
	}
}

void WorldGenBigTree::crosSection(const BlockPos& pos, float p_181631_2_, IBlockState* p_181631_3_)
{
	auto i = (int)((double)p_181631_2_ + 0.618);

	for (auto j = -i; j <= i; ++j) 
	{
		for (auto k = -i; k <= i; ++k) 
		{
			if (MathHelper::pow((double)MathHelper::abs(j) + 0.5, 2.0) + MathHelper::pow((double)MathHelper::abs(k) + 0.5, 2.0) <= (double)(p_181631_2_ * p_181631_2_)) {
				BlockPos blockpos = pos.add(j, 0, k);
				Material material = world->getBlockState(blockpos).getMaterial();
				if (material == Material::AIR || material == Material::LEAVES) 
				{
					setBlockAndNotifyAdequately(world, blockpos, p_181631_3_);
				}
			}
		}
	}
}

float WorldGenBigTree::layerSize(int32_t y) const
{
	if ((float)y < (float)heightLimit * 0.3F) 
	{
		return -1.0F;
	}
	else 
	{
		auto f = (float)heightLimit / 2.0F;
		auto f1 = f - (float)y;
		auto f2 = MathHelper::sqrt(f * f - f1 * f1);
		if (f1 == 0.0F) 
		{
			f2 = f;
		}
		else if (MathHelper::abs(f1) >= f)
		{
			return 0.0F;
		}

		return f2 * 0.5F;
	}
}

float WorldGenBigTree::leafSize(int32_t y) const
{
	if (y >= 0 && y < leafDistanceLimit) 
	{
		return y != 0 && y != leafDistanceLimit - 1 ? 3.0F : 2.0F;
	}
	else 
	{
		return -1.0F;
	}
}

void WorldGenBigTree::generateLeafNode(const BlockPos& pos)
{
	for (auto i = 0; i < leafDistanceLimit; ++i) 
	{
		crosSection(pos.up(i), leafSize(i), Blocks.LEAVES.getDefaultState().withProperty(BlockLeaves.CHECK_DECAY, false));
	}
}

void WorldGenBigTree::limb(const BlockPos& p_175937_1_, const BlockPos& p_175937_2_, Block* p_175937_3_)
{
	auto blockpos = p_175937_2_.add(-p_175937_1_.getx(), -p_175937_1_.gety(), -p_175937_1_.getz());
	auto i = getGreatestDistance(blockpos);
	auto f = (float)blockpos.getx() / (float)i;
	auto f1 = (float)blockpos.gety() / (float)i;
	auto f2 = (float)blockpos.getz() / (float)i;

	for (auto j = 0; j <= i; ++j) 
	{
		auto blockpos1 = p_175937_1_.add((double)(0.5F + (float)j * f), (double)(0.5F + (float)j * f1), (double)(0.5F + (float)j * f2));
		BlockLog::EnumAxis blocklog$enumaxis = getLogAxis(p_175937_1_, blockpos1);
		setBlockAndNotifyAdequately(world, blockpos1, p_175937_3_.getDefaultState().withProperty(BlockLog.LOG_AXIS, blocklog$enumaxis));
	}
}

int32_t WorldGenBigTree::getGreatestDistance(const BlockPos& posIn) const
{
	auto i = MathHelper::abs(posIn.getx());
	auto j = MathHelper::abs(posIn.gety());
	auto k = MathHelper::abs(posIn.getz());
	if (k > i && k > j) 
	{
		return k;
	}
	else 
	{
		return j > i ? j : i;
	}
}

BlockLog::EnumAxis WorldGenBigTree::getLogAxis(const BlockPos& p_175938_1_, const BlockPos& p_175938_2_)
{
	BlockLog::EnumAxis blocklog$enumaxis = BlockLog::EnumAxis.Y;
	auto i = MathHelper::abs(p_175938_2_.getx() - p_175938_1_.getx());
	auto j = MathHelper::abs(p_175938_2_.getz() - p_175938_1_.getz());
	auto k = MathHelper::max(i, j);
	if (k > 0) 
	{
		if (i == k) 
		{
			blocklog$enumaxis = BlockLog::EnumAxis.X;
		}
		else if (j == k) {
			blocklog$enumaxis = BlockLog::EnumAxis.Z;
		}
	}

	return blocklog$enumaxis;
}

void WorldGenBigTree::generateLeaves()
{
	for(auto worldgenbigtree$foliagecoordinates : foliageCoords)
	{
		generateLeafNode(worldgenbigtree$foliagecoordinates);
	}
}

bool WorldGenBigTree::leafNodeNeedsBase(int32_t p_76493_1_)
{
	return (double)p_76493_1_ >= (double)heightLimit * 0.2;
}

void WorldGenBigTree::generateTrunk()
{
	BlockPos blockpos = basePos;
	BlockPos blockpos1 = basePos.up(height);
	auto block = Blocks::LOG;
	limb(blockpos, blockpos1, block);
	if (trunkSize == 2) {
		limb(blockpos.east(), blockpos1.east(), block);
		limb(blockpos.east().south(), blockpos1.east().south(), block);
		limb(blockpos.south(), blockpos1.south(), block);
	}
}

void WorldGenBigTree::generateLeafNodeBases()
{
	for(auto worldgenbigtree$foliagecoordinates : foliageCoords)
	{
		auto i = worldgenbigtree$foliagecoordinates.getBranchBase();
		BlockPos blockpos(basePos.getx(), i, basePos.getz());
		if (!blockpos == (worldgenbigtree$foliagecoordinates) && leafNodeNeedsBase(i - basePos.gety())) {
			limb(blockpos, worldgenbigtree$foliagecoordinates, Blocks::LOG);
		}
	}
}

int32_t WorldGenBigTree::checkBlockLine(const BlockPos& posOne, const BlockPos& posTwo)
{
	BlockPos blockpos = posTwo.add(-posOne.getx(), -posOne.gety(), -posOne.getz());
	int i = getGreatestDistance(blockpos);
	float f = (float)blockpos.getx() / (float)i;
	float f1 = (float)blockpos.gety() / (float)i;
	float f2 = (float)blockpos.getz() / (float)i;
	if (i == 0) {
		return -1;
	}
	else {
		for (int j = 0; j <= i; ++j) {
			BlockPos blockpos1 = posOne.add((double)(0.5F + (float)j * f), (double)(0.5F + (float)j * f1), (double)(0.5F + (float)j * f2));
			if (!canGrowInto(world->getBlockState(blockpos1).getBlock())) {
				return j;
			}
		}

		return -1;
	}
}

bool WorldGenBigTree::validTreeLocation()
{
	auto block = world->getBlockState(basePos.down()).getBlock();
	if (block != Blocks::DIRT && block != Blocks::GRASS && block != Blocks::FARMLAND) 
	{
		return false;
	}
	else 
	{
		auto i = checkBlockLine(basePos, basePos.up(heightLimit - 1));
		if (i == -1) 
		{
			return true;
		}
		else if (i < 6) 
		{
			return false;
		}
		else 
		{
			heightLimit = i;
			return true;
		}
	}
}
