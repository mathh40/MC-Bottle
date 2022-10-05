#include "CreativeTabs.h"

#include "../potion/PotionUtils.h"

static CreativeTabs CreativeTabs::BUILDING_BLOCKS = new CreativeTabs(0, "buildingBlocks"){
    public ItemStack createIcon(){return new ItemStack(Item.getItemFromBlock(Blocks.BRICK_BLOCK));
}
}
;
static CreativeTabs CreativeTabs::DECORATIONS =
    new CreativeTabs(1, "decorations"){public ItemStack createIcon(){return new ItemStack(
        Item.getItemFromBlock(Blocks.DOUBLE_PLANT), 1, BlockDoublePlant.EnumPlantType.PAEONIA.getMeta());
}
}
;
static CreativeTabs CreativeTabs::REDSTONE =
    new CreativeTabs(2, "redstone"){public ItemStack createIcon(){return new ItemStack(Items.REDSTONE);
}
}
;
static CreativeTabs CreativeTabs::TRANSPORTATION = new CreativeTabs(3, "transportation"){
    public ItemStack createIcon(){return new ItemStack(Item.getItemFromBlock(Blocks.GOLDEN_RAIL));
}
}
;
static CreativeTabs CreativeTabs::MISC =
    new CreativeTabs(6, "misc"){public ItemStack createIcon(){return new ItemStack(Items.LAVA_BUCKET);
}
}
;
   static CreativeTabs CreativeTabs::SEARCH = (new CreativeTabs(5, "search") {
      public ItemStack createIcon() {
         return new ItemStack(Items.COMPASS);
   }
   }).setBackgroundImageName("item_search.png");
   static CreativeTabs CreativeTabs::FOOD =
       new CreativeTabs(7, "food"){public ItemStack createIcon(){return new ItemStack(Items.APPLE);
   }
   }
   ;

   class Tools : public CreativeTabs
   {
     public:
       Tools();
       ItemStack createIcon() override;
   };

   Tools::Tools() : CreativeTabs(8, "tools")
   {
       std::vector<EnumEnchantmentType> types = {EnumEnchantmentType::ALL, EnumEnchantmentType::DIGGER,
                                                 EnumEnchantmentType::FISHING_ROD, EnumEnchantmentType::BREAKABLE};
       setRelevantEnchantmentTypes(types);
   }

   ItemStack Tools::createIcon()
   {
       return ItemStack(Items::IRON_AXE);
   }

   CreativeTabs *CreativeTabs::TOOLS = new Tools();

   class Combat : public CreativeTabs
   {
     public:
       Combat();
       ItemStack createIcon() override;
   };

   Combat::Combat() : CreativeTabs(9, "combat")
   {
       std::vector<EnumEnchantmentType> types = {EnumEnchantmentType::ALL,        EnumEnchantmentType::ARMOR,
                                                 EnumEnchantmentType::ARMOR_FEET, EnumEnchantmentType::ARMOR_HEAD,
                                                 EnumEnchantmentType::ARMOR_LEGS, EnumEnchantmentType::ARMOR_CHEST,
                                                 EnumEnchantmentType::BOW,        EnumEnchantmentType::WEAPON,
                                                 EnumEnchantmentType::WEARABLE,   EnumEnchantmentType::BREAKABLE};
       setRelevantEnchantmentTypes(types);
   }

   ItemStack Combat::createIcon()
   {
       return ItemStack(Items::GOLDEN_SWORD);
   }

   CreativeTabs *CreativeTabs::COMBAT = new Combat();

   class Brewing : public CreativeTabs
   {
     public:
       Brewing();
       ItemStack createIcon() override;
   };

   Brewing::Brewing() : CreativeTabs(10, "brewing")
   {
       std::vector<EnumEnchantmentType> types = {EnumEnchantmentType::ALL,        EnumEnchantmentType::ARMOR,
                                                 EnumEnchantmentType::ARMOR_FEET, EnumEnchantmentType::ARMOR_HEAD,
                                                 EnumEnchantmentType::ARMOR_LEGS, EnumEnchantmentType::ARMOR_CHEST,
                                                 EnumEnchantmentType::BOW,        EnumEnchantmentType::WEAPON,
                                                 EnumEnchantmentType::WEARABLE,   EnumEnchantmentType::BREAKABLE};
       setRelevantEnchantmentTypes(types);
   }

   ItemStack Brewing::createIcon()
   {
       return PotionUtils::addPotionToItemStack(ItemStack(Items::POTIONITEM), PotionTypes::WATER);
   }

   CreativeTabs *CreativeTabs::BREWING = new Brewing();
   MATERIALS                           = MISC;

   class Hotbar : public CreativeTabs
   {
     public:
       Hotbar();
       ItemStack createIcon() override;
       void displayAllRelevantItems(std::vector<ItemStack> &p_78018_1_);
       bool isAlignedRight();
   };

   Hotbar::Hotbar() : CreativeTabs(4, "hotbar")
   {
   }

   ItemStack Hotbar::createIcon()
   {
       return ItemStack(Blocks::BOOKSHELF);
   }

   void Hotbar::displayAllRelevantItems(std::vector<ItemStack> &p_78018_1_)
   {
       throw std::runtime_error("Implement exception client-side.");
   }

   bool Hotbar::isAlignedRight()
   {
       return true;
   }

   CreativeTabs *CreativeTabs::HOTBAR = new Hotbar();

   class Inventory : public CreativeTabs
   {
     public:
       Inventory();
       ItemStack createIcon() override;
   };

   Inventory::Inventory() : CreativeTabs(11, "inventory")
   {
       setBackgroundImageName("inventory.png");
       setNoScrollbar();
       setNoTitle();
   }

   ItemStack Inventory::createIcon()
   {
       return PotionUtils::addPotionToItemStack(ItemStack(Items::POTIONITEM), PotionTypes::WATER);
   }

   CreativeTabs *CreativeTabs::INVENTORY = new Inventory();

   CreativeTabs::CreativeTabs(uint32_t indexIn, std::string_view label)
       : index(indexIn), tabLabel(label), icon(ItemStack::EMPTY)
   {
       CREATIVE_TAB_ARRAY[index] = this;
   }

   uint32_t CreativeTabs::getIndex() const
   {
       return index;
   }

   std::string CreativeTabs::getTabLabel() const
   {
       return tabLabel;
   }

   std::string CreativeTabs::getTranslationKey() const
   {
       return "itemGroup." + getTabLabel();
   }

   ItemStack CreativeTabs::getIcon()
   {
       if (icon.isEmpty())
       {
           icon = createIcon();
       }

       return icon;
   }

   std::string CreativeTabs::getBackgroundImageName() const
   {
       return backgroundTexture;
   }

   CreativeTabs &CreativeTabs::setBackgroundImageName(std::string_view texture)
   {
       backgroundTexture = texture;
       return *this;
   }

   bool CreativeTabs::drawInForegroundOfTab() const
   {
       return drawTitle;
   }

   CreativeTabs &CreativeTabs::setNoTitle()
   {
       drawTitle = false;
       return *this;
   }

   bool CreativeTabs::hasScrollbar() const
   {
       return bhasScrollbar;
   }

   CreativeTabs &CreativeTabs::setNoScrollbar()
   {
       bhasScrollbar = false;
       return *this;
   }

   uint32_t CreativeTabs::getColumn() const
   {
       return index % 6;
   }

   bool CreativeTabs::isOnTopRow() const
   {
       return index < 6;
   }

   bool CreativeTabs::isAlignedRight() const
   {
       return getColumn() == 5;
   }

   std::vector<EnumEnchantmentType> CreativeTabs::getRelevantEnchantmentTypes() const
   {
       return enchantmentTypes;
   }

   CreativeTabs &CreativeTabs::setRelevantEnchantmentTypes(std::span<EnumEnchantmentType> types)
   {
       enchantmentTypes;
       return *this;
   }

   bool CreativeTabs::hasRelevantEnchantmentType(std::optional<EnumEnchantmentType> enchantmentType)
   {
       if (enchantmentType.has_value())
       {

           for (auto &enumenchantmenttype : enchantmentTypes)
           {
               if (enumenchantmenttype == enchantmentType)
               {
                   return true;
               }
           }
       }

       return false;
   }

   void CreativeTabs::displayAllRelevantItems(std::vector<ItemStack> &p_78018_1_) const
   {

       for (auto &[fst, snd] : Item::REGISTRY)
       {
           snd->getSubItems(*this, p_78018_1_);
       }
   }
