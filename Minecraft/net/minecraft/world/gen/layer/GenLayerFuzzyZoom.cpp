#include "GenLayerFuzzyZoom.h"

GenLayerFuzzyZoom::GenLayerFuzzyZoom(int64_t p_i2123_1_, std::shared_ptr<GenLayer> p_i2123_3_)
	:GenLayerZoom(p_i2123_1_, p_i2123_3_)
{
}

int32_t GenLayerFuzzyZoom::selectModeOrRandom(int32_t p_151617_1_, int32_t p_151617_2_, int32_t p_151617_3_,
	int32_t p_151617_4_)
{
	return selectRandom({p_151617_1_, p_151617_2_, p_151617_3_, p_151617_4_});
}
