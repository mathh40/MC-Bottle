#pragma once
#include "AbstractAttributeMap.h"

#include <unordered_set>

class ModifiableAttributeInstance;

class AttributeMap : public AbstractAttributeMap
{
  public:
    ModifiableAttributeInstance *getAttributeInstance(IAttribute *attribute);
    ModifiableAttributeInstance *getAttributeInstanceByName(std::string attributeName);
    IAttributeInstance *registerAttribute(IAttribute *attribute) override;
    void onAttributeModified(IAttributeInstance *instance) override;
    std::unordered_set<IAttributeInstance *> &getDirtyInstances();
    std::unordered_set<IAttributeInstance *> getWatchedAttributes() const;

  protected:
    IAttributeInstance *createInstance(IAttribute *attribute) override;

    std::unordered_map<std::string, IAttributeInstance> instancesByName;

  private:
    std::unordered_set<IAttributeInstance *> dirtyInstances;
};
