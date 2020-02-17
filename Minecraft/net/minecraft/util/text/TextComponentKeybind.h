#include "TextComponentBase.h"

class TextComponentKeybind :public TextComponentBase
{
public:
	TextComponentKeybind(const std::string &keybind);
	std::string getUnformattedText() const override;
	std::string toString() const override;
	std::string getKeybind() const;
private:
	std::string keybind;
};
