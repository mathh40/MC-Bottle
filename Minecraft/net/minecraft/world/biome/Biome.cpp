#include "Biome.h"

#include "../../util/ResourceLocation.h"
#include "../../util/math/MathHelper.h"
#include "spdlog/spdlog.h"

#include <random>

std::shared_ptr<spdlog::logger> Biome::LOGGER = spdlog::get("Minecraft")->clone("Biome");
STONE                                         = Blocks.STONE.getDefaultState();
AIR                                           = Blocks.AIR.getDefaultState();
BEDROCK                                       = Blocks.BEDROCK.getDefaultState();
GRAVEL                                        = Blocks.GRAVEL.getDefaultState();
RED_SANDSTONE                                 = Blocks.RED_SANDSTONE.getDefaultState();
SANDSTONE                                     = Blocks.SANDSTONE.getDefaultState();
ICE                                           = Blocks.ICE.getDefaultState();
WATER                                         = Blocks.WATER.getDefaultState();
MUTATION_TO_BASE_ID_MAP                       = new ObjectIntIdentityMap();
TEMPERATURE_NOISE                             = new NoiseGeneratorPerlin(new Random(1234L), 1);
GRASS_COLOR_NOISE                             = new NoiseGeneratorPerlin(new Random(2345L), 1);
DOUBLE_PLANT_GENERATOR                        = new WorldGenDoublePlant();
TREE_FEATURE                                  = new WorldGenTrees(false);
BIG_TREE_FEATURE                              = new WorldGenBigTree(false);
SWAMP_FEATURE                                 = new WorldGenSwamp();
REGISTRY                                      = new RegistryNamespaced();

BiomeProperties::BiomeProperties(std::string_view nameIn) : biomeName(nameIn)
{
}

BiomeProperties &BiomeProperties::setTemperature(float temperatureIn)
{
    if (temperatureIn > 0.1F && temperatureIn < 0.2F)
    {
        throw std::logic_error("Please avoid temperatures in the range 0.1 - 0.2 because of snow");
    }
    else
    {
        temperature = temperatureIn;
        return *this;
    }
}

BiomeProperties &BiomeProperties::setRainfall(float rainfallIn)
{
    rainfall = rainfallIn;
    return *this;
}

BiomeProperties &BiomeProperties::setBaseHeight(float baseHeightIn)
{
    baseHeight = baseHeightIn;
    return *this;
}

BiomeProperties &BiomeProperties::setHeightVariation(float heightVariationIn)
{
    heightVariation = heightVariationIn;
    return *this;
}

BiomeProperties &BiomeProperties::setRainDisabled()
{
    enableRain = false;
    return *this;
}

BiomeProperties &BiomeProperties::setSnowEnabled()
{
    enableSnow = true;
    return *this;
}

BiomeProperties &BiomeProperties::setWaterColor(int64_t waterColorIn)
{
    waterColor = waterColorIn;
    return *this;
}

BiomeProperties &BiomeProperties::setBaseBiome(std::string_view nameIn)
{
    baseBiomeRegName = nameIn;
    return *this;
}

int32_t Biome::getIdForBiome(Biome *biome)
{
    return REGISTRY.getIDForObject(biome);
}

Biome *Biome::getBiomeForId(int32_t id)
{
    auto obj = REGISTRY.getObjectById(id);

    return obj ? obj.value() : nullptr;
}

Biome *Biome::getMutationForBiome(Biome *biome)
{
    return MUTATION_TO_BASE_ID_MAP.find(getIdForBiome(biome))->second;
}

bool Biome::isMutation() const
{
    return baseBiomeRegName.has_value();
}

Biome::Biome(BiomeProperties properties)
{
    topBlock                   = Blocks::GRASS.getDefaultState();
    fillerBlock                = Blocks.DIRT.getDefaultState();
    spawnableMonsterList       = Lists.newArrayList();
    spawnableCreatureList      = Lists.newArrayList();
    spawnableWaterCreatureList = Lists.newArrayList();
    spawnableCaveCreatureList  = Lists.newArrayList();
    biomeName                  = properties.biomeName;
    baseHeight                 = properties.baseHeight;
    heightVariation            = properties.heightVariation;
    temperature                = properties.temperature;
    rainfall                   = properties.rainfall;
    waterColor                 = properties.waterColor;
    enableSnow                 = properties.enableSnow;
    enableRain                 = properties.enableRain;
    baseBiomeRegName           = properties.baseBiomeRegName;
    decorator                  = createBiomeDecorator();
    spawnableCreatureList.add(SpawnListEntry(EntitySheep.class, 12, 4, 4));
    spawnableCreatureList.add(SpawnListEntry(EntityPig.class, 10, 4, 4));
    spawnableCreatureList.add(SpawnListEntry(EntityChicken.class, 10, 4, 4));
    spawnableCreatureList.add(SpawnListEntry(EntityCow.class, 8, 4, 4));
    spawnableMonsterList.add(SpawnListEntry(EntitySpider.class, 100, 4, 4));
    spawnableMonsterList.add(SpawnListEntry(EntityZombie.class, 95, 4, 4));
    spawnableMonsterList.add(SpawnListEntry(EntityZombieVillager.class, 5, 1, 1));
    spawnableMonsterList.add(SpawnListEntry(EntitySkeleton.class, 100, 4, 4));
    spawnableMonsterList.add(SpawnListEntry(EntityCreeper.class, 100, 4, 4));
    spawnableMonsterList.add(SpawnListEntry(EntitySlime.class, 100, 4, 4));
    spawnableMonsterList.add(SpawnListEntry(EntityEnderman.class, 10, 1, 4));
    spawnableMonsterList.add(SpawnListEntry(EntityWitch.class, 5, 1, 1));
    spawnableWaterCreatureList.add(SpawnListEntry(EntitySquid.class, 10, 4, 4));
    spawnableCaveCreatureList.add(SpawnListEntry(EntityBat.class, 10, 8, 8));
}

BiomeDecorator Biome::createBiomeDecorator()
{
    return BiomeDecorator();
}

void Biome::registerBiome(int id, std::string_view name, Biome *biome)
{
    REGISTRY.registe(id, ResourceLocation(name), biome);
    if (biome->isMutation())
    {
        ResourceLocation rloc(biome->baseBiomeRegName.value());
        MUTATION_TO_BASE_ID_MAP.emplace(biome, getIdForBiome(REGISTRY.getObject(rloc).value()));
    }
}

WorldGenAbstractTree Biome::getRandomTreeFeature(Random &rand)
{
    return (rand(10) == 0 ? BIG_TREE_FEATURE : TREE_FEATURE);
}

WorldGenerator Biome::getRandomWorldGenForGrass(Random &rand)
{
    return WorldGenTallGrass(BlockTallGrass::EnumType.GRASS);
}

BlockFlower.EnumFlowerType Biome::pickRandomFlower(Random &rand, BlockPos &pos)
{
    return rand(3) > 0 ? BlockFlower.EnumFlowerType.DANDELION : BlockFlower.EnumFlowerType.POPPY;
}

int32_t Biome::getSkyColorByTemp(float currentTemperature)
{
    currentTemperature /= 3.0F;
    currentTemperature = MathHelper::clamp(currentTemperature, -1.0F, 1.0F);
    return MathHelper::hsvToRGB(0.62222224F - currentTemperature * 0.05F, 0.5F + currentTemperature * 0.1F, 1.0F);
}

std::vector<SpawnListEntry> Biome::getSpawnableList(EnumCreatureType creatureType)
{
    switch (creatureType)
    {
    case MONSTER:
        return spawnableMonsterList;
    case CREATURE:
        return spawnableCreatureList;
    case WATER_CREATURE:
        return spawnableWaterCreatureList;
    case AMBIENT:
        return spawnableCaveCreatureList;
    default:
        return Collections.emptyList();
    }
}

bool Biome::getEnableSnow()
{
    return isSnowyBiome();
}

bool Biome::canRain()
{
    return isSnowyBiome() ? false : enableRain;
}

bool Biome::isHighHumidity()
{
    return getRainfall() > 0.85F;
}

float Biome::getSpawningChance()
{
    return 0.1F;
}

float Biome::getTemperature(BlockPos &pos)
{
    if (pos.gety() > 64)
    {
        float f =
            (float)(TEMPERATURE_NOISE.getValue((double)((float)pos.getx() / 8.0F), (double)((float)pos.getz() / 8.0F)) *
                    4.0);
        return getDefaultTemperature() - (f + (float)pos.gety() - 64.0F) * 0.05F / 30.0F;
    }
    else
    {
        return getDefaultTemperature();
    }
}

void Biome::decorate(World *worldIn, Random &rand, BlockPos &pos)
{
    decorator.decorate(worldIn, rand, this, pos);
}

int32_t Biome::getGrassColorAtPos(BlockPos &pos)
{
    double d0 = (double)MathHelper::clamp(getTemperature(pos), 0.0F, 1.0F);
    double d1 = (double)MathHelper::clamp(getRainfall(), 0.0F, 1.0F);
    return ColorizerGrass.getGrassColor(d0, d1);
}

int32_t Biome::getFoliageColorAtPos(BlockPos &pos)
{
    double d0 = (double)MathHelper::clamp(getTemperature(pos), 0.0F, 1.0F);
    double d1 = (double)MathHelper::clamp(getRainfall(), 0.0F, 1.0F);
    return ColorizerFoliage.getFoliageColor(d0, d1);
}

void Biome::genTerrainBlocks(World *worldIn, Random &rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z,
                             double noiseVal)
{
    generateBiomeTerrain(worldIn, rand, chunkPrimerIn, x, z, noiseVal);
}

void Biome::generateBiomeTerrain(World *worldIn, Random &rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z,
                                 double noiseVal)
{
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    auto i                    = worldIn->getSeaLevel();
    IBlockState *iblockstate  = topBlock;
    IBlockState *iblockstate1 = fillerBlock;
    auto j                    = -1;
    auto k                    = (noiseVal / 3.0 + 3.0 + dis(rand) * 0.25);
    auto l                    = x & 15;
    auto i1                   = z & 15;
    MutableBlockPos mutableblockpos;

    for (auto j1 = 255; j1 >= 0; --j1)
    {
        if (j1 <= rand(5))
        {
            chunkPrimerIn.setBlockState(i1, j1, l, BEDROCK);
        }
        else
        {
            IBlockState *iblockstate2 = chunkPrimerIn.getBlockState(i1, j1, l);
            if (iblockstate2.getMaterial() == Material::AIR)
            {
                j = -1;
            }
            else if (iblockstate2.getBlock() == Blocks.STONE)
            {
                if (j == -1)
                {
                    if (k <= 0)
                    {
                        iblockstate  = AIR;
                        iblockstate1 = STONE;
                    }
                    else if (j1 >= i - 4 && j1 <= i + 1)
                    {
                        iblockstate  = topBlock;
                        iblockstate1 = fillerBlock;
                    }

                    if (j1 < i && (iblockstate == nullptr || iblockstate.getMaterial() == Material::AIR))
                    {
                        auto pos = mutableblockpos.setPos(x, j1, z);
                        if (getTemperature(pos) < 0.15F)
                        {
                            iblockstate = ICE;
                        }
                        else
                        {
                            iblockstate = WATER;
                        }
                    }

                    j = k;
                    if (j1 >= i - 1)
                    {
                        chunkPrimerIn.setBlockState(i1, j1, l, iblockstate);
                    }
                    else if (j1 < i - 7 - k)
                    {
                        iblockstate  = AIR;
                        iblockstate1 = STONE;
                        chunkPrimerIn.setBlockState(i1, j1, l, GRAVEL);
                    }
                    else
                    {
                        chunkPrimerIn.setBlockState(i1, j1, l, iblockstate1);
                    }
                }
                else if (j > 0)
                {
                    --j;
                    chunkPrimerIn.setBlockState(i1, j1, l, iblockstate1);
                    if (j == 0 && iblockstate1.getBlock() == Blocks.SAND && k > 1)
                    {
                        j            = rand(4) + MathHelper::max(0, j1 - 63);
                        iblockstate1 = iblockstate1.getValue(BlockSand.VARIANT) == BlockSand.EnumType.RED_SAND
                                           ? RED_SANDSTONE
                                           : SANDSTONE;
                    }
                }
            }
        }
    }
}

TempCategory Biome::getTempCategory()
{
    if (getDefaultTemperature() < 0.2)
    {
        return TempCategory::COLD;
    }
    else
    {
        return getDefaultTemperature() < 1.0 ? TempCategory::MEDIUM : TempCategory::WARM;
    }
}

Biome *Biome::getBiome(int32_t id)
{
    return getBiome(id, nullptr);
}

Biome *Biome::getBiome(int32_t biomeId, Biome *fallback)
{
    auto biome = getBiomeForId(biomeId);
    return biome == nullptr ? fallback : biome;
}

bool Biome::ignorePlayerSpawnSuitability() const
{
    return false;
}

float Biome::getBaseHeight() const
{
    return baseHeight;
}

float Biome::getRainfall() const
{
    return rainfall;
}

std::string Biome::getBiomeName() const
{
    return biomeName;
}

float Biome::getHeightVariation() const
{
    return heightVariation;
}

float Biome::getDefaultTemperature() const
{
    return temperature;
}

int32_t Biome::getWaterColor() const
{
    return waterColor;
}

bool Biome::isSnowyBiome() const
{
    return enableSnow;
}

void Biome::registerBiomes()
{
    registerBiome(0, "ocean",
                  new BiomeOcean((new Biome.BiomeProperties("Ocean")).setBaseHeight(-1.0F).setHeightVariation(0.1F)));
    registerBiome(1, "plains",
                  new BiomePlains(false, (new Biome.BiomeProperties("Plains"))
                                             .setBaseHeight(0.125F)
                                             .setHeightVariation(0.05F)
                                             .setTemperature(0.8F)
                                             .setRainfall(0.4F)));
    registerBiome(2, "desert",
                  new BiomeDesert((new Biome.BiomeProperties("Desert"))
                                      .setBaseHeight(0.125F)
                                      .setHeightVariation(0.05F)
                                      .setTemperature(2.0F)
                                      .setRainfall(0.0F)
                                      .setRainDisabled()));
    registerBiome(3, "extreme_hills",
                  new BiomeHills(BiomeHills.Type.NORMAL, (new Biome.BiomeProperties("Extreme Hills"))
                                                             .setBaseHeight(1.0F)
                                                             .setHeightVariation(0.5F)
                                                             .setTemperature(0.2F)
                                                             .setRainfall(0.3F)));
    registerBiome(4, "forest",
                  new BiomeForest(BiomeForest.Type.NORMAL,
                                  (new Biome.BiomeProperties("Forest")).setTemperature(0.7F).setRainfall(0.8F)));
    registerBiome(5, "taiga",
                  new BiomeTaiga(BiomeTaiga.Type.NORMAL, (new Biome.BiomeProperties("Taiga"))
                                                             .setBaseHeight(0.2F)
                                                             .setHeightVariation(0.2F)
                                                             .setTemperature(0.25F)
                                                             .setRainfall(0.8F)));
    registerBiome(6, "swampland",
                  new BiomeSwamp((new Biome.BiomeProperties("Swampland"))
                                     .setBaseHeight(-0.2F)
                                     .setHeightVariation(0.1F)
                                     .setTemperature(0.8F)
                                     .setRainfall(0.9F)
                                     .setWaterColor(14745518)));
    registerBiome(7, "river",
                  new BiomeRiver((new Biome.BiomeProperties("River")).setBaseHeight(-0.5F).setHeightVariation(0.0F)));
    registerBiome(
        8, "hell",
        new BiomeHell((new Biome.BiomeProperties("Hell")).setTemperature(2.0F).setRainfall(0.0F).setRainDisabled()));
    registerBiome(9, "sky", new BiomeEnd((new Biome.BiomeProperties("The End")).setRainDisabled()));
    registerBiome(10, "frozen_ocean",
                  new BiomeOcean((new Biome.BiomeProperties("FrozenOcean"))
                                     .setBaseHeight(-1.0F)
                                     .setHeightVariation(0.1F)
                                     .setTemperature(0.0F)
                                     .setRainfall(0.5F)
                                     .setSnowEnabled()));
    registerBiome(11, "frozen_river",
                  new BiomeRiver((new Biome.BiomeProperties("FrozenRiver"))
                                     .setBaseHeight(-0.5F)
                                     .setHeightVariation(0.0F)
                                     .setTemperature(0.0F)
                                     .setRainfall(0.5F)
                                     .setSnowEnabled()));
    registerBiome(12, "ice_flats",
                  new BiomeSnow(false, (new Biome.BiomeProperties("Ice Plains"))
                                           .setBaseHeight(0.125F)
                                           .setHeightVariation(0.05F)
                                           .setTemperature(0.0F)
                                           .setRainfall(0.5F)
                                           .setSnowEnabled()));
    registerBiome(13, "ice_mountains",
                  new BiomeSnow(false, (new Biome.BiomeProperties("Ice Mountains"))
                                           .setBaseHeight(0.45F)
                                           .setHeightVariation(0.3F)
                                           .setTemperature(0.0F)
                                           .setRainfall(0.5F)
                                           .setSnowEnabled()));
    registerBiome(14, "mushroom_island",
                  new BiomeMushroomIsland((new Biome.BiomeProperties("MushroomIsland"))
                                              .setBaseHeight(0.2F)
                                              .setHeightVariation(0.3F)
                                              .setTemperature(0.9F)
                                              .setRainfall(1.0F)));
    registerBiome(15, "mushroom_island_shore",
                  new BiomeMushroomIsland((new Biome.BiomeProperties("MushroomIslandShore"))
                                              .setBaseHeight(0.0F)
                                              .setHeightVariation(0.025F)
                                              .setTemperature(0.9F)
                                              .setRainfall(1.0F)));
    registerBiome(16, "beaches",
                  new BiomeBeach((new Biome.BiomeProperties("Beach"))
                                     .setBaseHeight(0.0F)
                                     .setHeightVariation(0.025F)
                                     .setTemperature(0.8F)
                                     .setRainfall(0.4F)));
    registerBiome(17, "desert_hills",
                  new BiomeDesert((new Biome.BiomeProperties("DesertHills"))
                                      .setBaseHeight(0.45F)
                                      .setHeightVariation(0.3F)
                                      .setTemperature(2.0F)
                                      .setRainfall(0.0F)
                                      .setRainDisabled()));
    registerBiome(18, "forest_hills",
                  new BiomeForest(BiomeForest.Type.NORMAL, (new Biome.BiomeProperties("ForestHills"))
                                                               .setBaseHeight(0.45F)
                                                               .setHeightVariation(0.3F)
                                                               .setTemperature(0.7F)
                                                               .setRainfall(0.8F)));
    registerBiome(19, "taiga_hills",
                  new BiomeTaiga(BiomeTaiga.Type.NORMAL, (new Biome.BiomeProperties("TaigaHills"))
                                                             .setTemperature(0.25F)
                                                             .setRainfall(0.8F)
                                                             .setBaseHeight(0.45F)
                                                             .setHeightVariation(0.3F)));
    registerBiome(20, "smaller_extreme_hills",
                  new BiomeHills(BiomeHills.Type.EXTRA_TREES, (new Biome.BiomeProperties("Extreme Hills Edge"))
                                                                  .setBaseHeight(0.8F)
                                                                  .setHeightVariation(0.3F)
                                                                  .setTemperature(0.2F)
                                                                  .setRainfall(0.3F)));
    registerBiome(
        21, "jungle",
        new BiomeJungle(false, (new Biome.BiomeProperties("Jungle")).setTemperature(0.95F).setRainfall(0.9F)));
    registerBiome(22, "jungle_hills",
                  new BiomeJungle(false, (new Biome.BiomeProperties("JungleHills"))
                                             .setBaseHeight(0.45F)
                                             .setHeightVariation(0.3F)
                                             .setTemperature(0.95F)
                                             .setRainfall(0.9F)));
    registerBiome(
        23, "jungle_edge",
        new BiomeJungle(true, (new Biome.BiomeProperties("JungleEdge")).setTemperature(0.95F).setRainfall(0.8F)));
    registerBiome(
        24, "deep_ocean",
        new BiomeOcean((new Biome.BiomeProperties("Deep Ocean")).setBaseHeight(-1.8F).setHeightVariation(0.1F)));
    registerBiome(25, "stone_beach",
                  new BiomeStoneBeach((new Biome.BiomeProperties("Stone Beach"))
                                          .setBaseHeight(0.1F)
                                          .setHeightVariation(0.8F)
                                          .setTemperature(0.2F)
                                          .setRainfall(0.3F)));
    registerBiome(26, "cold_beach",
                  new BiomeBeach((new Biome.BiomeProperties("Cold Beach"))
                                     .setBaseHeight(0.0F)
                                     .setHeightVariation(0.025F)
                                     .setTemperature(0.05F)
                                     .setRainfall(0.3F)
                                     .setSnowEnabled()));
    registerBiome(27, "birch_forest",
                  new BiomeForest(BiomeForest.Type.BIRCH,
                                  (new Biome.BiomeProperties("Birch Forest")).setTemperature(0.6F).setRainfall(0.6F)));
    registerBiome(28, "birch_forest_hills",
                  new BiomeForest(BiomeForest.Type.BIRCH, (new Biome.BiomeProperties("Birch Forest Hills"))
                                                              .setBaseHeight(0.45F)
                                                              .setHeightVariation(0.3F)
                                                              .setTemperature(0.6F)
                                                              .setRainfall(0.6F)));
    registerBiome(29, "roofed_forest",
                  new BiomeForest(BiomeForest.Type.ROOFED,
                                  (new Biome.BiomeProperties("Roofed Forest")).setTemperature(0.7F).setRainfall(0.8F)));
    registerBiome(30, "taiga_cold",
                  new BiomeTaiga(BiomeTaiga.Type.NORMAL, (new Biome.BiomeProperties("Cold Taiga"))
                                                             .setBaseHeight(0.2F)
                                                             .setHeightVariation(0.2F)
                                                             .setTemperature(-0.5F)
                                                             .setRainfall(0.4F)
                                                             .setSnowEnabled()));
    registerBiome(31, "taiga_cold_hills",
                  new BiomeTaiga(BiomeTaiga.Type.NORMAL, (new Biome.BiomeProperties("Cold Taiga Hills"))
                                                             .setBaseHeight(0.45F)
                                                             .setHeightVariation(0.3F)
                                                             .setTemperature(-0.5F)
                                                             .setRainfall(0.4F)
                                                             .setSnowEnabled()));
    registerBiome(32, "redwood_taiga",
                  new BiomeTaiga(BiomeTaiga.Type.MEGA, (new Biome.BiomeProperties("Mega Taiga"))
                                                           .setTemperature(0.3F)
                                                           .setRainfall(0.8F)
                                                           .setBaseHeight(0.2F)
                                                           .setHeightVariation(0.2F)));
    registerBiome(33, "redwood_taiga_hills",
                  new BiomeTaiga(BiomeTaiga.Type.MEGA, (new Biome.BiomeProperties("Mega Taiga Hills"))
                                                           .setBaseHeight(0.45F)
                                                           .setHeightVariation(0.3F)
                                                           .setTemperature(0.3F)
                                                           .setRainfall(0.8F)));
    registerBiome(34, "extreme_hills_with_trees",
                  new BiomeHills(BiomeHills.Type.EXTRA_TREES, (new Biome.BiomeProperties("Extreme Hills+"))
                                                                  .setBaseHeight(1.0F)
                                                                  .setHeightVariation(0.5F)
                                                                  .setTemperature(0.2F)
                                                                  .setRainfall(0.3F)));
    registerBiome(35, "savanna",
                  new BiomeSavanna((new Biome.BiomeProperties("Savanna"))
                                       .setBaseHeight(0.125F)
                                       .setHeightVariation(0.05F)
                                       .setTemperature(1.2F)
                                       .setRainfall(0.0F)
                                       .setRainDisabled()));
    registerBiome(36, "savanna_rock",
                  new BiomeSavanna((new Biome.BiomeProperties("Savanna Plateau"))
                                       .setBaseHeight(1.5F)
                                       .setHeightVariation(0.025F)
                                       .setTemperature(1.0F)
                                       .setRainfall(0.0F)
                                       .setRainDisabled()));
    registerBiome(
        37, "mesa",
        new BiomeMesa(false, false,
                      (new Biome.BiomeProperties("Mesa")).setTemperature(2.0F).setRainfall(0.0F).setRainDisabled()));
    registerBiome(38, "mesa_rock",
                  new BiomeMesa(false, true,
                                (new Biome.BiomeProperties("Mesa Plateau F"))
                                    .setBaseHeight(1.5F)
                                    .setHeightVariation(0.025F)
                                    .setTemperature(2.0F)
                                    .setRainfall(0.0F)
                                    .setRainDisabled()));
    registerBiome(39, "mesa_clear_rock",
                  new BiomeMesa(false, false,
                                (new Biome.BiomeProperties("Mesa Plateau"))
                                    .setBaseHeight(1.5F)
                                    .setHeightVariation(0.025F)
                                    .setTemperature(2.0F)
                                    .setRainfall(0.0F)
                                    .setRainDisabled()));
    registerBiome(127, "void", new BiomeVoid((new Biome.BiomeProperties("The Void")).setRainDisabled()));
    registerBiome(129, "mutated_plains",
                  new BiomePlains(true, (new Biome.BiomeProperties("Sunflower Plains"))
                                            .setBaseBiome("plains")
                                            .setBaseHeight(0.125F)
                                            .setHeightVariation(0.05F)
                                            .setTemperature(0.8F)
                                            .setRainfall(0.4F)));
    registerBiome(130, "mutated_desert",
                  new BiomeDesert((new Biome.BiomeProperties("Desert M"))
                                      .setBaseBiome("desert")
                                      .setBaseHeight(0.225F)
                                      .setHeightVariation(0.25F)
                                      .setTemperature(2.0F)
                                      .setRainfall(0.0F)
                                      .setRainDisabled()));
    registerBiome(131, "mutated_extreme_hills",
                  new BiomeHills(BiomeHills.Type.MUTATED, (new Biome.BiomeProperties("Extreme Hills M"))
                                                              .setBaseBiome("extreme_hills")
                                                              .setBaseHeight(1.0F)
                                                              .setHeightVariation(0.5F)
                                                              .setTemperature(0.2F)
                                                              .setRainfall(0.3F)));
    registerBiome(132, "mutated_forest",
                  new BiomeForest(BiomeForest.Type.FLOWER, (new Biome.BiomeProperties("Flower Forest"))
                                                               .setBaseBiome("forest")
                                                               .setHeightVariation(0.4F)
                                                               .setTemperature(0.7F)
                                                               .setRainfall(0.8F)));
    registerBiome(133, "mutated_taiga",
                  new BiomeTaiga(BiomeTaiga.Type.NORMAL, (new Biome.BiomeProperties("Taiga M"))
                                                             .setBaseBiome("taiga")
                                                             .setBaseHeight(0.3F)
                                                             .setHeightVariation(0.4F)
                                                             .setTemperature(0.25F)
                                                             .setRainfall(0.8F)));
    registerBiome(134, "mutated_swampland",
                  new BiomeSwamp((new Biome.BiomeProperties("Swampland M"))
                                     .setBaseBiome("swampland")
                                     .setBaseHeight(-0.1F)
                                     .setHeightVariation(0.3F)
                                     .setTemperature(0.8F)
                                     .setRainfall(0.9F)
                                     .setWaterColor(14745518)));
    registerBiome(140, "mutated_ice_flats",
                  new BiomeSnow(true, (new Biome.BiomeProperties("Ice Plains Spikes"))
                                          .setBaseBiome("ice_flats")
                                          .setBaseHeight(0.425F)
                                          .setHeightVariation(0.45000002F)
                                          .setTemperature(0.0F)
                                          .setRainfall(0.5F)
                                          .setSnowEnabled()));
    registerBiome(149, "mutated_jungle",
                  new BiomeJungle(false, (new Biome.BiomeProperties("Jungle M"))
                                             .setBaseBiome("jungle")
                                             .setBaseHeight(0.2F)
                                             .setHeightVariation(0.4F)
                                             .setTemperature(0.95F)
                                             .setRainfall(0.9F)));
    registerBiome(151, "mutated_jungle_edge",
                  new BiomeJungle(true, (new Biome.BiomeProperties("JungleEdge M"))
                                            .setBaseBiome("jungle_edge")
                                            .setBaseHeight(0.2F)
                                            .setHeightVariation(0.4F)
                                            .setTemperature(0.95F)
                                            .setRainfall(0.8F)));
    registerBiome(155, "mutated_birch_forest",
                  new BiomeForestMutated((new Biome.BiomeProperties("Birch Forest M"))
                                             .setBaseBiome("birch_forest")
                                             .setBaseHeight(0.2F)
                                             .setHeightVariation(0.4F)
                                             .setTemperature(0.6F)
                                             .setRainfall(0.6F)));
    registerBiome(156, "mutated_birch_forest_hills",
                  new BiomeForestMutated((new Biome.BiomeProperties("Birch Forest Hills M"))
                                             .setBaseBiome("birch_forest_hills")
                                             .setBaseHeight(0.55F)
                                             .setHeightVariation(0.5F)
                                             .setTemperature(0.6F)
                                             .setRainfall(0.6F)));
    registerBiome(157, "mutated_roofed_forest",
                  new BiomeForest(BiomeForest.Type.ROOFED, (new Biome.BiomeProperties("Roofed Forest M"))
                                                               .setBaseBiome("roofed_forest")
                                                               .setBaseHeight(0.2F)
                                                               .setHeightVariation(0.4F)
                                                               .setTemperature(0.7F)
                                                               .setRainfall(0.8F)));
    registerBiome(158, "mutated_taiga_cold",
                  new BiomeTaiga(BiomeTaiga.Type.NORMAL, (new Biome.BiomeProperties("Cold Taiga M"))
                                                             .setBaseBiome("taiga_cold")
                                                             .setBaseHeight(0.3F)
                                                             .setHeightVariation(0.4F)
                                                             .setTemperature(-0.5F)
                                                             .setRainfall(0.4F)
                                                             .setSnowEnabled()));
    registerBiome(160, "mutated_redwood_taiga",
                  new BiomeTaiga(BiomeTaiga.Type.MEGA_SPRUCE, (new Biome.BiomeProperties("Mega Spruce Taiga"))
                                                                  .setBaseBiome("redwood_taiga")
                                                                  .setBaseHeight(0.2F)
                                                                  .setHeightVariation(0.2F)
                                                                  .setTemperature(0.25F)
                                                                  .setRainfall(0.8F)));
    registerBiome(161, "mutated_redwood_taiga_hills",
                  new BiomeTaiga(BiomeTaiga.Type.MEGA_SPRUCE, (new Biome.BiomeProperties("Redwood Taiga Hills M"))
                                                                  .setBaseBiome("redwood_taiga_hills")
                                                                  .setBaseHeight(0.2F)
                                                                  .setHeightVariation(0.2F)
                                                                  .setTemperature(0.25F)
                                                                  .setRainfall(0.8F)));
    registerBiome(162, "mutated_extreme_hills_with_trees",
                  new BiomeHills(BiomeHills.Type.MUTATED, (new Biome.BiomeProperties("Extreme Hills+ M"))
                                                              .setBaseBiome("extreme_hills_with_trees")
                                                              .setBaseHeight(1.0F)
                                                              .setHeightVariation(0.5F)
                                                              .setTemperature(0.2F)
                                                              .setRainfall(0.3F)));
    registerBiome(163, "mutated_savanna",
                  new BiomeSavannaMutated((new Biome.BiomeProperties("Savanna M"))
                                              .setBaseBiome("savanna")
                                              .setBaseHeight(0.3625F)
                                              .setHeightVariation(1.225F)
                                              .setTemperature(1.1F)
                                              .setRainfall(0.0F)
                                              .setRainDisabled()));
    registerBiome(164, "mutated_savanna_rock",
                  new BiomeSavannaMutated((new Biome.BiomeProperties("Savanna Plateau M"))
                                              .setBaseBiome("savanna_rock")
                                              .setBaseHeight(1.05F)
                                              .setHeightVariation(1.2125001F)
                                              .setTemperature(1.0F)
                                              .setRainfall(0.0F)
                                              .setRainDisabled()));
    registerBiome(165, "mutated_mesa",
                  new BiomeMesa(true, false,
                                (new Biome.BiomeProperties("Mesa (Bryce)"))
                                    .setBaseBiome("mesa")
                                    .setTemperature(2.0F)
                                    .setRainfall(0.0F)
                                    .setRainDisabled()));
    registerBiome(166, "mutated_mesa_rock",
                  new BiomeMesa(false, true,
                                (new Biome.BiomeProperties("Mesa Plateau F M"))
                                    .setBaseBiome("mesa_rock")
                                    .setBaseHeight(0.45F)
                                    .setHeightVariation(0.3F)
                                    .setTemperature(2.0F)
                                    .setRainfall(0.0F)
                                    .setRainDisabled()));
    registerBiome(167, "mutated_mesa_clear_rock",
                  new BiomeMesa(false, false,
                                (new Biome.BiomeProperties("Mesa Plateau M"))
                                    .setBaseBiome("mesa_clear_rock")
                                    .setBaseHeight(0.45F)
                                    .setHeightVariation(0.3F)
                                    .setTemperature(2.0F)
                                    .setRainfall(0.0F)
                                    .setRainDisabled()));
}
SpawnListEntry::SpawnListEntry(Entity *entityclassIn, int32_t weight, int32_t groupCountMin, int32_t groupCountMax)
    : WeightedItem(weight)
{
    entityClass   = entityclassIn;
    minGroupCount = groupCountMin;
    maxGroupCount = groupCountMax;
}
std::string SpawnListEntry::toString() const
{
    return entityClass->getSimpleName() + "*(" + minGroupCount + "-" + maxGroupCount + "):" + itemWeight;
}
