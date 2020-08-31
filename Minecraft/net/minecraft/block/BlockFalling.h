#pragma once

class EntityFallingBlock;
class BlockFalling : public Block
{
public:
	static bool fallInstantly;

	BlockFalling();
	BlockFalling(Material materialIn);

	void onBlockAdded(World* worldIn, const BlockPos& pos, IBlockState* state)override;
	void neighborChanged(IBlockState* state, World* worldIn, const BlockPos& pos, Block* blockIn, const BlockPos& fromPos)override;
	void updateTick(World* worldIn, const BlockPos& pos, IBlockState* state, pcg32& rand)override;
	int32_t tickRate(World* worldIn)override;
	static bool canFallThrough(IBlockState* state);
	virtual void onEndFalling(World* worldIn, const BlockPos& pos, IBlockState* fallingState, IBlockState* hitState);
	virtual void onBroken(World* worldIn, const BlockPos& pos);
	void randomDisplayTick(IBlockState* stateIn, World* worldIn, const BlockPos& pos, pcg32& rand)override;
	int32_t getDustColor(IBlockState* state);
protected:
	~BlockFalling() = default;
	virtual void onStartFalling(EntityFallingBlock* fallingEntity);
private:
	void checkFallable(World* worldIn, const BlockPos& pos);
};
