#pragma once
#include <string>

class EnumHandSide
{
  public:
    static EnumHandSide LEFT;
    static EnumHandSide RIGHT;

    EnumHandSide(ITextComponent nameIn);
    EnumHandSide opposite();
    std::string toString();

  private:
    ITextComponent handName;
    bool left;
};
