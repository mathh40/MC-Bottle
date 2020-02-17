#pragma once
#include "GenLayer.h"

class GenLayerAddSnow : public GenLayer
{
public:
	GenLayerAddSnow(int64_t p_i2121_1_, std::shared_ptr<GenLayer> p_i2121_3_);
	std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;
private:
};