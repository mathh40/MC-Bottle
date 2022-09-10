#include "BlockPattern.h"

BlockPattern::BlockPattern(multivec3<std::function<bool()>> predicatesIn)
{
    blockMatches = predicatesIn;
      fingerLength = predicatesIn.sizeRow();
      if (fingerLength > 0) {
         thumbLength = predicatesIn.sizeColom();
         if (thumbLength > 0) {
            palmLength = predicatesIn.sizeDim();
         } else {
            palmLength = 0;
         }
      } else {
         thumbLength = 0;
         palmLength = 0;
      }
}

uint32_t BlockPattern::getFingerLength() const {
    return fingerLength;
}

uint32_t BlockPattern::getThumbLength() const {
    return thumbLength;
}

uint32_t BlockPattern::getPalmLength() const {
    return palmLength;
}


std::optional<BlockPattern::PatternHelper> BlockPattern::checkPatternAt(const BlockPos &pos,
  EnumFacing finger,
  EnumFacing thumb,
  lru_cache_t<BlockPos, BlockWorldState>& lcache)
{
          for(auto i = 0; i < palmLength; ++i) {
         for(auto j = 0; j < thumbLength; ++j) {
            for(auto k = 0; k < fingerLength; ++k) {
             if (!blockMatches(k,j,i)(lcache.Get(translateOffset(pos, finger, thumb, i, j, k)))) {
                  return std::nullopt;
               }
            }
         }
      }

      return BlockPattern::PatternHelper(pos, finger, thumb, lcache, palmLength, thumbLength, fingerLength);
}

std::optional<BlockPattern::PatternHelper> BlockPattern::match(World* worldIn, const BlockPos& pos) {
  auto loadingcache = createLoadingCache();
      auto i = MathHelper::max(MathHelper::max(palmLength, thumbLength), fingerLength);
  auto var5 = getAllInBoxMutable(pos, pos.add(static_cast<int32_t>(i - 1), i - 1, i - 1));

  for (auto &blockpos : var5){
         auto var7 = EnumFacing::values();
         for (auto &enumfacing : var7) {
           auto var11 = EnumFacing::values();

            for (auto &enumfacing1 : var11) {
               if (enumfacing1 != enumfacing && enumfacing1 != enumfacing.getOpposite()) {
                  auto blockpattern$patternhelper = checkPatternAt(blockpos, enumfacing, enumfacing1, loadingcache);
                  if (blockpattern$patternhelper.has_value()) {
                     return blockpattern$patternhelper;
                  }
               }
            }
         }
      }

      return std::nullopt;
}

lru_cache_t<BlockPos, BlockWorldState> BlockPattern::createLoadingCache()
{
  constexpr std::size_t CACHE_SIZE = 256;
    return lru_cache_t<BlockPos, BlockWorldState> (CACHE_SIZE);
}

BlockPos BlockPattern::translateOffset(const BlockPos &pos,
  EnumFacing finger,
  EnumFacing thumb,
  uint32_t palmOffset,
  uint32_t thumbOffset,
  uint32_t fingerOffset)
{
    if (finger != thumb && finger != thumb.getOpposite()) {
         Vec3i vec3i = Vec3i(finger.getXOffset(), finger.getYOffset(), finger.getZOffset());
         Vec3i vec3i1 = Vec3i(thumb.getXOffset(), thumb.getYOffset(), thumb.getZOffset());
         Vec3i vec3i2 = vec3i.crossProduct(vec3i1);
         return pos.add(vec3i1.getx() * -thumbOffset + vec3i2.getx() * palmOffset + vec3i.getx() * fingerOffset, vec3i1.gety() * -thumbOffset + vec3i2.gety() * palmOffset + vec3i.gety() * fingerOffset, vec3i1.getz() * -thumbOffset + vec3i2.getz() * palmOffset + vec3i.getz() * fingerOffset);
      } else {
         throw std::logic_error("Invalid forwards & up combination");
      }
}


BlockPattern::PatternHelper::PatternHelper(const BlockPos &posIn,
  EnumFacing fingerIn,
  EnumFacing thumbIn,
  lru_cache_t<BlockPos, BlockWorldState>& lcacheIn,
  uint32_t widthIn,
  uint32_t heightIn,
  uint32_t depthIn)
  : frontTopLeft(posIn), forwards(fingerIn), up(thumbIn), lcache(lcacheIn), width(widthIn), height(heightIn), depth(depthIn)
{

}

BlockPos& BlockPattern::PatternHelper::getFrontTopLeft() const {
    return frontTopLeft;
}

EnumFacing BlockPattern::PatternHelper::getForwards() const {
    return forwards;
}

EnumFacing BlockPattern::PatternHelper::getUp() const {
    return up;
}

uint32_t BlockPattern::PatternHelper::getWidth() const {
    width;
}

uint32_t BlockPattern::PatternHelper::getHeight() const {
    return height;
}

BlockWorldState
  BlockPattern::PatternHelper::translateOffset(uint32_t palmOffset, uint32_t thumbOffset, uint32_t fingerOffset) const
{
    return lcache.Get(BlockPattern::translateOffset(frontTopLeft, getForwards(), getUp(), palmOffset, thumbOffset, fingerOffset));
}

std::string BlockPattern::PatternHelper::toString() const {
  return fmt::format("up:{} forwards:{} frontTopLeft:{}",up,forwards,frontTopLeft);
}
