#include "IntCache.h"

int32_t IntCache::intCacheSize = 256;

std::vector<int32_t> IntCache::getIntCache(int32_t size)
{
    if (size <= 256)
    {
        std::array<int32_t, 256> aint;
        if (freeSmallArrays.empty())
        {
            inUseSmallArrays.emplace_back(aint);
            return std::vector<int32_t>(aint.begin(), aint.end());
        }
        else
        {
            aint = *freeSmallArrays.erase(freeSmallArrays.end() - 1);
            inUseSmallArrays.emplace_back(aint);
            return std::vector<int32_t>(aint.begin(), aint.end());
        }
    }
    else if (size > intCacheSize)
    {
        intCacheSize = size;
        freeLargeArrays.clear();
        inUseLargeArrays.clear();
        std::vector<int32_t> aint(intCacheSize);
        inUseLargeArrays.emplace_back(aint);
        return aint;
    }
    else if (freeLargeArrays.empty())
    {
        std::vector<int32_t> aint(intCacheSize);
        inUseLargeArrays.emplace_back(aint);
        return aint;
    }
    else
    {
        auto aint = *freeLargeArrays.erase(freeLargeArrays.end() - 1);
        inUseLargeArrays.emplace_back(aint);
        return aint;
    }
}

void IntCache::resetIntCache()
{
    if (!freeLargeArrays.empty())
    {
        freeLargeArrays.erase(freeLargeArrays.end() - 1);
    }

    if (!freeSmallArrays.empty())
    {
        freeSmallArrays.erase(freeSmallArrays.end() - 1);
    }

    freeLargeArrays.assign(inUseLargeArrays.begin(), inUseLargeArrays.end());
    freeSmallArrays.assign(inUseSmallArrays.begin(), inUseSmallArrays.end());
    inUseLargeArrays.clear();
    inUseSmallArrays.clear();
}
