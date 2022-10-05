#pragma once
#include "Item.h"
#include "spdlog/details/registry.h"

class ItemKnowledgeBook : public Item
{
  public:
    ItemKnowledgeBook();
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
};
