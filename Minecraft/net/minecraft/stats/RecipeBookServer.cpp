#include "RecipeBookServer.h"
#include "spdlog/spdlog.h"
#include "NBTTagString.h"
#include "ResourceLocation.h"

std::shared_ptr<spdlog::logger> RecipeBookServer::LOGGER = spdlog::get("Minecraft")->clone("RecipeBook");

void RecipeBookServer::add(std::vector<IRecipe*> recipesIn, EntityPlayerMP* player)
{
   std::vector<IRecipe*> list;
   for(auto irecipe : recipesIn)
   {
      if (!recipes[getRecipeId(irecipe)] && !irecipe->isDynamic()) 
      {
         unlock(irecipe);
         markNew(irecipe);
         list.push_back(irecipe);
         CriteriaTriggers::RECIPE_UNLOCKED::trigger(player, irecipe);
      }
   }

   sendPacket(SPacketRecipeBook::State::ADD, player, list);
}

void RecipeBookServer::remove(std::vector<IRecipe*> recipesIn, EntityPlayerMP* player)
{
   std::vector<IRecipe*> list;

   for(auto irecipe : recipesIn) 
   {
      if (recipes[getRecipeId(irecipe)]) 
      {
         lock(irecipe);
         list.push_back(irecipe);
      }
   }

   sendPacket(SPacketRecipeBook::State::REMOVE, player, list);
}

std::shared_ptr<NBTTagCompound> RecipeBookServer::write()
{
   auto nbttagcompound = std::make_shared<NBTTagCompound>();
   nbttagcompound->setBoolean("isGuiOpen", isGuiOpen);
   nbttagcompound->setBoolean("isFilteringCraftable", isFilteringCraftable);
   auto nbttaglist = std::make_shared<NBTTagList>();
   auto recipesIn = getRecipes();

   for(auto irecipe : recipesIn)
   {
      nbttaglist->appendTag(std::make_shared<NBTTagString>(((ResourceLocation)CraftingManager::REGISTRY.getNameForObject(irecipe)).toString()));
   }

   nbttagcompound->setTag("recipes", nbttaglist);
   auto nbttaglist1 = std::make_shared<NBTTagList>();
   auto var7 = getDisplayedRecipes();


   for(auto irecipe1 : var7) 
   {
      nbttaglist1->appendTag(std::make_shared<NBTTagString>(((ResourceLocation)CraftingManager::REGISTRY.getNameForObject(irecipe1)).toString()));
   }

   nbttagcompound->setTag("toBeDisplayed", nbttaglist1);
   return nbttagcompound;
}

void RecipeBookServer::read(NBTTagCompound* tag)
{
   bisGuiOpen = tag->getBoolean("isGuiOpen");
   bisFilteringCraftable = tag->getBoolean("isFilteringCraftable");
   auto nbttaglist = tag->getTagList("recipes", 8);

   for(int i = 0; i < nbttaglist->tagCount(); ++i) 
   {
      ResourceLocation resourcelocation = ResourceLocation(nbttaglist->getStringTagAt(i));
      auto irecipe = CraftingManager::getRecipe(resourcelocation);
      if (irecipe == nullptr) 
      {
         LOGGER->info("Tried to load unrecognized recipe: {} removed now.", resourcelocation);
      }
      else 
      {
         unlock(irecipe);
      }
   }

   auto nbttaglist1 = tag->getTagList("toBeDisplayed", 8);

   for(int j = 0; j < nbttaglist1->tagCount(); ++j) {
      ResourceLocation resourcelocation1 = ResourceLocation(nbttaglist1->getStringTagAt(j));
      auto irecipe1 = CraftingManager::getRecipe(resourcelocation1);
      if (irecipe1 == nullptr) 
      {
         LOGGER->info("Tried to load unrecognized recipe: {} removed now.", resourcelocation1);
      }
      else 
      {
         markNew(irecipe1);
      }
   }
}

void RecipeBookServer::init(EntityPlayerMP* player)
{
   player->connection.sendPacket(SPacketRecipeBook(SPacketRecipeBook::State::INIT, getRecipes(), getDisplayedRecipes(), isGuiOpen, isFilteringCraftable));
}

void RecipeBookServer::sendPacket(SPacketRecipeBook::State state, EntityPlayerMP* player, std::vector<IRecipe*> recipesIn)
{
   player->connection.sendPacket(SPacketRecipeBook(state, recipesIn, Collections.emptyList(), isGuiOpen, isFilteringCraftable));
}

std::vector<IRecipe*> RecipeBookServer::getRecipes()
{
   std::vector<IRecipe*> list;

   for(int i = recipes.find_next(0); i >= 0; i = recipes.find_next(i + 1)) 
   {
      list.push_back(CraftingManager::REGISTRY::getObjectById(i));
   }

   return list;
}

std::vector<IRecipe*> RecipeBookServer::getDisplayedRecipes()
{
   std::vector<IRecipe*> list;

   for(int i = newRecipes.find_next(0); i >= 0; i = newRecipes.find_next(i + 1)) 
   {
      list.push_back(CraftingManager::REGISTRY::getObjectById(i));
   }

   return list;
}
