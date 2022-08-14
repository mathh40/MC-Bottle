#include "ITextComponent.h"
#include "Style.h"
#include "../Util.h"
#include "TextComponentString.h"
#include "TextComponentTranslation.h"
#include "../../nlohmann_json/include/nlohmann/json.hpp"
#include "TextComponentScore.h"
#include "TextComponentSelector.h"
#include "TextComponentKeybind.h"

namespace ns {

    void serializeChatStyle(Style &style, json &object) {
        json jsonelement = json::object();
        to_json(jsonelement,style);
        if (jsonelement.is_object()) {
            auto &jsonobject = jsonelement;

            for (auto &entry : jsonobject.items()) {
                object[entry.key()] = entry.value();
            }
        }
    }

    void to_json(json &jsonobject, const ITextComponent *p_serialize_1_) {
        if (!p_serialize_1_->getStyle().isEmpty()) {
            serializeChatStyle(p_serialize_1_->getStyle(), jsonobject);
        }

        if (!p_serialize_1_->getSiblings().empty()) {
            json jsonarray = json::array();

            for (auto &itextcomponent : p_serialize_1_->getSiblings()) {
                to_json(jsonarray,itextcomponent.get());
            }

            jsonobject["extra"] == jsonarray;
        }

        if (Util::instanceof<TextComponentString>(p_serialize_1_)) {
            jsonobject["text"] = (reinterpret_cast<TextComponentString*>(p_serialize_1_)->getText());
        } else if (Util:: instanceof <TextComponentTranslation>(p_serialize_1_)) {
            auto textcomponenttranslation = reinterpret_cast<TextComponentTranslation *>(p_serialize_1_);
            jsonobject["translate"] = textcomponenttranslation->getKey());
            if (textcomponenttranslation->getFormatArgs() != nullptr &&
                textcomponenttranslation->getFormatArgs().length > 0) {
                json jsonarray1 = json::array();
                auto var17 = textcomponenttranslation->getFormatArgs();
                int var8 = var17.length;

                for (int var9 = 0; var9 < var8; ++var9) {
                    Object object = var17[var9];
                    if (object instanceof ITextComponent) {
                        jsonarray1[](
                            serialize((ITextComponent)((ITextComponent)object), object.getClass(),
                                                      ));
                    } else {
                        jsonarray1[](new JsonPrimitive(String.valueOf(object)));
                    }
                }

                jsonobject["with"] = jsonarray1;
            }
        } else if (Util:: instanceof <TextComponentScore>(p_serialize_1_)) {
            auto textcomponentscore = reinterpret_cast<TextComponentScore*>(p_serialize_1_);
            json jsonobject1 = json::object();
            jsonobject1["name"] = textcomponentscore->getName();
            jsonobject1["objective"] = textcomponentscore->getObjective();
            jsonobject1["value"] = textcomponentscore->getUnformattedComponentText();
            jsonobject["score"] = jsonobject1;
        } else if (Util:: instanceof <TextComponentSelector>(p_serialize_1_)) {
            auto textcomponentselector = reinterpret_cast <TextComponentSelector*>(p_serialize_1_);
            jsonobject["selector"] = textcomponentselector->getSelector());
        } else {
            if (!(Util:: instanceof <TextComponentKeybind>(p_serialize_1_))) {
                throw std::logic_error("Don't know how to serialize " + p_serialize_1_->getUnformattedText() +
                                               " as a Component");
            }

            auto textcomponentkeybind = reinterpret_cast <TextComponentKeybind*>(p_serialize_1_);
            jsonobject["keybind"] = textcomponentkeybind->getKeybind();
        }
    }

    void from_json(const json &j, std::shared_ptr<ITextComponent> &p) {

        if (j.is_primitive()) {
            p = std::make_shared<TextComponentString>(j.get<std::string>());
        } else if (j.is_object()) {
            if (j.is_array()) {
                std::shared_ptr<ITextComponent> itextcomponent1 = nullptr;
                std::shared_ptr<ITextComponent> itextcomponent2 = nullptr;
                for (auto it : j) {
                    from_json(it, itextcomponent2);
                    if (itextcomponent1 == nullptr) {
                        itextcomponent1 = std::move(itextcomponent2);
                        itextcomponent1->appendSibling(itextcomponent2);
                    }
                }

                p = std::move(itextcomponent1);
            } else {
                throw std::logic_error("Don't know how to turn " + j.dump() + " into a Component");
            }
        } else {
            std::shared_ptr<ITextComponent> itextcomponent = nullptr;

            if (j.find("text") != j.end()) {
                auto text = j.at("text").get<std::string>();
                itextcomponent = std::make_shared<TextComponentString>(text);
            } else if (j.find("translate") != j.end()) {
                auto translate = j.at("translate").get<std::string>();
                if (j.find("with") != j.end()) {
                    std::shared_ptr<ITextComponent> itextcomponent2 = nullptr;
                    json jsonarray = j.at("with");
                    std::vector<json> aobject(jsonarray.size());
                    for (auto &object : aobject) {
                        from_json(object, itextcomponent2);
                        auto textcomponentstring = reinterpret_cast<TextComponentString *>(itextcomponent2);
                        if (textcomponentstring) {
                            if (textcomponentstring.getStyle().isEmpty() &&
                                textcomponentstring.getSiblings().isEmpty()) {
                                object = textcomponentstring.getText();
                            }
                        }
                    }
                    itextcomponent = std::shared_ptr<TextComponentTranslation>(translate, aobject);
                } else {
                    itextcomponent = std::shared_ptr<TextComponentTranslation>(translate, std::nullopt);
                }
            } else if (j.find("score") != j.end()) {
                auto score = j.at("score");
                if (score.find("name") == score.end() || score.find("objective") == score.end()) {
                    throw std::logic_error("A score component needs a least a name and an objective");
                }
                itextcomponent = std::shared_ptr<TextComponentScore>(score.at("name").get<std::string>(),
                                                                     score.at("objective").get<std::string>());
                if (score.find("value") != score.end()) {
                    ((TextComponentScore *)itextcomponent.get()).setValue(score.at("value").get<std::string>());
                }
            } else if (j.find("selector") != j.end()) {
                itextcomponent = std::shared_ptr<TextComponentSelector>(j.at("selector").get<std::string>());
            } else {
                if (j.find("keybind") == j.end()) {
                    throw std::logic_error("Don't know how to turn " + j.dump() + " into a Component");
                }

                itextcomponent = std::shared_ptr<TextComponentKeybind>(j.at("keybind").get<std::string>());
            }
            if (j.find("extra") != j.end()) {
                auto jsonarray2 = j.at("extra");
                if (jsonarray2.size() <= 0) {
                    throw std::logic_error("Unexpected empty array of components");
                }

                for (auto sibling : jsonarray2) {

                    std::shared_ptr<ITextComponent> itextcomponent2 = nullptr;
                    from_json(sibling, itextcomponent2);
                    itextcomponent->appendSibling(itextcomponent2);
                }
            }
            Style style;
            from_json(j, style);
            itextcomponent->setStyle(style);
        }
    }
} // namespace ns
