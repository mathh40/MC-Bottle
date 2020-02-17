#pragma once
#include "GenLayer.h"
#include <memory>

class GenLayerZoom : public GenLayer
{
public:
	GenLayerZoom(int64_t p_i2134_1_, std::shared_ptr<GenLayer> p_i2134_3_);
	std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;
	static std::shared_ptr<GenLayer> magnify(int64_t p_75915_0_, std::shared_ptr<GenLayer> p_75915_2_, int32_t p_75915_3_);
private:
};
