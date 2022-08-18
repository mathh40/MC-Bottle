#pragma once
#include "AdvancementList.h"
#include <filesystem>

class AdvancementManager
{
public:
  AdvancementManager(const std::filesystem::path& advancementsDirIn);
  void reload();
  bool hasErrored();

private:
  std::unordered_map<>& loadCustomAdvancements();
  void loadBuiltInAdvancements(const std::unordered_map<>& map);
  std::optional<Advancement> getAdvancement(const ResourceLocation& id);
  std::unordered_map<>& getAdvancements();

  static std::shared_ptr<spdlog::logger> LOGGER;
   static Gson GSON = (new GsonBuilder()).registerTypeHierarchyAdapter(Advancement.Builder.class, new JsonDeserializer() {
      public Advancement.Builder deserialize(JsonElement p_deserialize_1_, Type p_deserialize_2_, JsonDeserializationContext p_deserialize_3_) throws JsonParseException {
         JsonObject jsonobject = JsonUtils.getJsonObject(p_deserialize_1_, "advancement");
         return Advancement.Builder.deserialize(jsonobject, p_deserialize_3_);
      }
   }).registerTypeAdapter(AdvancementRewards.class, new AdvancementRewards.Deserializer()).registerTypeHierarchyAdapter(ITextComponent.class, new ITextComponent.Serializer()).registerTypeHierarchyAdapter(Style.class, new Style.Serializer()).registerTypeAdapterFactory(new EnumTypeAdapterFactory()).create();
   static AdvancementList ADVANCEMENT_LIST;
   std::filesystem::path advancementsDir;
   bool hasErrored = false;
};
