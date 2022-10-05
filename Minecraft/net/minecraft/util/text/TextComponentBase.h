#pragma once
#include "Style.h"

class TextComponentBase : public ITextComponent
{
  public:
    ~TextComponentBase() override = default;
    ITextComponent *appendSibling(const std::shared_ptr<ITextComponent> &component) override;
    TextComponentList getSiblings() const override;
    ITextComponent *appendText(std::string_view text) override;
    ITextComponent *setStyle(const Style &style) override;
    const Style &getStyle() const override;
    Style &getStyle() override;
    std::string getUnformattedText() const override;
    std::string getFormattedText() const override;
    virtual std::string toString() const;
    size_t hashCode() const override;

  protected:
    TextComponentList siblings;

  private:
    Style textstyle;
};
