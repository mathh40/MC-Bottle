#pragma once
class IBlockState;
class BlockPos;
class World;

class WorldGenerator
{
public:
	virtual ~WorldGenerator() = default;
	WorldGenerator();
	explicit WorldGenerator(bool notify);
	virtual bool generate(World* var1, Random& var2, const BlockPos& var3) = 0;
	virtual void setDecorationDefaults();
protected:
	void setBlockAndNotifyAdequately(World* worldIn, const BlockPos& pos, IBlockState* state);
private:
	bool doBlockNotify;
};
