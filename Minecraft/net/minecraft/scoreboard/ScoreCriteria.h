#pragma once
#include "IScoreCriteria.h"

class ScoreCriteria :public IScoreCriteria
{
public:
   ScoreCriteria(std::string_view name);
   std::string getName() override;
   bool isReadOnly() override;
   IScoreCriteria::EnumRenderType getRenderType() override;
private:
   std::string dummyName;
};