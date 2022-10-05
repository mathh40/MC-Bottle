#pragma once
#include "../../../../crossguid/include/crossguid/guid.hpp"

#include <optional>
class Entity;
class IEntityOwnable
{
    virtual std::optional<xg::Guid> getOwnerId() = 0;

    virtual Entity *getOwner() = 0;
};
