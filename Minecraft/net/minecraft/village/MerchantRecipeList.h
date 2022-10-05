#include "MerchantRecipe.h"

#include <vector>

class ItemStack;
class NBTTagCompound;

class MerchantRecipeList
{
  public:
    MerchantRecipeList() = default;
    explicit MerchantRecipeList(NBTTagCompound *compound);

    MerchantRecipe canRecipeBeUsed(ItemStack stack0, ItemStack stack1, int32_t index);
    void writeToBuf(PacketBuffer &buffer);
    void add(const MerchantRecipe &merchantrecipe);
    void readRecipiesFromTags(NBTTagCompound *compound);
    MerchantRecipeList readFromBuf(const PacketBuffer &buffer);
    NBTTagCompound *getRecipiesAsTags();

  private:
    bool areItemStacksExactlyEqual(ItemStack stack1, ItemStack stack2);

    std::vector<MerchantRecipe> list;
};
