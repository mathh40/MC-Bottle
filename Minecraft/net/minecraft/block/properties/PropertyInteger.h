#pragma once
#include "PropertyHelper.h"

class PropertyInteger : public PropertyHelper<int32_t> {
public:
    std::set<std::any> getAllowedValues() override;
    static PropertyInteger create(const std::string &name, int32_t min, int32_t max);
    std::optional<std::any> parseValue(const std::string &value) override;
    std::string getName(std::any value) override;
private:
    std::set<int32_t> allowedValues;
protected:
    ~PropertyInteger() = default;
    PropertyInteger(const std::string &name, int32_t min, int32_t max);
};
