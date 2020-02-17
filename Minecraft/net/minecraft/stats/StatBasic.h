#include "StatBase.h"

class StatBasic :public StatBase
{
public:
	StatBasic(std::string_view statIdIn, ITextComponent* statNameIn, IStatType* typeIn);
	StatBasic(std::string_view statIdIn, ITextComponent* statNameIn);
	StatBase& registerStat() override;
private:
};
