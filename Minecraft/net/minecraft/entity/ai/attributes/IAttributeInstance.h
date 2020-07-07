#pragma once
#include <cstdint>

class AttributeModifier;

namespace xg {
    class Guid;
}

class IAttribute;

class IAttributeInstance
{
public:
    virtual IAttribute* getAttribute() = 0;

    virtual double getBaseValue() = 0;

    virtual void setBaseValue(double var1) = 0;

    virtual std::vector<AttributeModifier> getModifiersByOperation(int32_t var1) = 0;

    virtual std::vector<AttributeModifier> getModifiers() = 0;

    virtual bool hasModifier(const AttributeModifier& var1) = 0;

    virtual AttributeModifier getModifier(const xg::Guid& var1) = 0;

    virtual void applyModifier(const AttributeModifier& var1) = 0;

    virtual void removeModifier(const AttributeModifier& var1) = 0;

    virtual void removeModifier(const xg::Guid& var1) = 0;

    virtual void removeAllModifiers() = 0;

    virtual double getAttributeValue() = 0;
};
