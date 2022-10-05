#include "BiomeDecorator.h"

class BiomeEndDecorator : public BiomeDecorator
{
  public:
    static WorldGenSpikes.EndSpike[] getSpikesForWorld(World *p_185426_0_);

  protected:
    void genDecorations(Biome *biomeIn, World *worldIn, pcg32 &random) override;

  private:
    static LoadingCache SPIKE_CACHE;
    WorldGenSpikes spikeGen = new WorldGenSpikes();
};
