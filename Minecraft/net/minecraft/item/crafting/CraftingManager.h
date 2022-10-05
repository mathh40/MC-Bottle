#pragma once
#include "../../../../../nlohmann_json/include/nlohmann/json_fwd.hpp"
#include "../../../../../spdlog/include/spdlog/async_logger.h"
#include "IRecipe.h"
#include "registry/RegistryNamespaced.h"

#include <memory>

class CraftingManager
{
  public:
    static RegistryNamespaced<ResourceLocation, std::unique_ptr<IRecipe>> REGISTRY;
    static bool init();
    static void registery(std::string name, IRecipe *recipe);
    static void registery(ResourceLocation name, IRecipe *recipe);
    static ItemStack findMatchingResult(InventoryCrafting *craftMatrix, World *worldIn);
    static IRecipe *findMatchingRecipe(InventoryCrafting *craftMatrix, World *worldIn);
    static std::vector<ItemStack> getRemainingItems(InventoryCrafting *craftMatrix, World *worldIn);
    static IRecipe *getRecipe(ResourceLocation name);
    static int32_t getIDForRecipe(IRecipe *recipe);
    static IRecipe *getRecipeById(int32_t id);

  private:
    static std::shared_ptr<spdlog::async_logger> LOGGER;
    static int32_t nextAvailableId;
    static bool parseJsonRecipes();
    static std::unique_ptr<IRecipe> parseRecipeJson(nlohmann::json &json);
};
