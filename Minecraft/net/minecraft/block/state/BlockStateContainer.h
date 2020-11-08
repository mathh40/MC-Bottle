#pragma once
#include "../properties/IProperty.h"
#include "BlockStateBase.h"

namespace state {
    class BlockStateContainer {
    public:
        BlockStateContainer(Block *blockIn, std::initializer_list<IProperty *> listproperties);
        std::string validateProperty(Block *block, IProperty *property);
        std::vector<std::set<std::any>> getValidStates() const;
        IBlockState *getBaseState();
        Block *getBlock();
        std::vector<IProperty *> getProperties();

    protected:
    private:
        Block *block;
        std::unordered_map<std::string, IProperty *> properties{};
        std::vector<std::set<std::any>> validStates{};

        std::vector<std::set<std::any>> getAllowedValues();
    };

    class StateImplementation : public BlockStateBase {
    public:
        std::vector<IProperty *> getPropertyKeys() override;
        std::function<bool()> getValue(IProperty *propertys) override;

    private:
        Block *block;
        std::unordered_map<std::string, IProperty *> properties;
        std::unordered_map<> propertyValueTable;

        StateImplementation(Block *blockIn, std::unordered_map<std::string, IProperty *> propertiesIn);
    };
}
