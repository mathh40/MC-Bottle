#pragma once
#include "../util/ResourceLocation.h"

class ICriterionInstance
{
public:
  virtual ~ICriterionInstance() = default;
  virtual ResourceLocation& getId() const;
};
