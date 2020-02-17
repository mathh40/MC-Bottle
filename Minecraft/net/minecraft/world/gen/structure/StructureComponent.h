#pragma once
#include "EnumFacing.h"
#include "Mirror.h"
#include "Rotation.h"
#include "NBTTagCompound.h"
#include "StructureBoundingBox.h"
#include "Rotation.h"
#include "state/IBlockState.h"

class ResourceLocation;
class TemplateManager;
class World;

class StructureComponent
{
public:
	virtual ~StructureComponent() = default;
	StructureComponent() = default;
	std::unique_ptr<NBTTagCompound> createStructureBaseNBT();
	void readStructureBaseNBT(World* worldIn, NBTTagCompound* tagCompound);
	static void buildComponent(StructureComponent* componentIn, const std::vector<StructureComponent*>& listIn, pcg32& rand);
	virtual bool addComponentParts(World* var1, pcg32& var2, StructureBoundingBox var3) = 0;
	StructureBoundingBox getBoundingBox() const;
	int32_t getComponentType() const;
	static StructureComponent* findIntersecting(const std::vector<StructureComponent*>& listIn, std::optional<StructureBoundingBox> boundingboxIn);
	virtual void offset(int32_t x, int32_t y, int32_t z);
	EnumFacing getCoordBaseMode() const;
	void setCoordBaseMode(EnumFacing facing);

	class BlockSelector
	{
	public:
		BlockSelector();
		virtual void selectBlocks(pcg32& var1, int32_t var2, int32_t var3, int32_t var4, bool var5) = 0;
		IBlockState* getBlockState() const;
	protected:
		IBlockState* blockstate;
	};

protected:
	explicit StructureComponent(int32_t type);

	StructureBoundingBox boundingBox;
	int32_t componentType;

	virtual void writeStructureToNBT(NBTTagCompound* var1) = 0;
	virtual void readStructureFromNBT(NBTTagCompound* var1, const TemplateManager& var2) = 0;
	bool isLiquidInStructureBoundingBox(World* worldIn, StructureBoundingBox boundingboxIn);
	int32_t getXWithOffset(int32_t x, int32_t z);
	int32_t getYWithOffset(int32_t y);
	int32_t getZWithOffset(int32_t x, int32_t z);
	void setBlockState(World* worldIn, IBlockState* blockstateIn, int32_t x, int32_t y, int32_t z, StructureBoundingBox boundingboxIn);
	IBlockState* getBlockStateFromPos(World* worldIn, int32_t x, int32_t y, int32_t z, StructureBoundingBox boundingboxIn);
	int32_t getSkyBrightness(World* worldIn, int32_t x, int32_t y, int32_t z, StructureBoundingBox boundingboxIn);
	void fillWithAir(World* worldIn, StructureBoundingBox structurebb, int32_t minX, int32_t minY, int32_t minZ, int32_t maxX, int32_t maxY, int32_t maxZ);
	void fillWithBlocks(World* worldIn, StructureBoundingBox boundingboxIn, int32_t xMin, int32_t yMin, int32_t zMin, int32_t xMax, int32_t yMax, int32_t zMax, IBlockState* boundaryBlockState, IBlockState* insideBlockState, bool existingOnly);
	void fillWithRandomizedBlocks(World* worldIn, StructureBoundingBox boundingboxIn, int32_t minX, int32_t minY, int32_t minZ, int32_t maxX, int32_t maxY, int32_t maxZ, bool alwaysReplace, pcg32& rand, StructureComponent::BlockSelector blockselector);
	void generateMaybeBox(World* worldIn, StructureBoundingBox sbb, pcg32& rand, float chance, int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2, IBlockState* edgeState, IBlockState* state, bool requireNonAir, int32_t requiredSkylight);
	void randomlyPlaceBlock(World* worldIn, StructureBoundingBox boundingboxIn, pcg32& rand, float chance, int32_t x, int32_t y, int32_t z, IBlockState* blockstateIn);
	void randomlyRareFillWithBlocks(World* worldIn, StructureBoundingBox boundingboxIn, int32_t minX, int32_t minY, int32_t minZ, int32_t maxX, int32_t maxY, int32_t maxZ, IBlockState* blockstateIn, bool excludeAir);
	void clearCurrentPositionBlocksUpwards(World* worldIn, int32_t x, int32_t y, int32_t z, StructureBoundingBox structurebb);
	void replaceAirAndLiquidDownwards(World* worldIn, IBlockState* blockstateIn, int32_t x, int32_t y, int32_t z, StructureBoundingBox boundingboxIn);
	virtual bool generateChest(World* worldIn, StructureBoundingBox structurebb, pcg32& randomIn, int32_t x, int32_t y, int32_t z, ResourceLocation loot);
	bool generateChest(World* p_191080_1_, StructureBoundingBox p_191080_2_, pcg32& p_191080_3_, BlockPos p_191080_4_, ResourceLocation p_191080_5_, IBlockState* p_191080_6_);
	bool createDispenser(World* worldIn, StructureBoundingBox sbb, pcg32& rand, int32_t x, int32_t y, int32_t z, EnumFacing facing, ResourceLocation lootTableIn);
	void generateDoor(World* worldIn, StructureBoundingBox sbb, pcg32& rand, int32_t x, int32_t y, int32_t z, EnumFacing facing, BlockDoor door);
	
private:	
	EnumFacing coordBaseMode;
	Mirror mirror;
	Rotation rotation;
};
