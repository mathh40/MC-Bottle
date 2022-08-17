#pragma once
#include "TextComponentBase.h"
#include <any>

class TextComponentTranslation : public TextComponentBase 
{
public:
    template <class... Args>
  TextComponentTranslation(std::string_view translationKey, Args... args) : TextComponentBase(), key(translationKey) ,formatArgs{args ...}
  {}
  ITextComponent* setStyle(Style style);
  std::string getUnformattedComponentText();
  ITextComponent* createCopy();
  friend bool operator==(const TextComponentTranslation &a,const TextComponentTranslation& b);
  std::string getKey() const;

protected:
    void initializeFromFormat(std::string_view format);

private:
    ITextComponent *getFormatArgumentAsComponent(uint64_t index);

    std::vector<std::any> formatArgs;
	std::string key;
    std::any syncLock;
	long lastTranslationUpdateTimeInMilliseconds = -1L;
};

namespace std {
	template <> struct hash<TextComponentTranslation>
	{
		size_t operator()(const TextComponentTranslation & x) const noexcept
		{
			size_t i = std::hash<ITextComponent>{}(x);
		    i = 31 * i + std::hash<std::string>{}(x.getKey());
			i = 31 * i + std::hash<std::vector<std::any>>{}(formatArgs);
			return i;
		}
	};
}

