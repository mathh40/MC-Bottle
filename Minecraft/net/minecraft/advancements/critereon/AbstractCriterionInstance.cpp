#include "AbstractCriterionInstance.h"

AbstractCriterionInstance::AbstractCriterionInstance(const ResourceLocation &criterionIn)
  : ICriterionInstance(), criterion(criterionIn)
{}
ResourceLocation& AbstractCriterionInstance::getId() const {
    return criterion;
}

std::string toString() const {
    return fmt::format("AbstractCriterionInstance{criterion={}}",criterion);
}
