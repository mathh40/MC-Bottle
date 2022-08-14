#include "ModifiableAttributeInstance.h"



#include "AbstractAttributeMap.h"
#include "AttributeModifier.h"
#include "IAttribute.h"

ModifiableAttributeInstance::ModifiableAttributeInstance(AbstractAttributeMap *attributeMapIn,
                                                         IAttribute *genericAttributeIn)
        :attributeMap(attributeMapIn),genericAttribute(genericAttributeIn),baseValue(genericAttributeIn->getDefaultValue())
{
    for(auto i = 0; i < 3; ++i) 
    {
        mapByOperation.emplace(i, {});
    }
}

IAttribute * ModifiableAttributeInstance::getAttribute()
{
    return genericAttribute;
}

double ModifiableAttributeInstance::getBaseValue()
{
    return baseValue;
}

void ModifiableAttributeInstance::setBaseValue(double baseValue)
{
    if (baseValue != getBaseValue()) 
    {
        baseValue = baseValue;
        flagForUpdate();
    }
}

std::vector<AttributeModifier> ModifiableAttributeInstance::getModifiersByOperation(int32_t operation)
{
    return mapByOperation.find(operation)->second;
}

std::vector<AttributeModifier> ModifiableAttributeInstance::getModifiers()
{
    std::unordered_set<> set;

    for(auto i = 0; i < 3; ++i) 
    {
        set.addAll(getModifiersByOperation(i));
    }

    return set;    
}

AttributeModifier ModifiableAttributeInstance::getModifier(const xg::Guid &uuid)
{
    return mapByUUID.find(uuid)->second;
}

bool ModifiableAttributeInstance::hasModifier(const AttributeModifier &modifier)
{
    return mapByUUID.find(modifier.getID()) != mapByUUID.end();
}

void ModifiableAttributeInstance::applyModifier(const AttributeModifier &modifier)
{
    if (getModifier(modifier.getID()) != nullptr) 
    {
        throw std::logic_error("Modifier is already applied on this attribute!");
    }
    else 
    {
        Set set = (Set)mapByName.get(modifier.getName());
        if (set.empty()) 
        {
            set = Sets.newHashSet();
            mapByName.put(modifier.getName(), set);
        }

        ((Set)mapByOperation.get(modifier.getOperation())).add(modifier);
        ((Set)set).add(modifier);
        mapByUUID.put(modifier.getID(), modifier);
        flagForUpdate();
    }
}

void ModifiableAttributeInstance::removeModifier(const AttributeModifier &modifier)
{
    for(auto i = 0; i < 3; ++i) 
    {
        Set set = (Set)mapByOperation.get(i);
        set.remove(modifier);
    }

    Set set1 = (Set)mapByName.get(modifier.getName());
    if (set1 != nullptr) 
    {
        set1.remove(modifier);
        if (set1.isEmpty()) 
        {
            mapByName.remove(modifier.getName());
        }
    }

    mapByUUID.remove(modifier.getID());
    flagForUpdate();
}

void ModifiableAttributeInstance::removeModifier(const xg::Guid &p_188479_1_)
{
    AttributeModifier attributemodifier = getModifier(p_188479_1_);
    if (attributemodifier != nullptr) 
    {
        removeModifier(attributemodifier);
    }
}

void ModifiableAttributeInstance::removeAllModifiers()
{
    std::vector<AttributeModifier> collection = getModifiers();
    if (collection != nullptr) 
    {
        Iterator var2 = Lists.newArrayList(collection).iterator();

        while(var2.hasNext()) {
            AttributeModifier attributemodifier = (AttributeModifier)var2.next();
            removeModifier(attributemodifier);
        }
    }
}

double ModifiableAttributeInstance::getAttributeValue()
{
    if (needsUpdate) 
    {
        cachedValue = computeValue();
        needsUpdate = false;
    }

    return cachedValue;
}

void ModifiableAttributeInstance::flagForUpdate()
{
    needsUpdate = true;
    attributeMap->onAttributeModified(this);
}

double ModifiableAttributeInstance::computeValue()
{
    double d0 = getBaseValue();

    AttributeModifier attributemodifier;
    for(Iterator var3 = getAppliedModifiers(0).iterator(); var3.hasNext(); d0 += attributemodifier.getAmount()) 
    {
        attributemodifier = (AttributeModifier)var3.next();
    }

    double d1 = d0;

    Iterator var5;
    AttributeModifier attributemodifier2;
    for(var5 = getAppliedModifiers(1).iterator(); var5.hasNext(); d1 += d0 * attributemodifier2.getAmount()) 
    {
        attributemodifier2 = (AttributeModifier)var5.next();
    }

    for(var5 = getAppliedModifiers(2).iterator(); var5.hasNext(); d1 *= 1.0 + attributemodifier2.getAmount()) 
    {
        attributemodifier2 = (AttributeModifier)var5.next();
    }

    return genericAttribute->clampValue(d1);
}

std::vector<AttributeModifier> ModifiableAttributeInstance::getAppliedModifiers(int32_t operation)
{
    Set set = Sets.newHashSet(getModifiersByOperation(operation));

    for(IAttribute* iattribute = genericAttribute->getParent(); iattribute != nullptr; iattribute = iattribute->getParent()) 
    {
        IAttributeInstance* iattributeinstance = attributeMap->getAttributeInstance(iattribute);
        if (iattributeinstance != nullptr) 
        {
            set.addAll(iattributeinstance->getModifiersByOperation(operation));
        }
    }

    return set;
}
