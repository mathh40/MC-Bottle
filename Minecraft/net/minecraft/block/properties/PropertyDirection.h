#pragma once
#include "../../util/EnumFacing.h"
#include "PropertyEnum.h"
#include <functional>


class PropertyDirection final : public PropertyEnum<EnumFacing> {
public:
  static PropertyDirection create(std::string_view name);
  static PropertyDirection create(std::string_view name, std::function<bool()> filter);
  static PropertyDirection create(std::string_view name, std::set<EnumFacing> &values);

protected:
    ~PropertyDirection() override = default;
    PropertyDirection(std::string_view name, std::set<EnumFacing> &values);
};
