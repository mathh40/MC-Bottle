#include "WorldGenDesertWells.h"

BlockStateMatcher WorldGenDesertWells::IS_SAND = BlockStateMatcher.forBlock(Blocks.SAND).where(BlockSand.VARIANT, Predicates.equalTo(BlockSand.EnumType.SAND));

WorldGenDesertWells::WorldGenDesertWells()
	: sandSlab(Blocks.STONE_SLAB.getDefaultState().withProperty(BlockStoneSlab.VARIANT, BlockStoneSlab.EnumType.SAND).withProperty(BlockSlab.HALF, BlockSlab.EnumBlockHalf.BOTTOM)),
	  sandstone(Blocks.SANDSTONE.getDefaultState()),
	  water(Blocks.FLOWING_WATER.getDefaultState())
{
}

bool WorldGenDesertWells::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	while (worldIn.isAirBlock(position) && position.getY() > 2) {
		position = position.down();
	}

	if (!IS_SAND.apply(worldIn.getBlockState(position))) {
		return false;
	}
	else {
		int j1;
		int j2;
		for (j1 = -2; j1 <= 2; ++j1) {
			for (j2 = -2; j2 <= 2; ++j2) {
				if (worldIn.isAirBlock(position.add(j1, -1, j2)) && worldIn.isAirBlock(position.add(j1, -2, j2))) {
					return false;
				}
			}
		}

		for (j1 = -1; j1 <= 0; ++j1) {
			for (j2 = -2; j2 <= 2; ++j2) {
				for (int k = -2; k <= 2; ++k) {
					worldIn.setBlockState(position.add(j2, j1, k), this.sandstone, 2);
				}
			}
		}

		worldIn.setBlockState(position, this.water, 2);
		Iterator var7 = EnumFacing.Plane.HORIZONTAL.iterator();

		while (var7.hasNext()) {
			EnumFacing enumfacing = (EnumFacing)var7.next();
			worldIn.setBlockState(position.offset(enumfacing), this.water, 2);
		}

		for (j1 = -2; j1 <= 2; ++j1) {
			for (j2 = -2; j2 <= 2; ++j2) {
				if (j1 == -2 || j1 == 2 || j2 == -2 || j2 == 2) {
					worldIn.setBlockState(position.add(j1, 1, j2), this.sandstone, 2);
				}
			}
		}

		worldIn.setBlockState(position.add(2, 1, 0), this.sandSlab, 2);
		worldIn.setBlockState(position.add(-2, 1, 0), this.sandSlab, 2);
		worldIn.setBlockState(position.add(0, 1, 2), this.sandSlab, 2);
		worldIn.setBlockState(position.add(0, 1, -2), this.sandSlab, 2);

		for (j1 = -1; j1 <= 1; ++j1) {
			for (j2 = -1; j2 <= 1; ++j2) {
				if (j1 == 0 && j2 == 0) {
					worldIn.setBlockState(position.add(j1, 4, j2), this.sandstone, 2);
				}
				else {
					worldIn.setBlockState(position.add(j1, 4, j2), this.sandSlab, 2);
				}
			}
		}

		for (j1 = 1; j1 <= 3; ++j1) {
			worldIn.setBlockState(position.add(-1, j1, -1), this.sandstone, 2);
			worldIn.setBlockState(position.add(-1, j1, 1), this.sandstone, 2);
			worldIn.setBlockState(position.add(1, j1, -1), this.sandstone, 2);
			worldIn.setBlockState(position.add(1, j1, 1), this.sandstone, 2);
		}

		return true;
	}
}
