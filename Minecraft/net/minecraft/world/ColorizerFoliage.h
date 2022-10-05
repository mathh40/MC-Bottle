#pragma once
#include <array>

namespace ColorizerFoliage
{
void setFoliageBiomeColorizer(const std::array<int32_t, 65536> &foliageBufferIn);
int32_t getFoliageColor(double temperature, double humidity);
int32_t getFoliageColorPine();
int32_t getFoliageColorBirch();
int32_t getFoliageColorBasic();
} // namespace ColorizerFoliage
