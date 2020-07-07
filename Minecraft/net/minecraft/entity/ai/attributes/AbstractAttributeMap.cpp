#include "AbstractAttributeMap.h"
#include <stdexcept>
#include "IAttribute.h"
#include "Util.h"

IAttributeInstance * AbstractAttributeMap::getAttributeInstance(IAttribute *attribute)
{
    return attributes.find(attribute)->second;
}

IAttributeInstance * AbstractAttributeMap::getAttributeInstanceByName(const std::string& attributeName)
{
    return attributesByName.find(attributeName)->second;
}

IAttributeInstance * AbstractAttributeMap::registerAttribute(IAttribute *attribute)
{
    if (attributesByName.find(attribute->getName()) != attributesByName.end()) 
    {
        throw std::logic_error("Attribute is already registered!");
    }
    else 
    {
        IAttributeInstance* iattributeinstance = createInstance(attribute);
        attributesByName.emplace(attribute->getName(), iattributeinstance);
        attributes.emplace(attribute, iattributeinstance);

        for(IAttribute* iattribute = attribute->getParent(); iattribute != nullptr; iattribute = iattribute->getParent()) 
        {
            descendantsByParent.put(iattribute, attribute);
        }

        return iattributeinstance;
    }
}

std::vector<IAttributeInstance *> AbstractAttributeMap::getAllAttributes() const
{
    std::vector<IAttributeInstance *> collection;
    Util::Collection(collection,attributesByName);
    return collection;
}

void AbstractAttributeMap::onAttributeModified(IAttributeInstance *instance)
{

}

void AbstractAttributeMap::removeAttributeModifiers(const std::unordered_multimap<std::string,AttributeModifier> &modifiers)
{
    for(auto& entry : modifiers)
    {
        IAttributeInstance* iattributeinstance = getAttributeInstanceByName(entry.getKey());
        if (iattributeinstance != nullptr) 
        {
            iattributeinstance->removeModifier(entry.getValue());
        }
    }
}

void AbstractAttributeMap::applyAttributeModifiers(const std::unordered_multimap<std::string,AttributeModifier> &modifiers)
{
    for(auto& entry : modifiers)
    {
         IAttributeInstance* iattributeinstance = getAttributeInstanceByName(entry.getKey());
         if (iattributeinstance != nullptr) 
         {
            iattributeinstance->removeModifier(entry.getValue());
            iattributeinstance->applyModifier(entry.getValue());
         }
      }
}
