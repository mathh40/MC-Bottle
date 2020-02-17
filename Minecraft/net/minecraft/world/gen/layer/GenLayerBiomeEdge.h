#pragma once
#include "GenLayer.h"

class GenLayerBiomeEdge : public GenLayer
{
public:
	GenLayerBiomeEdge(int64_t p_i45475_1_, std::shared_ptr<GenLayer> p_i45475_3_);
	std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;
private:
	bool replaceBiomeEdgeIfNecessary(const std::vector<int32_t>& p_151636_1_, std::vector<int32_t>& p_151636_2_, int32_t p_151636_3_, int32_t p_151636_4_, int32_t p_151636_5_, int32_t p_151636_6_, int32_t p_151636_7_, int32_t p_151636_8_);
	bool replaceBiomeEdge(const std::vector<int32_t>& p_151635_1_, std::vector<int32_t>& p_151635_2_, int32_t p_151635_3_, int32_t p_151635_4_, int32_t p_151635_5_, int32_t p_151635_6_, int32_t p_151635_7_, int32_t p_151635_8_);
	bool canBiomesBeNeighbors(int32_t p_151634_1_, int32_t p_151634_2_);
