#pragma once
#include "../ICriterionInstance.h"
#include "../../util/ResourceLocation.h"

#include <string>

class AbstractCriterionInstance :public ICriterionInstance
{
public:
    AbstractCriterionInstance(const ResourceLocation& criterionIn);
    ResourceLocation& getId() const override;
    std::string toString() const;

private:
    ResourceLocation criterion;
};
