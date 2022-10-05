#pragma once
#include "AttributeModifier.h"

#include <unordered_map>

class IAttribute;
class IAttributeInstance;

class AbstractAttributeMap
{
  public:
    IAttributeInstance *getAttributeInstance(IAttribute *attribute);
    IAttributeInstance *getAttributeInstanceByName(const std::string &attributeName);
    virtual IAttributeInstance *registerAttribute(IAttribute *attribute);
    std::vector<IAttributeInstance *> getAllAttributes() const;
    virtual void onAttributeModified(IAttributeInstance *instance);
    void removeAttributeModifiers(const std::unordered_multimap<std::string, AttributeModifier> &modifiers);
    void applyAttributeModifiers(const std::unordered_multimap<std::string, AttributeModifier> &modifiers);

  protected:
    virtual IAttributeInstance *createInstance(IAttribute *var1) = 0;

    std::unordered_map<IAttribute *, IAttributeInstance *> attributes;
    std::unordered_map<std::string, IAttributeInstance *> attributesByName;
    std::unordered_multimap<std::string, AttributeModifier> descendantsByParent;
};
