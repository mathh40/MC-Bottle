#include "WorldGenSpikes.h"
#include "math/MathHelper.h"
#include "World.h"

WorldGenSpikes::EndSpike::EndSpike(int32_t centerXIn, int32_t centerZIn, int32_t radiusIn, int32_t heightIn,
                                   bool guardedIn)
	: centerX(centerXIn), centerZ(centerZIn), radius(radiusIn), height(heightIn), guarded(guardedIn)
	  , topBoundingBox((double)( centerXIn - radiusIn), 0.0, (double)(centerZIn - radiusIn), (double)( centerXIn + radiusIn), 256.0, (double)(centerZIn + radiusIn))
{
}

bool WorldGenSpikes::EndSpike::doesStartInChunk(const BlockPos& p_186154_1_) const
{
	auto i = centerX - radius;
	auto j = centerZ - radius;
	return p_186154_1_.getx() == (i & -16) && p_186154_1_.getz() == (j & -16);
}

int32_t WorldGenSpikes::EndSpike::getCenterX() const
{
	return centerX;
}

int32_t WorldGenSpikes::EndSpike::getCenterZ() const
{
	return centerZ;
}

int32_t WorldGenSpikes::EndSpike::getRadius() const
{
	return radius;
}

int32_t WorldGenSpikes::EndSpike::getHeight() const
{
	return height;
}

bool WorldGenSpikes::EndSpike::isGuarded() const
{
	return guarded;
}

const AxisAlignedBB WorldGenSpikes::EndSpike::getTopBoundingBox() const
{
	return topBoundingBox;
}

void WorldGenSpikes::setSpike(const EndSpike& p_186143_1_)
{
	spike = p_186143_1_;
}

void WorldGenSpikes::setCrystalInvulnerable(bool p_186144_1_)
{
	crystalInvulnerable = p_186144_1_;
}

bool WorldGenSpikes::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	auto i = spike.getRadius();

	auto var5 = getAllInBoxMutable(BlockPos(position.getx() - i, 0, position.getz() - i), BlockPos(position.getx() + i, spike.getHeight() + 10, position.getz() + i);

	while (true) 
	{
		for(auto blockpos$mutableblockpos : var5)
		{
			if (blockpos$mutableblockpos.distanceSq((double)position.getx(), (double)blockpos$mutableblockpos.gety(), (double)position.getz()) <= (double)(i * i + 1) && blockpos$mutableblockpos.gety() < spike.getHeight()) 
			{
				setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks::OBSIDIAN.getDefaultState());
			}
			else if (blockpos$mutableblockpos.gety() > 65) 
			{
				setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks::AIR.getDefaultState());
			}
		}

		if (spike.isGuarded()) 
		{
			for (auto j = -2; j <= 2; ++j) 
			{
				for (auto k = -2; k <= 2; ++k)
				{
					if (MathHelper::abs(j) == 2 || MathHelper::abs(k) == 2) 
					{
						setBlockAndNotifyAdequately(worldIn, BlockPos(position.getx() + j, spike.getHeight(), position.getz() + k), Blocks::IRON_BARS.getDefaultState());
						setBlockAndNotifyAdequately(worldIn, BlockPos(position.getx() + j, spike.getHeight() + 1, position.getz() + k), Blocks::IRON_BARS.getDefaultState());
						setBlockAndNotifyAdequately(worldIn, BlockPos(position.getx() + j, spike.getHeight() + 2, position.getz() + k), Blocks::IRON_BARS.getDefaultState());
					}

					setBlockAndNotifyAdequately(worldIn, BlockPos(position.getx() + j, spike.getHeight() + 3, position.getz() + k), Blocks::IRON_BARS.getDefaultState());
				}
			}
		}

		EntityEnderCrystal entityendercrystal(worldIn);
		entityendercrystal.setBeamTarget(beamTarget);
		entityendercrystal.setEntityInvulnerable(crystalInvulnerable);
		entityendercrystal.setLocationAndAngles((double)((float)position.getx() + 0.5F), (double)(spike.getHeight() + 1), (double)((float)position.getz() + 0.5F), MathHelper::nextFloat(rand) * 360.0F, 0.0F);
		worldIn->spawnEntity(entityendercrystal);
		setBlockAndNotifyAdequately(worldIn, BlockPos(position.getx(), spike.getHeight(), position.getz()), Blocks::BEDROCK.getDefaultState());
		return true;
	}
}

void WorldGenSpikes::setBeamTarget(std::optional<BlockPos> pos)
{
	beamTarget = pos.value();
}
