#include "ItemKnowledgeBook.h"

#include "../stats/RecipeBook.h"
#include "../stats/StatList.h"
#include "../world/World.h"
#include "ItemStack.h"
#include "spdlog/spdlog-inl.h"

std::shared_ptr<spdlog::logger> ItemKnowledgeBook::LOGGER = spdlog::get("Minecraft")->clone("ItemKnowledgeBook");

ItemKnowledgeBook::ItemKnowledgeBook()
{
    setMaxStackSize(1);
}

ActionResult ItemKnowledgeBook::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack            = playerIn->getHeldItem(handIn);
    NBTTagCompound *nbttagcompound = itemstack.getTagCompound();
    if (!playerIn->capabilities.isCreativeMode)
    {
        playerIn->setHeldItem(handIn, ItemStack::EMPTY);
    }

    if (nbttagcompound != nullptr && nbttagcompound->hasKey("Recipes", 9))
    {
        if (!worldIn->isRemote)
        {
            NBTTagList *nbttaglist = nbttagcompound->getTagList("Recipes", 8);
            std::vector<IRecipe *> list;
            for (int i = 0; i < nbttaglist->tagCount(); ++i)
            {
                auto s           = nbttaglist->getStringTagAt(i);
                IRecipe *irecipe = CraftingManager::getRecipe(ResourceLocation(s));
                if (irecipe == nullptr)
                {
                    LOGGER->error("Invalid recipe: " + s);
                    return ActionResult(EnumActionResult::FAIL, itemstack);
                }

                list.emplace_back(irecipe);
            }

            playerIn->unlockRecipes(list);
            playerIn->addStat(StatList::getObjectUseStats(this));
        }

        return ActionResult(EnumActionResult::SUCCESS, itemstack);
    }
    else
    {
        LOGGER->error("Tag not valid: " + nbttagcompound);
        return ActionResult(EnumActionResult::FAIL, itemstack);
    }
}
}
