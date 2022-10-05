#pragma once
#include "../../../world/chunk/BlockStateContainer.h"

#include <functional>
#include <unordered_map>
class IProperty;
class IBlockState;
class Block;

class BlockStateMatcher
{
  public:
    static BlockStateMatcher forBlock(Block *blockIn);
    bool operator()(IBlockState *p_apply_1_);
    template <typename T> BlockStateMatcher where(IProperty *property, T is)
    {
        if (!blockstate.getProperties().contains(property))
        {
            throw std::logic_error(blockstate + " cannot support property " + property);
        }
        else
        {
            propertyPredicates.emplace(property, is);
            return *this;
        }
    }

  protected:
    template <typename T> bool matches(IBlockState *blockState, IProperty *property, T predicate)
    {
        return predicate(blockState.getValue(property));
    }

  private:
    BlockStateMatcher(BlockStateContainer blockStateIn);

    BlockStateContainer blockstate;
    std::unordered_map<IProperty *, std::function<bool()>> propertyPredicates;
};
