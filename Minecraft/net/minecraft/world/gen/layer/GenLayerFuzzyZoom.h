#pragma once
#include "GenLayer.h"

class GenLayerFuzzyZoom : public GenLayerZoom
{
public:
	GenLayerFuzzyZoom(int64_t p_i2123_1_, std::shared_ptr<GenLayer> p_i2123_3_);
protected:
	int32_t selectModeOrRandom(int32_t p_151617_1_, int32_t p_151617_2_, int32_t p_151617_3_, int32_t p_151617_4_);
private:
};