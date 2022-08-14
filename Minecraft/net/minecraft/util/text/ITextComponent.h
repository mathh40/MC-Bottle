#pragma once
#include <string>
#include <list>
#include <memory>
#include "../../nlohmann_json/include/nlohmann/json_fwd.hpp"

class Style;
class ITextComponent
{
public:

	using TextComponentList = std::list<std::shared_ptr<ITextComponent>>;
	virtual ~ITextComponent() = default;
    virtual ITextComponent *setStyle(const Style &var1) = 0;
	virtual Style& getStyle() = 0;
	virtual ITextComponent* appendText(std::string_view var1) = 0;
    virtual ITextComponent *appendSibling(std::shared_ptr<ITextComponent>& var1) = 0;
	virtual std::string getUnformattedComponentText() const = 0;
	virtual std::string getUnformattedText() const  = 0;
	virtual std::string getFormattedText() const  = 0;
	virtual TextComponentList& getSiblings() = 0;
	virtual ITextComponent* createCopy() = 0;

};

namespace ns {

	using json = nlohmann::json;
    void to_json(json &jsonobject, const ITextComponent *pp_serialize_1_);

	void from_json(const json &j, std::shared_ptr<ITextComponent> &p);


} // namespace ns