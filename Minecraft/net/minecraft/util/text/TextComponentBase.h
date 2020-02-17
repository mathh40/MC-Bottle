#pragma once
#include "Style.h"
#include "ITextComponent.h"


class TextComponentBase :virtual  ITextComponent
{
public:
	virtual ~TextComponentBase() = default;
	ITextComponent* appendSibling(std::unique_ptr<ITextComponent> component) override;
	TextComponentList& getSiblings() const override;
	ITextComponent* appendText(std::string text)override;
	ITextComponent* setStyle(Style style) override;
	Style& getStyle() const override;
	std::string getUnformattedText() const override;
	std::string getFormattedText() const override;
	virtual std::string toString() const;
protected:
	TextComponentList siblings;
private:
	Style style;
};

namespace std {
	template <> struct hash<TextComponentBase>
	{
		size_t operator()(const TextComponentBase & x) const noexcept
		{
			return 31 * std::hash<Style>{}(x.getStyle()) + std::hash<ITextComponent::TextComponentList>{}(x.getSiblings());
		}
	};
}

