#pragma once
#include <array>

namespace ColorizerGrass
{
void setGrassBiomeColorizer(const std::array<int32_t, 65536> &grassBufferIn);
int32_t getGrassColor(double temperature, double humidity);
} // namespace ColorizerGrass
