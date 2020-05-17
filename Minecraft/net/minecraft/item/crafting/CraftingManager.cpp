#include "CraftingManager.h"

#include <combaseapi.h>
#include <fstream>



#include "RecipeBookCloning.h"
#include "RecipeFireworks.h"
#include "RecipeRepairItem.h"
#include "RecipesArmorDyes.h"
#include "RecipesBanners.h"
#include "RecipesMapCloning.h"
#include "RecipesMapExtending.h"
#include "RecipeTippedArrow.h"
#include "ShapelessRecipes.h"
#include "ShieldRecipes.h"
#include "ShulkerBoxRecipes.h"

std::shared_ptr<spdlog::async_logger> CraftingManager::LOGGER = spdlog::get("Minecraft")->clone("CraftingManager");

bool CraftingManager::init()
{
    try {
        registery("armordye", std::make_unique<RecipesArmorDyes>());
        registery("bookcloning", std::make_unique< RecipeBookCloning>());
        registery("mapcloning", std::make_unique< RecipesMapCloning>());
        registery("mapextending", std::make_unique< RecipesMapExtending>());
        registery("fireworks", std::make_unique< RecipeFireworks>());
        registery("repairitem", std::make_unique< RecipeRepairItem()>);
        registery("tippedarrow", std::make_unique< RecipeTippedArrow()>);
        registery("bannerduplicate", std::make_unique< RecipesBanners::RecipeDuplicatePattern()>);
        registery("banneraddpattern", std::make_unique< RecipesBanners::RecipeAddPattern>());
        registery("shielddecoration", std::make_unique< ShieldRecipes::Decoration>());
        registery("shulkerboxcoloring", std::make_unique< ShulkerBoxRecipes::ShulkerBoxColoring>());
        return parseJsonRecipes();
    }
    catch (std::exception var1) 
    {
        return false;
    }
}

void CraftingManager::registery(std::string name, IRecipe* recipe)
{
    registery(ResourceLocation(name), recipe);
}

void CraftingManager::registery(ResourceLocation name, IRecipe *recipe)
{
    if (REGISTRY.containsKey(name)) 
    {
        throw std::runtime_error("Duplicate recipe ignored with ID " + name.to_string());
    }
    else 
    {
        REGISTRY.registe(nextAvailableId++, name, recipe);
    }
}

ItemStack CraftingManager::findMatchingResult(InventoryCrafting* craftMatrix, World* worldIn)
{
    for(auto& irecipe : REGISTRY)
    {
        if (irecipe.second->matches(craftMatrix, worldIn)) 
        {
            return irecipe.second->getCraftingResult(craftMatrix);
        }
    }

    return ItemStack::EMPTY;
}

IRecipe * CraftingManager::findMatchingRecipe(InventoryCrafting *craftMatrix, World *worldIn)
{
    for(auto& irecipe : REGISTRY)
    {
        if (irecipe.second->matches(craftMatrix, worldIn)) 
        {
            return irecipe.second.get();
        }
    }

    return nullptr;
}

std::vector<ItemStack> CraftingManager::getRemainingItems(InventoryCrafting *craftMatrix, World *worldIn)
{
    for(auto& irecipe : REGISTRY)
    {
        if (irecipe.second->matches(craftMatrix, worldIn)) 
        {
            return irecipe.second->getRemainingItems(craftMatrix);
        }
    }

    std::vector<ItemStack> nonnulllist(craftMatrix->getSizeInventory(), ItemStack::EMPTY);

    for(auto i = 0; i < nonnulllist.size(); ++i) 
    {
        nonnulllist[i] = craftMatrix->getStackInSlot(i);
    }

    return nonnulllist;
}

IRecipe* CraftingManager::getRecipe(ResourceLocation name)
{
    return REGISTRY.getObject(name).value().get();
}

int32_t CraftingManager::getIDForRecipe(IRecipe *recipe)
{
    return REGISTRY.getIDForObject(recipe);
}

IRecipe * CraftingManager::getRecipeById(int32_t id)
{
    return REGISTRY.getObjectById(id).value().get();
}

bool CraftingManager::parseJsonRecipes()
{
    std::filesystem::path filesystem;
    json gson;

    try 
    {
        bool flag1 = false;
        std::filesystem::path path("/assets/.mcassetsroot");
        if (path.empty()) 
        {
            LOGGER->error("Couldn't find .mcassetsroot");
            flag1 = false;
            return flag1;
        }
        else 
        {
            path = "/assets/minecraft/recipes";
        }

        for(auto& path1 : path)
        {
            if (path1.extension() == ".json") 
            {
                std::filesystem::path s = path1.stem();
                ResourceLocation resourcelocation = ResourceLocation(s.string());
                try 
                {
                    std::ifstream bufferedreader(path1);
                    bufferedreader >> gson;
                    registery(s, parseRecipeJson(gson));
                }
                catch (json::exception& var27) 
                {
                    LOGGER->error("Parsing error loading recipe " + resourcelocation.to_string(), var27);
                    return false;
                }
                catch (std::exception& var28) 
                {
                    LOGGER->error("Couldn't read recipe " + resourcelocation.to_string() + " from " + path1.string(), var28);
                    return false;
                }
            }
        }
        return true;
    }
    catch (std::exception& var30) 
    {
        LOGGER->error("Couldn't get a list of all recipe files", var30);
        return false;
    }
}

std::unique_ptr<IRecipe> CraftingManager::parseRecipeJson(nlohmann::json &json)
{
    std::string s = json.value("type","");
    if ("crafting_shaped" == s) 
    {
        std::shared_ptr<ShapedRecipes>sharp;
        json.get_to<ShapedRecipes>(sharp);
        return sharp;
    }
    else if ("crafting_shapeless" == s) 
    {
        std::unique_ptr<ShapelessRecipes> sharpless;
        json.get_to(sharpless);
        return sharpless;
    }
    else 
    {
        throw std::runtime_error("Invalid or unsupported recipe type '" + s + "'");
    }
}
