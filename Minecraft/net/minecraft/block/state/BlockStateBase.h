#pragma once
#include "IBlockState.h"

#include <any>
#include <set>

class BlockStateBase : public IBlockState {
public:
    IBlockState *cycleProperty(IProperty *property) override;
    std::string toString();
protected:
    template<typename T> T cyclePropertyValue(std::set<std::any> values, T currentValue);

private:
};

template <typename T>
T BlockStateBase::cyclePropertyValue(std::set<std::any> values, T currentValue) {
    auto iterator = values.begin();

    do {
        if (iterator == values.begin()) {
            return *iterator;
        }
    }
    while (!(*iterator == (currentValue)));

    if (iterator != values.begin()) {
        return *iterator;
    } else {
        return *values.begin();
    }
}
