#pragma once
#include <string>
#include <list>
#include <memory>

class Style;

class ITextComponent
{
public:
	using TextComponentList = std::list<std::unique_ptr<ITextComponent>>;
	virtual ~ITextComponent() = default;
	virtual ITextComponent* setStyle(Style var1) = 0;
	virtual Style& getStyle() const = 0;
	virtual ITextComponent* appendText(std::string var1) = 0;
	virtual ITextComponent* appendSibling(std::unique_ptr<ITextComponent> var1) = 0;
	virtual std::string getUnformattedComponentText() const = 0;
	virtual std::string getUnformattedText() const  = 0;
	virtual std::string getFormattedText() const  = 0;
	virtual TextComponentList& getSiblings() const = 0;
	virtual ITextComponent* createCopy() = 0;

};


/*using nlohmann::json;

namespace ns {
	void to_json(json& j, const std::shared_ptr<ITextComponent>& p) {
		JsonObject jsonobject = new JsonObject();
		if (!p_serialize_1_.getStyle().isEmpty()) {
			this.serializeChatStyle(p_serialize_1_.getStyle(), jsonobject, p_serialize_3_);
		}

		if (!p_serialize_1_.getSiblings().isEmpty()) {
			JsonArray jsonarray = new JsonArray();
			Iterator var6 = p_serialize_1_.getSiblings().iterator();

			while (var6.hasNext()) {
				ITextComponent itextcomponent = (ITextComponent)var6.next();
				jsonarray.add(this.serialize((ITextComponent)itextcomponent, itextcomponent.getClass(), p_serialize_3_));
			}

			jsonobject.add("extra", jsonarray);
		}

		if (p_serialize_1_ instanceof TextComponentString) {
			jsonobject.addProperty("text", ((TextComponentString)p_serialize_1_).getText());
		}
		else if (p_serialize_1_ instanceof TextComponentTranslation) {
			TextComponentTranslation textcomponenttranslation = (TextComponentTranslation)p_serialize_1_;
			jsonobject.addProperty("translate", textcomponenttranslation.getKey());
			if (textcomponenttranslation.getFormatArgs() != null && textcomponenttranslation.getFormatArgs().length > 0) {
				JsonArray jsonarray1 = new JsonArray();
				Object[] var17 = textcomponenttranslation.getFormatArgs();
				int var8 = var17.length;

				for (int var9 = 0; var9 < var8; ++var9) {
					Object object = var17[var9];
					if (object instanceof ITextComponent) {
						jsonarray1.add(this.serialize((ITextComponent)((ITextComponent)object), object.getClass(), p_serialize_3_));
					}
					else {
						jsonarray1.add(new JsonPrimitive(String.valueOf(object)));
					}
				}

				jsonobject.add("with", jsonarray1);
			}
		}
		else if (p_serialize_1_ instanceof TextComponentScore) {
			TextComponentScore textcomponentscore = (TextComponentScore)p_serialize_1_;
			JsonObject jsonobject1 = new JsonObject();
			jsonobject1.addProperty("name", textcomponentscore.getName());
			jsonobject1.addProperty("objective", textcomponentscore.getObjective());
			jsonobject1.addProperty("value", textcomponentscore.getUnformattedComponentText());
			jsonobject.add("score", jsonobject1);
		}
		else if (p_serialize_1_ instanceof TextComponentSelector) {
			TextComponentSelector textcomponentselector = (TextComponentSelector)p_serialize_1_;
			jsonobject.addProperty("selector", textcomponentselector.getSelector());
		}
		else {
			if (!(p_serialize_1_ instanceof TextComponentKeybind)) {
				throw new IllegalArgumentException("Don't know how to serialize " + p_serialize_1_ + " as a Component");
			}

			TextComponentKeybind textcomponentkeybind = (TextComponentKeybind)p_serialize_1_;
			jsonobject.addProperty("keybind", textcomponentkeybind.getKeybind());
		}

		return jsonobject;
	}

	void from_json(const json& j, std::shared_ptr<ITextComponent>& p) {

		if (j.is_primitive())
		{
			return std::make_shared<TextComponentString>(j.get<std::string>());
		}
		else if (j.is_object())
		{
			if (j.is_array())
			{
				std::shared_ptr<ITextComponent> itextcomponent1 = nullptr;
				for (auto it = j.begin(); it != j.end(); ++it) {
					auto itextcomponent2 = it->get<std::shared_ptr<ITextComponent>>();
					if (itextcomponent1 == nullptr)
					{
						itextcomponent1 = std::move(itextcomponent2);
						itextcomponent1->appendSibling(itextcomponent2.get());
					}
				}

				p = std::move(itextcomponent1);
			}
			else
			{
				throw std::logic_error("Don't know how to turn " + j.dump() + " into a Component");
			}
		}
		else
		{
			std::shared_ptr<ITextComponent> itextcomponent = nullptr;

			if (j.find("text") != j.end())
			{
				auto text = j.at("text").get<std::string>();
				itextcomponent = std::make_shared <TextComponentString>(text);
			}
			else if (j.find("translate") != j.end())
			{
				auto translate = j.at("translate").get<std::string>();
				if (j.find("with") != j.end())
				{
					for (auto aobject : j.at("with"))
					{
						auto itextcomponent2 = aobject.get<std::shared_ptr<ITextComponent>>();
						auto textcomponentstring = reinterpret_cast<TextComponentString*>(itextcomponent2);
						if (textcomponentstring)
						{
							if (textcomponentstring.getStyle().isEmpty() && textcomponentstring.getSiblings().isEmpty()) {
								aobject = textcomponentstring.getText();
							}
						}
					}
					itextcomponent = std::shared_ptr <TextComponentTranslation>(translate, aobject);
				}
				else
				{
					itextcomponent = std::shared_ptr <TextComponentTranslation>(translate, std::nullopt);
				}
			}
			else if (j.find("score") != j.end())
			{
				auto score = j.at("score");
				if (score.find("name") == score.end() || score.find("objective") == score.end())
				{
					throw std::logic_error("A score component needs a least a name and an objective");
				}
				itextcomponent = std::shared_ptr < TextComponentScore>(score.at("name").get<std::string>(), score.at("objective").get<std::string>());
				if (score.find("value") != score.end()) {
					((TextComponentScore*)itextcomponent.get()).setValue(score.at("value").get<std::string>());
				}
			}
			else if (j.find("selector") != j.end())
			{
				itextcomponent = std::shared_ptr <TextComponentSelector>(j.at("selector").get<std::string>());
			}
			else
			{
				if (j.find("keybind") == j.end()) {
					throw std::logic_error("Don't know how to turn " + j.dump() + " into a Component");
				}

				itextcomponent = std::shared_ptr < TextComponentKeybind>(j.at("keybind").get<std::string>());
			}
			if (j.find("extra") != j.end())
			{
				auto jsonarray2 = j.at("extra");
				if (jsonarray2.size() <= 0) {
					throw std::logic_error("Unexpected empty array of components");
				}

				for (auto sibling : jsonarray2)
					itextcomponent->appendSibling(sibling.get<std::shared_ptr<ITextComponent>>());
			}
			itextcomponent.setStyle((Style)j.get<Style>());
		}
	}


} // namespace ns*/