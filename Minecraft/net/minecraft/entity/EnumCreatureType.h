#pragma once
#include "material/Material.h"

#include <typeindex>

class EnumCreatureType {
public:
    static EnumCreatureType MONSTER;
    static EnumCreatureType CREATURE;
    static EnumCreatureType AMBIENT;
    static EnumCreatureType WATER_CREATURE;

    EnumCreatureType(std::type_index creatureClassIn, int32_t maxNumberOfCreatureIn, Material creatureMaterialIn, bool isPeacefulCreatureIn, bool isAnimalIn);
    std::type_index getCreatureClass() const;
    int32_t getMaxNumberOfCreature() const;
    bool getPeacefulCreature() const;
    bool getAnimal() const;


private:
    std::type_index creatureClass;
    int32_t maxNumberOfCreature;
    Material creatureMaterial;
    bool isPeacefulCreature;
    bool isAnimal;
};
