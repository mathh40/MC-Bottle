#pragma once
#include "PropertyHelper.h"
#include <set>
#include <memory>
#include <optional>

class PropertyBool : public PropertyHelper<bool> {
public:
    static std::unique_ptr<PropertyBool> create(const std::string &name);
    std::optional<std::any> parseValue(const std::string &value) override;
    std::string getName(std::any value) override;
    friend bool operator==(const PropertyBool &lhs, const PropertyBool &rhs);
protected:
    PropertyBool(const std::string &name);
private:
    std::set<bool> allowedValues = {true, false};
};
