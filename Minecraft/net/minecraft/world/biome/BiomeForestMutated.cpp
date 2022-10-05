#include "BiomeForestMutated.h"

#include "../../util/Util.h"

BiomeForestMutated::BiomeForestMutated(BiomeProperties properties) : BiomeForest(Type::BIRCH, properties)
{
}

WorldGenAbstractTree BiomeForestMutated::getRandomTreeFeature(pcg32 &rand)
{
    return Util::RandomizerWithSentinelShift(rand) ? BiomeForest::SUPER_BIRCH_TREE : BiomeForest::BIRCH_TREE;
}
