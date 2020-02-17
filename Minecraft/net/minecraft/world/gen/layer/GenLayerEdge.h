#pragma once
#include "GenLayer.h"

class GenLayerEdge : public GenLayer
{
public:
	enum class Mode
	{
		COOL_WARM,
		HEAT_ICE,
		SPECIAL
	};

	std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;
	GenLayerEdge(int64_t p_i45474_1_, std::shared_ptr<GenLayer> p_i45474_3_, GenLayerEdge::Mode p_i45474_4_);
private:
	GenLayerEdge::Mode mode;

	std::vector<int32_t> getIntsCoolWarm(int32_t p_151626_1_, int32_t p_151626_2_, int32_t p_151626_3_, int32_t p_151626_4_);
	std::vector<int32_t> getIntsHeatIce(int32_t p_151624_1_, int32_t p_151624_2_, int32_t p_151624_3_, int32_t p_151624_4_);
	std::vector<int32_t> getIntsSpecial(int32_t p_151625_1_, int32_t p_151625_2_, int32_t p_151625_3_, int32_t p_151625_4_);
};