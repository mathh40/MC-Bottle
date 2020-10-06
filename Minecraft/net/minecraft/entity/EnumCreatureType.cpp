#include "EnumCreatureType.h"
#include "material/Material.h"

static EnumCreatureType MONSTER = EnumCreatureType(typeid(IMob), 70, Material::AIR, false, false);
static EnumCreatureType CREATURE = EnumCreatureType(typeid(EntityAnimal), 10, Material::AIR, true, true);
static EnumCreatureType AMBIENT = EnumCreatureType(typeid(EntityAmbientCreature), 15, Material::AIR, true, false);
static EnumCreatureType WATER_CREATURE = EnumCreatureType(typeid(EntityWaterMob), 5, Material::WATER, true, false);

EnumCreatureType::EnumCreatureType(std::type_index creatureClassIn, int32_t maxNumberOfCreatureIn,
    Material creatureMaterialIn, bool isPeacefulCreatureIn, bool isAnimalIn) 
    :creatureClass(creatureClassIn),maxNumberOfCreature(maxNumberOfCreatureIn),creatureMaterial(creatureMaterialIn)
    ,isPeacefulCreature(isPeacefulCreatureIn),isAnimal(isAnimalIn)
{
}

std::type_index EnumCreatureType::getCreatureClass() const {
    return creatureClass;
}

int32_t EnumCreatureType::getMaxNumberOfCreature() const {
     return maxNumberOfCreature;
}

bool EnumCreatureType::getPeacefulCreature() const {
    return isPeacefulCreature;
}

bool EnumCreatureType::getAnimal() const {
    return isAnimal;
}
