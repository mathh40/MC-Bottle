#pragma once
#include "TextComponentBase.h"

class TextComponentScore : public TextComponentBase
{
  public:
    TextComponentScore(std::string nameIn, std::string objectiveIn);
    std::string getName();
    std::string getObjective();
    void setValue(std::string valueIn);
    std::string getUnformattedComponentText() const override;
    void resolve(ICommandSender sender);
    std::string toString() const override;

  private:
    std::string name;
    std::string objective;
    std::string value = "";
};
