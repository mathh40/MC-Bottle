#pragma once
#include "Style.h"
#include "ITextComponent.h"


class TextComponentBase :public  ITextComponent
{
public:
	virtual ~TextComponentBase() = default;
  ITextComponent *appendSibling(std::shared_ptr<ITextComponent> &component) override;
	TextComponentList& getSiblings() override;
  ITextComponent *appendText(std::string_view text) override;
    ITextComponent *setStyle(const Style &style) override;
	Style& getStyle() override;
	std::string getUnformattedText() const override;
	std::string getFormattedText() const override;
	virtual std::string toString() const;
protected:
	TextComponentList siblings;
private:
	Style textstyle;
};

namespace std {
	template <> struct hash<TextComponentBase>
	{
		size_t operator()(const TextComponentBase & x) const noexcept
		{
            return 31 * std::hash<Style>{}(x.getStyle()) +
                   std::hash<ITextComponent::TextComponentList>{}(x.getSiblings());
		}
	};
}

