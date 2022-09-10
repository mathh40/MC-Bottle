#pragma once
#include "../BlockWorldState.h"
#include "../../../util/multivec.h"
#include "../util/cache/cache.hpp"
#include "../util/cache/lru_cache_policy.hpp"

template <typename Key, typename Value>
using lru_cache_t = typename caches::fixed_sized_cache<Key, Value, caches::LRUCachePolicy>;


class EnumFacing;
class BlockPos;

class BlockPattern
{
public:
    class PatternHelper
  {
    public:
    PatternHelper(const BlockPos &posIn,
      EnumFacing fingerIn,
      EnumFacing thumbIn,
        lru_cache_t<BlockPos, BlockWorldState>& lcacheIn,
      uint32_t widthIn,
        uint32_t heightIn,
        uint32_t depthIn);
        BlockPos& getFrontTopLeft() const;
        EnumFacing getForwards() const;
        EnumFacing getUp() const;
        uint32_t getWidth() const;
        uint32_t getHeight() const;
        BlockWorldState translateOffset(uint32_t palmOffset, uint32_t thumbOffset, uint32_t fingerOffset) const;
        std::string toString() const;



  private:
      BlockPos frontTopLeft;
      EnumFacing forwards;
      EnumFacing up;
      lru_cache_t<BlockPos, BlockWorldState> lcache;
      uint32_t width;
      uint32_t height;
      uint32_t depth;
  };

    BlockPattern(multivec3<std::function<bool()>> predicatesIn);
    std::optional<BlockPattern::PatternHelper> match(World* worldIn, const BlockPos& pos);
    static lru_cache_t<BlockPos, BlockWorldState> createLoadingCache();

    uint32_t getFingerLength() const;
    uint32_t getThumbLength() const;
    uint32_t getPalmLength() const;

  protected:
    static BlockPos translateOffset(const BlockPos &pos,
      EnumFacing finger,
      EnumFacing thumb,
      uint32_t palmOffset,
      uint32_t thumbOffset,
      uint32_t fingerOffset);

private:
      std::optional<BlockPattern::PatternHelper> checkPatternAt(const BlockPos &pos,
    EnumFacing finger,
    EnumFacing thumb,
    lru_cache_t<BlockPos, BlockWorldState>& lcache);

   multivec3<std::function<bool()>>blockMatches;
      uint32_t fingerLength;
   uint32_t thumbLength;
      uint32_t palmLength;
};
