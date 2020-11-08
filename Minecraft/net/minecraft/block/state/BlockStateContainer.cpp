#include "BlockStateContainer.h"
#include "../../../../../compile-time-regular-expressions/single-header/ctre.hpp"
#include "../../util/MapPopulator.h"
#include "../../util/math/Cartesian.h"

namespace state {
    static constexpr auto NAME_PATTERN = ctll::fixed_string{ "^[a-z0-9_]+$" };
    BlockStateContainer::BlockStateContainer(Block *blockIn, std::initializer_list<IProperty *> listproperties) :
        block(blockIn) {
        std::unordered_map<std::string, IProperty *> map;

        for (auto &iproperty : listproperties) {
            validateProperty(blockIn, iproperty);
            map.emplace(iproperty->getName(), iproperty);
        }

        properties = map;
        std::unordered_map<> map2;
        std::vector<std::set<std::any>> list1;
        auto var12 = Cartesian::cartesianProduct(getAllowedValues());
        std::vector<IProperty *> vints;
        vints.reserve(properties.size());
        for (auto const &imap : properties)
            vints.push_back(imap.second);

        for (auto list : var12) {
            auto map1 = MapPopulator::createMap(vints, list);
            StateImplementation blockstatecontainer$stateimplementation =
                StateImplementation(blockIn, ImmutableMap.copyOf(map1));
            map2.put(map1, blockstatecontainer$stateimplementation);
            list1.emplace_back(blockstatecontainer$stateimplementation);
        }

        for (auto blockstatecontainer$stateimplementation1 : list1) {
            blockstatecontainer$stateimplementation1.buildPropertyValueTable(map2);
        }

        validStates = list1;
    }

    std::string BlockStateContainer::validateProperty(Block *block, IProperty *property) {
        auto s = property->getName();

        if (!ctre::match<NAME_PATTERN>(s)) {
            throw std::runtime_error("Block: " + block->getLocalizedName() + " has invalidly named property: " + s);
        } else {
            auto t = property->getAllowedValues();

            std::string s1;
            do {
                if (!var3.hasNext()) {
                    return s;
                }

                s1 = property->getName(t);
            }
            while (ctre::match<NAME_PATTERN>(s1));

            throw std::runtime_error("Block: " + block->getLocalizedName() + " has property: " + s +
                                     " with invalidly named value: " + s1);
        }
    }

    std::vector<std::set<std::any>> BlockStateContainer::getValidStates() const { return validStates; }

    IBlockState *BlockStateContainer::getBaseState() { return std::any_cast<IBlockState *>(validStates[0]); }

    Block *BlockStateContainer::getBlock() { return block; }

    std::vector<IProperty *> BlockStateContainer::getProperties() {
        std::vector<IProperty *> immutablecollection;
        immutablecollection.reserve(properties.size());
        for (auto const &imap : properties)
            immutablecollection.push_back(imap.second);
        return immutablecollection;
    }

    std::vector<std::set<std::any>> BlockStateContainer::getAllowedValues() {
        std::vector<std::set<std::any>> list;

        std::vector<IProperty *> immutablecollection;
        immutablecollection.reserve(properties.size());
        for (auto const &imap : properties)
            immutablecollection.push_back(imap.second);

        for (const auto &iproperty : immutablecollection) {
            list.emplace_back(iproperty->getAllowedValues());
        }

        return list;
    }

    std::vector<IProperty *> StateImplementation::getPropertyKeys() {
        std::vector<IProperty *> collect;
        collect.resize(properties.size());
        for (auto &key : properties) {
            collect.emplace_back(key.first);
        }
        return collect;
    }

    StateImplementation::StateImplementation(Block *blockIn,
                                             std::unordered_map<std::string, IProperty *> propertiesIn) :
        block(blockIn),
        properties(propertiesIn) {
    }

    std::string StateImplementation::getValue(IProperty *propertys) {
        auto comparable = (Comparable)properties.get(propertys);
        if (comparable == nullptr) {
            throw new IllegalArgumentException("Cannot get property " + property + " as it does not exist in " +
                                               this.block.getBlockState());
        } else {
            return (Comparable)property.getValueClass().cast(comparable);
        }
    }

}
