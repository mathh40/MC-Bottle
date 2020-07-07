#pragma once
#include <unordered_map>
#include <unordered_set>
#include "IAttributeInstance.h"

class AbstractAttributeMap;
class IAttribute;

class ModifiableAttributeInstance :public IAttributeInstance
{
public:
    ModifiableAttributeInstance(AbstractAttributeMap* attributeMapIn, IAttribute* genericAttributeIn);
    IAttribute* getAttribute() override;
    double getBaseValue() override;
    void setBaseValue(double baseValue) override;
    std::vector<AttributeModifier> getModifiersByOperation(int32_t operation) override;
    std::vector<AttributeModifier> getModifiers() override;
    AttributeModifier getModifier(const xg::Guid& uuid) override;
    bool hasModifier(const AttributeModifier& modifier) override;
    void applyModifier(const AttributeModifier& modifier) override;
    void removeModifier(const AttributeModifier& modifier) override;
    void removeModifier(const xg::Guid& p_188479_1_) override;
    void removeAllModifiers() override;
    double getAttributeValue() override;

protected:
    void flagForUpdate();

private:
    double computeValue();
    std::vector<AttributeModifier> getAppliedModifiers(int32_t operation);

    AbstractAttributeMap* attributeMap;
    IAttribute* genericAttribute;
    std::unordered_map<int32_t,std::unordered_set<>> mapByOperation;
    std::unordered_map<std::string,AttributeModifier> mapByName;
    std::unordered_map<xg::Guid,std::unordered_set<>> mapByUUID;
    double baseValue;
    bool needsUpdate = true;
    double cachedValue;
};