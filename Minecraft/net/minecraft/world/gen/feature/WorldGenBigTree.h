#pragma once
#include "WorldGenAbstractTree.h"
#include "math/BlockPos.h"

class WorldGenBigTree :public WorldGenAbstractTree
{
public:

	class FoliageCoordinates :public BlockPos 
	{
	private: 
		int32_t branchBase;

	public: 
		FoliageCoordinates(const BlockPos& pos, int32_t p_i45635_2_) 
		{
			BlockPos(pos.getx(), pos.gety(), pos.getz());
			branchBase = p_i45635_2_;
		}

		int32_t getBranchBase() const
		{
			return branchBase;
		}
	};

	explicit WorldGenBigTree(bool notify);

	void setDecorationDefaults() override;
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	pcg32 rand;
	World* world{};
	BlockPos basePos;
	int32_t heightLimit{};
	int32_t height{};
	double heightAttenuation;
	double branchSlope;
	double scaleWidth;
	double leafDensity;
	int32_t trunkSize;
	int32_t heightLimitLimit;
	int32_t leafDistanceLimit;
	std::vector<FoliageCoordinates> foliageCoords{};

	void generateLeafNodeList();
	void crosSection(const BlockPos& pos, float p_181631_2_, IBlockState* p_181631_3_);
	float layerSize(int32_t y) const;
	float leafSize(int32_t y) const;
	void generateLeafNode(const BlockPos& pos);
	void limb(const BlockPos& p_175937_1_, const BlockPos& p_175937_2_, Block* p_175937_3_);
	int32_t getGreatestDistance(const BlockPos& posIn) const;
	BlockLog::EnumAxis getLogAxis(const BlockPos& p_175938_1_, const BlockPos& p_175938_2_);
	void generateLeaves();
	bool leafNodeNeedsBase(int32_t p_76493_1_);
	void generateTrunk();
	void generateLeafNodeBases();
	int32_t checkBlockLine(const BlockPos& posOne, const BlockPos& posTwo);
	bool validTreeLocation();
};
