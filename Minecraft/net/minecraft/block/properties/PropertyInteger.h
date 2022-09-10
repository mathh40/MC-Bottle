#pragma once
#include "PropertyHelper.h"

class PropertyInteger : public PropertyHelper<uint32_t> {
public:
    std::set<std::any> getAllowedValues() const override;
    static PropertyInteger create(std::string_view name, uint32_t min, uint32_t max);
    std::optional<std::any> parseValue(std::string_view value) override;
    std::string getName(std::any value) const override;

  private:
    std::set<uint32_t> allowedValues;
protected:
    ~PropertyInteger() override = default;
    PropertyInteger(std::string_view name, uint32_t min, uint32_t max);
};
