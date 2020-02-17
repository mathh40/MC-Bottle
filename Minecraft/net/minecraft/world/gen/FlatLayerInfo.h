#pragma once
#include "state/IBlockState.h"

class Block;

class FlatLayerInfo
{
public:
	FlatLayerInfo(int32_t p_i45467_1_, Block* layerMaterialIn);
	FlatLayerInfo(int32_t p_i45627_1_, int32_t height, Block* layerMaterialIn);
	FlatLayerInfo(int32_t p_i45628_1_, int32_t p_i45628_2_, Block* layerMaterialIn, int32_t p_i45628_4_);

	int32_t getLayerCount() const;
	IBlockState* getLayerMaterial() const;
	int32_t getMinY() const;
	void setMinY(int32_t minY);
	std::string toString();
private:
	int32_t version;
	IBlockState* layerMaterial;
	int32_t layerCount;
	int32_t layerMinimumY;

	Block* getLayerMaterialBlock();
	int32_t getFillBlockMeta();
};
