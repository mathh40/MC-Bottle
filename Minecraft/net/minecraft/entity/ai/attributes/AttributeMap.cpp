#include "AttributeMap.h"

#include "IAttribute.h"
#include "IAttributeInstance.h"
#include "Util.h"

ModifiableAttributeInstance *AttributeMap::getAttributeInstance(IAttribute *attribute)
{
    return (ModifiableAttributeInstance *)AbstractAttributeMap::getAttributeInstance(attribute);
}

ModifiableAttributeInstance *AttributeMap::getAttributeInstanceByName(std::string attributeName)
{
    IAttributeInstance *iattributeinstance = AbstractAttributeMap::getAttributeInstanceByName(attributeName);
    if (iattributeinstance == nullptr)
    {
        iattributeinstance = instancesByName[attributeName];
    }

    return (ModifiableAttributeInstance *)iattributeinstance;
}

IAttributeInstance *AttributeMap::registerAttribute(IAttribute *attribute)
{
    IAttributeInstance *iattributeinstance = AbstractAttributeMap::registerAttribute(attribute);
    if (Util:: instanceof <RangedAttribute>(attribute) && ((RangedAttribute *)attribute).getDescription() != nullptr)
    {
        instancesByName.emplace(((RangedAttribute *)attribute).getDescription(), iattributeinstance);
    }

    return iattributeinstance;
}

void AttributeMap::onAttributeModified(IAttributeInstance *instance)
{
    if (instance->getAttribute()->getShouldWatch())
    {
        dirtyInstances.emplace(instance);
    }

    Iterator var2 = descendantsByParent.get(instance->getAttribute()).iterator();

    while (var2.hasNext())
    {
        IAttribute *iattribute                                   = (IAttribute)var2.next();
        ModifiableAttributeInstance *modifiableattributeinstance = getAttributeInstance(iattribute);
        if (modifiableattributeinstance != nullptr)
        {
            modifiableattributeinstance->flagForUpdate();
        }
    }
}

std::unordered_set<IAttributeInstance *> &AttributeMap::getDirtyInstances()
{
    return dirtyInstances;
}

std::unordered_set<IAttributeInstance *> AttributeMap::getWatchedAttributes() const
{
    std::unordered_set<IAttributeInstance *> set;

    for (auto &iattributeinstance : getAllAttributes())
    {
        if (iattributeinstance->getAttribute()->getShouldWatch())
        {
            set.emplace(iattributeinstance);
        }
    }

    return set;
}

IAttributeInstance *AttributeMap::createInstance(IAttribute *attribute)
{
    return new ModifiableAttributeInstance(this, attribute);
}
