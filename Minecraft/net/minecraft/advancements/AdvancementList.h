#pragma once
#include <memory>
#include <optional>
#include <set>
#include <unordered_map>

namespace spdlog {
class logger;
}

class ResourceLocation;
class Advancement;

class AdvancementList
{
public:
     class Listener {
     public:
      virtual void rootAdvancementAdded(Advancement var1) = 0;

      virtual void rootAdvancementRemoved(Advancement var1) = 0;

      virtual void nonRootAdvancementAdded(Advancement var1) = 0;

      virtual void nonRootAdvancementRemoved(Advancement var1) = 0;

      virtual void advancementsCleared() = 0;
   };

  void removeAll(const std::set<ResourceLocation>& ids);
  void loadAdvancements(const std::unordered_map<>& advancementsIn);
  void clear();
  std::set<>& getRoots();
  std::unordered_map<>& getAdvancements();
  std::optional<Advancement> getAdvancement(ResourceLocation id);
  void setListener(std::optional<Listener> listenerIn);

private:
  void remove(const Advancement& advancementIn);

  static std::shared_ptr<spdlog::logger> LOGGER;
   std::unordered_map<> advancements;
   std::set<>roots;
   std::set<> nonRoots;
   Listener* listener;

};
