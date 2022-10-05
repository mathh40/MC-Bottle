#include "GenLayerEdge.h"

#include "IntCache.h"

std::vector<int32_t> GenLayerEdge::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
    switch (mode)
    {
    case Mode::COOL_WARM:
    default:
        return getIntsCoolWarm(areaX, areaY, areaWidth, areaHeight);
    case Mode::HEAT_ICE:
        return getIntsHeatIce(areaX, areaY, areaWidth, areaHeight);
    case Mode::SPECIAL:
        return getIntsSpecial(areaX, areaY, areaWidth, areaHeight);
    }
}

GenLayerEdge::GenLayerEdge(int64_t p_i45474_1_, std::shared_ptr<GenLayer> p_i45474_3_, GenLayerEdge::Mode p_i45474_4_)
    : GenLayer(p_i45474_1_)
{
    parent = p_i45474_3_;
    mode   = p_i45474_4_;
}

std::vector<int32_t> GenLayerEdge::getIntsCoolWarm(int32_t p_151626_1_, int32_t p_151626_2_, int32_t p_151626_3_,
                                                   int32_t p_151626_4_)
{
    auto i     = p_151626_1_ - 1;
    auto j     = p_151626_2_ - 1;
    auto k     = 1 + p_151626_3_ + 1;
    auto l     = 1 + p_151626_4_ + 1;
    auto aint  = parent->getInts(i, j, k, l);
    auto aint1 = IntCache::getIntCache(p_151626_3_ * p_151626_4_);

    for (auto i1 = 0; i1 < p_151626_4_; ++i1)
    {
        for (auto j1 = 0; j1 < p_151626_3_; ++j1)
        {
            initChunkSeed((int64_t)(j1 + p_151626_1_), (int64_t)(i1 + p_151626_2_));
            auto k1 = aint[j1 + 1 + (i1 + 1) * k];
            if (k1 == 1)
            {
                auto l1    = aint[j1 + 1 + (i1 + 1 - 1) * k];
                auto i2    = aint[j1 + 1 + 1 + (i1 + 1) * k];
                auto j2    = aint[j1 + 1 - 1 + (i1 + 1) * k];
                auto k2    = aint[j1 + 1 + (i1 + 1 + 1) * k];
                auto flag  = l1 == 3 || i2 == 3 || j2 == 3 || k2 == 3;
                auto flag1 = l1 == 4 || i2 == 4 || j2 == 4 || k2 == 4;
                if (flag || flag1)
                {
                    k1 = 2;
                }
            }

            aint1[j1 + i1 * p_151626_3_] = k1;
        }
    }

    return aint1;
}

std::vector<int32_t> GenLayerEdge::getIntsHeatIce(int32_t p_151624_1_, int32_t p_151624_2_, int32_t p_151624_3_,
                                                  int32_t p_151624_4_)
{
    auto i     = p_151624_1_ - 1;
    auto j     = p_151624_2_ - 1;
    auto k     = 1 + p_151624_3_ + 1;
    auto l     = 1 + p_151624_4_ + 1;
    auto aint  = parent->getInts(i, j, k, l);
    auto aint1 = IntCache::getIntCache(p_151624_3_ * p_151624_4_);

    for (auto i1 = 0; i1 < p_151624_4_; ++i1)
    {
        for (auto j1 = 0; j1 < p_151624_3_; ++j1)
        {
            auto k1 = aint[j1 + 1 + (i1 + 1) * k];
            if (k1 == 4)
            {
                auto l1    = aint[j1 + 1 + (i1 + 1 - 1) * k];
                auto i2    = aint[j1 + 1 + 1 + (i1 + 1) * k];
                auto j2    = aint[j1 + 1 - 1 + (i1 + 1) * k];
                auto k2    = aint[j1 + 1 + (i1 + 1 + 1) * k];
                auto flag  = l1 == 2 || i2 == 2 || j2 == 2 || k2 == 2;
                auto flag1 = l1 == 1 || i2 == 1 || j2 == 1 || k2 == 1;
                if (flag1 || flag)
                {
                    k1 = 3;
                }
            }

            aint1[j1 + i1 * p_151624_3_] = k1;
        }
    }

    return aint1;
}

std::vector<int32_t> GenLayerEdge::getIntsSpecial(int32_t p_151625_1_, int32_t p_151625_2_, int32_t p_151625_3_,
                                                  int32_t p_151625_4_)
{
    auto aint  = parent->getInts(p_151625_1_, p_151625_2_, p_151625_3_, p_151625_4_);
    auto aint1 = IntCache::getIntCache(p_151625_3_ * p_151625_4_);

    for (auto i = 0; i < p_151625_4_; ++i)
    {
        for (auto j = 0; j < p_151625_3_; ++j)
        {
            initChunkSeed((int64_t)(j + p_151625_1_), (int64_t)(i + p_151625_2_));
            auto k = aint[j + i * p_151625_3_];
            if (k != 0 && nextInt(13) == 0)
            {
                k |= 1 + nextInt(15) << 8 & 3840;
            }

            aint1[j + i * p_151625_3_] = k;
        }
    }

    return aint1;
}
