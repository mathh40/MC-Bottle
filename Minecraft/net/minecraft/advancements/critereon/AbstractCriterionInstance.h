#pragma once
#include "../../util/ResourceLocation.h"
#include "../ICriterionInstance.h"

#include <string>

class AbstractCriterionInstance : public ICriterionInstance
{
  public:
    AbstractCriterionInstance(const ResourceLocation &criterionIn);
    ResourceLocation &getId() const override;
    std::string toString() const;

  private:
    ResourceLocation criterion;
};
