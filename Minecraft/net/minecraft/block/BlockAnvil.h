#pragma once
#include "BlockFalling.h"
#include "../util/text/ITextComponent.h"
#include "../world/IInteractionObject.h"
#include "properties/PropertyDirection.h"
#include "properties/PropertyInteger.h"
#include "state/BlockStateContainer.h"
#include "../../../../spdlog/include/spdlog/logger.h"

class BlockAnvil :public BlockFalling
{
public:
	static PropertyDirection FACING;
	static PropertyInteger DAMAGE;

	bool isFullCube(IBlockState* state)override;
	BlockFaceShape getBlockFaceShape(IBlockAccess worldIn, IBlockState state, BlockPos pos, EnumFacing face)override;
	bool isOpaqueCube(IBlockState* state)override;
	IBlockState* getStateForPlacement(World* worldIn, BlockPos pos, EnumFacing facing, float hitX, float hitY, float hitZ, int32_t meta, EntityLivingBase placer)override;
	bool onBlockActivated(World* worldIn, BlockPos pos, IBlockState state, EntityPlayer playerIn, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ)override;
	int32_t damageDropped(IBlockState* state)override;
	AxisAlignedBB getBoundingBox(IBlockState* state, IBlockAccess source, BlockPos pos)override;
	void getSubBlocks(CreativeTabs itemIn, std::vector<ItemStack>& items)override;
	void onEndFalling(World* worldIn, BlockPos pos, IBlockState* fallingState, * hitState)override;
	void onBroken(World* worldIn, BlockPos pos)override;
	bool shouldSideBeRendered(IBlockState* blockState, IBlockAccess* blockAccess, BlockPos pos, EnumFacing side)override;
	IBlockState* getStateFromMeta(uint16_t meta)override;
	uint16_t getMetaFromState(IBlockState* state)override;
	IBlockState* withRotation(IBlockState* state, Rotation rot)override;

	static class Anvil :public IInteractionObject
	{
	protected:
		~Anvil() = default;
	public:
		Anvil(World* worldIn, BlockPos pos);
		std::string getName() const override;
		bool hasCustomName() const override;
		std::unique_ptr<ITextComponent> getDisplayName()const override;
		Container createContainer(InventoryPlayer playerInventory, EntityPlayer playerIn);
		std::string getGuiID() const override;
	private:
		World* world;
		BlockPos position;
	};

protected:
	static AxisAlignedBB X_AXIS_AABB;
	static AxisAlignedBB Z_AXIS_AABB;
	static std::shared_ptr<spdlog::logger> LOGGER;

	BlockAnvil();

	void onStartFalling(EntityFallingBlock fallingEntity)override;
	BlockStateContainer createBlockState()override;
private:
};
