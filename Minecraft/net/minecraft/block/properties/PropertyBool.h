#pragma once
#include "PropertyHelper.h"

#include <memory>
#include <optional>
#include <set>

class PropertyBool : public PropertyHelper<bool>
{
  public:
    static std::unique_ptr<PropertyBool> create(std::string_view name);
    std::optional<std::any> parseValue(std::string_view value) override;
    std::string getName(std::any value) const override;
    friend bool operator==(const PropertyBool &lhs, const PropertyBool &rhs);

  protected:
    PropertyBool(std::string_view name);

  private:
    std::set<bool> allowedValues = {true, false};
};
