#pragma once
#include <any>
#include <string>
#include <optional>
#include <set>

class IProperty {
public:
    virtual ~IProperty() = default;
    virtual std::string getName() const = 0;
    virtual std::set<std::any> getAllowedValues() const = 0;
    virtual std::any getValueClass() const = 0;
    virtual std::optional<std::any> parseValue(std::string_view var1) = 0;
    virtual std::string getName(std::any var1) const = 0;
};
