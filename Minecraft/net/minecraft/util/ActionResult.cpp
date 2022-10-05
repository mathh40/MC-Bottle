#include "ActionResult.h"

ActionResult::ActionResult(EnumActionResult typeIn, std::any resultIn) : type(typeIn), result(resultIn)
{
}

EnumActionResult ActionResult::getType() const
{
    return type;
}

std::any ActionResult::getResult() const
{
    return result;
}
