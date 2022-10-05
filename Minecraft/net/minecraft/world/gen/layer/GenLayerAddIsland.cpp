#include "GenLayerAddIsland.h"

#include "IntCache.h"

GenLayerAddIsland::GenLayerAddIsland(int64_t p_i2119_1_, std::shared_ptr<GenLayer> p_i2119_3_)
    : GenLayer(p_i2119_1_), parent(p_i2119_3_)
{
}

std::vector<int32_t> GenLayerAddIsland::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
    auto i     = areaX - 1;
    auto j     = areaY - 1;
    auto k     = areaWidth + 2;
    auto l     = areaHeight + 2;
    auto aint  = parent->getInts(i, j, k, l);
    auto aint1 = IntCache::getIntCache(areaWidth * areaHeight);

    for (auto i1 = 0; i1 < areaHeight; ++i1)
    {
        for (auto j1 = 0; j1 < areaWidth; ++j1)
        {
            auto k1 = aint[j1 + 0 + (i1 + 0) * k];
            auto l1 = aint[j1 + 2 + (i1 + 0) * k];
            auto i2 = aint[j1 + 0 + (i1 + 2) * k];
            auto j2 = aint[j1 + 2 + (i1 + 2) * k];
            auto k2 = aint[j1 + 1 + (i1 + 1) * k];
            initChunkSeed((int64_t)(j1 + areaX), (int64_t)(i1 + areaY));
            if (k2 != 0 || k1 == 0 && l1 == 0 && i2 == 0 && j2 == 0)
            {
                if (k2 > 0 && (k1 == 0 || l1 == 0 || i2 == 0 || j2 == 0))
                {
                    if (nextInt(5) == 0)
                    {
                        if (k2 == 4)
                        {
                            aint1[j1 + i1 * areaWidth] = 4;
                        }
                        else
                        {
                            aint1[j1 + i1 * areaWidth] = 0;
                        }
                    }
                    else
                    {
                        aint1[j1 + i1 * areaWidth] = k2;
                    }
                }
                else
                {
                    aint1[j1 + i1 * areaWidth] = k2;
                }
            }
            else
            {
                auto l2 = 1;
                auto i3 = 1;
                if (k1 != 0 && nextInt(l2++) == 0)
                {
                    i3 = k1;
                }

                if (l1 != 0 && nextInt(l2++) == 0)
                {
                    i3 = l1;
                }

                if (i2 != 0 && nextInt(l2++) == 0)
                {
                    i3 = i2;
                }

                if (j2 != 0 && nextInt(l2++) == 0)
                {
                    i3 = j2;
                }

                if (nextInt(3) == 0)
                {
                    aint1[j1 + i1 * areaWidth] = i3;
                }
                else if (i3 == 4)
                {
                    aint1[j1 + i1 * areaWidth] = 4;
                }
                else
                {
                    aint1[j1 + i1 * areaWidth] = 0;
                }
            }
        }
    }

    return aint1;
}
