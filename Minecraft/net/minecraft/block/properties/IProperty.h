#pragma once
#include <any>
#include <string>
#include <optional>
#include <set>


class IProperty {
protected:
    ~IProperty() = default;
public:
    virtual std::string getName() = 0;
    virtual std::set<std::any> getAllowedValues() = 0;
    virtual std::any getValueClass() = 0;
    virtual std::optional<std::any> parseValue(const std::string &var1) = 0;
    virtual std::string getName(std::any var1) = 0;
};
