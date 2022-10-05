#include "MapGenStructureIO.h"

#include "ComponentScatteredFeaturePieces.h"

std::shared_ptr<spdlog::logger> MapGenStructureIO::LOGGER = spdlog::get("Minecraft")->clone("MapGenStructureIO");

std::string MapGenStructureIO::getStructureStartName(const StructureStart &start)
{
    return startClassToNameMap.get(start.getClass());
}

std::string MapGenStructureIO::getStructureComponentName(const StructureComponent &component)
{
    return componentClassToNameMap.get(component.getClass());
}

std::optional<StructureStart> MapGenStructureIO::getStructureStart(NBTTagCompound *tagCompound, World *worldIn)
{
    StructureStart structurestart;

    try
    {
        Class oclass = (Class)startNameToClassMap.get(tagCompound.getString("id"));
        if (oclass != nullptr)
        {
            structurestart = (StructureStart)oclass.newInstance();
        }
    }
    catch (std::exception var4)
    {
        LOGGER->warn("Failed Start with id {}", tagCompound->getString("id"));
    }

    if (structurestart != nullptr)
    {
        structurestart.readStructureComponentsFromNBT(worldIn, tagCompound);
    }
    else
    {
        LOGGER->warn("Skipping Structure with id {}", tagCompound->getString("id"));
    }

    return structurestart;
}

StructureComponent *MapGenStructureIO::getStructureComponent(NBTTagCompound *tagCompound, World *worldIn)
{
    StructureComponent *structurecomponent = nullptr;

    try
    {
        Class oclass = (Class)componentNameToClassMap.get(tagCompound->getString("id"));
        if (oclass != nullptr)
        {
            structurecomponent = (StructureComponent)oclass.newInstance();
        }
    }
    catch (std::exception var4)
    {
        LOGGER->warn("Failed Piece with id {}", tagCompound->getString("id"));
        var4.printStackTrace();
    }

    if (structurecomponent != nullptr)
    {
        structurecomponent->readStructureBaseNBT(worldIn, tagCompound);
    }
    else
    {
        LOGGER->warn("Skipping Piece with id {}", tagCompound->getString("id"));
    }

    return structurecomponent;
}

registerStructure(StructureMineshaftStart.class, "Mineshaft");
registerStructure(MapGenVillage.Start.class, "Village");
registerStructure(MapGenNetherBridge.Start.class, "Fortress");
registerStructure(MapGenStronghold.Start.class, "Stronghold");
registerStructure(MapGenScatteredFeature.Start.class, "Temple");
registerStructure(StructureOceanMonument.StartMonument.class, "Monument");
registerStructure(MapGenEndCity.Start.class, "EndCity");
registerStructure(WoodlandMansion.Start.class, "Mansion");
StructureMineshaftPieces.registerStructurePieces();
StructureVillagePieces.registerVillagePieces();
StructureNetherBridgePieces.registerNetherFortressPieces();
StructureStrongholdPieces.registerStrongholdPieces();
ComponentScatteredFeaturePieces.registerScatteredFeaturePieces();
StructureOceanMonumentPieces.registerOceanMonumentPieces();
StructureEndCityPieces.registerPieces();
WoodlandMansionPieces.registerWoodlandMansionPieces();
