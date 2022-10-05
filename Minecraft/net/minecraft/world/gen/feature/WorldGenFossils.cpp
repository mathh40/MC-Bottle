#include "WorldGenFossils.h"

#include "Mirror.h"
#include "ResourceLocation.h"
#include "World.h"

ResourceLocation WorldGenFossils::STRUCTURE_SPINE_01      = ResourceLocation("fossils/fossil_spine_01");
ResourceLocation WorldGenFossils::STRUCTURE_SPINE_02      = ResourceLocation("fossils/fossil_spine_02");
ResourceLocation WorldGenFossils::STRUCTURE_SPINE_03      = ResourceLocation("fossils/fossil_spine_03");
ResourceLocation WorldGenFossils::STRUCTURE_SPINE_04      = ResourceLocation("fossils/fossil_spine_04");
ResourceLocation WorldGenFossils::STRUCTURE_SPINE_01_COAL = ResourceLocation("fossils/fossil_spine_01_coal");
ResourceLocation WorldGenFossils::STRUCTURE_SPINE_02_COAL = ResourceLocation("fossils/fossil_spine_02_coal");
ResourceLocation WorldGenFossils::STRUCTURE_SPINE_03_COAL = ResourceLocation("fossils/fossil_spine_03_coal");
ResourceLocation WorldGenFossils::STRUCTURE_SPINE_04_COAL = ResourceLocation("fossils/fossil_spine_04_coal");
ResourceLocation WorldGenFossils::STRUCTURE_SKULL_01      = ResourceLocation("fossils/fossil_skull_01");
ResourceLocation WorldGenFossils::STRUCTURE_SKULL_02      = ResourceLocation("fossils/fossil_skull_02");
ResourceLocation WorldGenFossils::STRUCTURE_SKULL_03      = ResourceLocation("fossils/fossil_skull_03");
ResourceLocation WorldGenFossils::STRUCTURE_SKULL_04      = ResourceLocation("fossils/fossil_skull_04");
ResourceLocation WorldGenFossils::STRUCTURE_SKULL_01_COAL = ResourceLocation("fossils/fossil_skull_01_coal");
ResourceLocation WorldGenFossils::STRUCTURE_SKULL_02_COAL = ResourceLocation("fossils/fossil_skull_02_coal");
ResourceLocation WorldGenFossils::STRUCTURE_SKULL_03_COAL = ResourceLocation("fossils/fossil_skull_03_coal");
ResourceLocation WorldGenFossils::STRUCTURE_SKULL_04_COAL = ResourceLocation("fossils/fossil_skull_04_coal");

std::array<ResourceLocation, 8> WorldGenFossils::FOSSILS{STRUCTURE_SPINE_01, STRUCTURE_SPINE_02, STRUCTURE_SPINE_03,
                                                         STRUCTURE_SPINE_04, STRUCTURE_SKULL_01, STRUCTURE_SKULL_02,
                                                         STRUCTURE_SKULL_03, STRUCTURE_SKULL_04};
std::array<ResourceLocation, 8> WorldGenFossils::FOSSILS_COAL{
    STRUCTURE_SPINE_01_COAL, STRUCTURE_SPINE_02_COAL, STRUCTURE_SPINE_03_COAL, STRUCTURE_SPINE_04_COAL,
    STRUCTURE_SKULL_01_COAL, STRUCTURE_SKULL_02_COAL, STRUCTURE_SKULL_03_COAL, STRUCTURE_SKULL_04_COAL};

bool WorldGenFossils::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    auto random          = worldIn->getChunk(position).getRandomWithSeed(987234911L);
    auto minecraftserver = worldIn->getMinecraftServer();
    auto arotation       = Rotation.values();
    auto rotation        = arotation[random(arotation.length)];
    auto i               = random(FOSSILS.size());
    auto templatemanager = worldIn->getSaveHandler().getStructureTemplateManager();
    auto templates       = templatemanager.getTemplate(minecraftserver, FOSSILS[i]);
    auto template1       = templatemanager.getTemplate(minecraftserver, FOSSILS_COAL[i]);
    const ChunkPos chunkpos(position);
    auto structureboundingbox(chunkpos.getXStart(), 0, chunkpos.getZStart(), chunkpos.getXEnd(), 256,
                              chunkpos.getZEnd());
    auto placementsettings =
        (PlacementSettings()).setRotation(rotation).setBoundingBox(structureboundingbox).setRandom(random);
    BlockPos blockpos = templates.transformedSize(rotation);
    auto j            = random(16 - blockpos.getx());
    auto k            = random(16 - blockpos.getz());
    auto l            = 256;

    for (auto i1 = 0; i1 < blockpos.getx(); ++i1)
    {
        for (auto j1 = 0; j1 < blockpos.getx(); ++j1)
        {
            l = MathHelper::min(l, worldIn->getHeight(position.getx() + i1 + j, position.getz() + j1 + k));
        }
    }

    auto i1            = MathHelper::max<int>(l - 15 - random(10), 10);
    BlockPos blockpos1 = templates.getZeroPositionWithTransform(position.add(j, i1, k), Mirror::NONE, rotation);
    placementsettings.setIntegrity(0.9F);
    templates.addBlocksToWorld(worldIn, blockpos1, placementsettings, 20);
    placementsettings.setIntegrity(0.1F);
    template1.addBlocksToWorld(worldIn, blockpos1, placementsettings, 20);
    return true;
}
