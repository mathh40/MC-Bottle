#pragma once
#include "GenLayer.h"

class GenLayerSmooth : public GenLayer
{
public:
	GenLayerSmooth(int64_t p_i2131_1_, std::shared_ptr<GenLayer> p_i2131_3_);
	std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;
private:

};