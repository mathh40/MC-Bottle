#pragma once
#include "WorldGenerator.h"
#include "math/AxisAlignedBB.h"
#include "math/BlockPos.h"

class WorldGenSpikes : public WorldGenerator
{
  public:
    class EndSpike
    {
      public:
        EndSpike(int32_t centerXIn, int32_t centerZIn, int32_t radiusIn, int32_t heightIn, bool guardedIn);
        bool doesStartInChunk(const BlockPos &p_186154_1_) const;
        int32_t getCenterX() const;
        int32_t getCenterZ() const;
        int32_t getRadius() const;
        int32_t getHeight() const;
        bool isGuarded() const;
        const AxisAlignedBB getTopBoundingBox() const;

      private:
        int32_t centerX;
        int32_t centerZ;
        int32_t radius;
        int32_t height;
        bool guarded;
        AxisAlignedBB topBoundingBox;
    };

    void setSpike(const EndSpike &p_186143_1_);
    void setCrystalInvulnerable(bool p_186144_1_);
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;
    void setBeamTarget(std::optional<BlockPos> pos);

  private:
    bool crystalInvulnerable;
    EndSpike spike;
    BlockPos beamTarget;
};
