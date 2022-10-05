#include <vector>

class Item;
class ItemStack;

class PotionHelper
{
  public:
    static bool isReagent(ItemStack stack);
    static ItemStack doReaction(ItemStack reagent, ItemStack potionIn);
    static void init();

    template <class T, class C, class Ingredient> class MixPredicate
    {
      public:
        T input;
        Ingredient reagent;
        C output;

        MixPredicate(T p_i47570_1_, Ingredient p_i47570_2_, C p_i47570_3_);
    };

  protected:
    static bool isItemConversionReagent(ItemStack stack);
    static bool isTypeConversionReagent(ItemStack stack);
    static bool hasConversions(ItemStack input, ItemStack reagent);
    static bool hasItemConversions(ItemStack input, ItemStack reagent);
    static bool hasTypeConversions(ItemStack input, ItemStack reagent);

  private:
    static void addContainerRecipe(ItemPotion p_193355_0_, Item p_193355_1_, ItemPotion p_193355_2_);
    static void addContainer(ItemPotion p_193354_0_);
    static void addMix(PotionType p_193357_0_, Item p_193357_1_, PotionType p_193357_2_);
    static void addMix(PotionType p_193356_0_, Ingredient p_193356_1_, PotionType p_193356_2_);

    static std::vector<> POTION_TYPE_CONVERSIONS;
    static std::vector<> POTION_ITEM_CONVERSIONS;
    static std::vector<> POTION_ITEMS;
};

template <class T, class C, class Ingredient>
PotionHelper::MixPredicate<T, C, Ingredient>::MixPredicate(T p_i47570_1_, Ingredient p_i47570_2_, C p_i47570_3_)
{
    input   = p_i47570_1_;
    reagent = p_i47570_2_;
    output  = p_i47570_3_;
}
