#pragma once
#include "../enchantment/EnumEnchantmentType.h"
#include "../item/ItemStack.h"
#include <array>
#include <string>

class CreativeTabs
{
  public:
    virtual ~CreativeTabs() = default;
    static std::array<CreativeTabs*,12> CREATIVE_TAB_ARRAY;
    static CreativeTabs* TOOLS;
    static CreativeTabs* COMBAT;
    static CreativeTabs* BREWING;
    static CreativeTabs* MATERIALS;
    static CreativeTabs* HOTBAR;
    static CreativeTabs* INVENTORY;
    std::string tabLabel;
    std::string backgroundTexture = "items.png";

    CreativeTabs(uint32_t indexIn, std::string_view label);
    uint32_t getIndex() const;
    std::string getTabLabel() const;
    std::string getTranslationKey() const;
    ItemStack getIcon();
    virtual ItemStack createIcon() = 0;
    std::string getBackgroundImageName() const;
    CreativeTabs &setBackgroundImageName(std::string_view texture);
    bool drawInForegroundOfTab() const;
    CreativeTabs &setNoTitle();
    bool hasScrollbar() const;
    CreativeTabs &setNoScrollbar();
    uint32_t getColumn() const;
    bool isOnTopRow() const;
    bool isAlignedRight() const;
    std::vector<EnumEnchantmentType> getRelevantEnchantmentTypes() const;
    CreativeTabs &setRelevantEnchantmentTypes(std::span<EnumEnchantmentType> types);
    bool hasRelevantEnchantmentType(std::optional<EnumEnchantmentType> enchantmentType);
    void displayAllRelevantItems(std::vector<ItemStack> &p_78018_1_) const;

private:
   uint32_t index;
   bool bhasScrollbar = true;
   bool drawTitle = true;
   std::vector<EnumEnchantmentType> enchantmentTypes;
   ItemStack icon;
};


