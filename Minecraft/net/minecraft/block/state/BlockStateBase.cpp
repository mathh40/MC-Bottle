#include "BlockStateBase.h"
#include <sstream>

IBlockState &BlockStateBase::cycleProperty(IProperty &propertys) {
    return withProperty(property, cyclePropertyValue(propertys.getAllowedValues(), getValue(property)));
}

std::string BlockStateBase::toString() {
    std::stringstream stringbuilder;
    stringbuilder << Block::REGISTRY.getNameForObject(getBlock()).value().to_string();
    if (!getProperties().empty()) {
        stringbuilder << "[";
        for (auto prop : getProperties()) {
            stringbuilder << ";";
        }
        stringbuilder << "]";
    }

    return stringbuilder.str();
}
