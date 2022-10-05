#pragma once
#include "NBTTagCompound.h"
#include "RecipeBook.h"
#include "spdlog/logger.h"

class EntityPlayerMP;

class RecipeBookServer : public RecipeBook
{
  public:
    void add(std::vector<IRecipe *> recipesIn, EntityPlayerMP *player);
    void remove(std::vector<IRecipe *> recipesIn, EntityPlayerMP *player);
    std::shared_ptr<NBTTagCompound> write();
    void read(NBTTagCompound *tag);

  private:
    void sendPacket(SPacketRecipeBook::State state, EntityPlayerMP *player, std::vector<IRecipe *> recipesIn);
    std::vector<IRecipe *> getRecipes();
    std::vector<IRecipe *> getDisplayedRecipes();

    static std::shared_ptr<spdlog::logger> LOGGER;
};
