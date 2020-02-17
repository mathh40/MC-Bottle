#pragma once
#include <any>


enum class EnumActionResult;

class ActionResult
{
public:
	ActionResult(EnumActionResult typeIn, std::any resultIn);
	EnumActionResult getType() const;
	std::any getResult() const;
private:
	const EnumActionResult type;
	const std::any result;
};

