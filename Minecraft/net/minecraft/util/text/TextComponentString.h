#pragma once
#include "TextComponentBase.h"

class TextComponentString : public TextComponentBase
{
  public:
    TextComponentString(std::string_view msg);
    std::string getText() const;
    std::string getUnformattedText() const override;
    std::string toString() const override;

  private:
    std::string text;
};
