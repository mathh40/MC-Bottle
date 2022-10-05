#include <optional>
#include <vector>

class ServerRecipeBookHelper
{
  private:
    Logger field_194330_a;
    RecipeItemHelper field_194331_b = new RecipeItemHelper();
    EntityPlayerMP field_194332_c;
    IRecipe field_194333_d;
    boolean field_194334_e;
    InventoryCraftResult field_194335_f;
    InventoryCrafting field_194336_g;
    List field_194337_h;

  public:
    void func_194327_a(EntityPlayerMP p_194327_1_, std::optional<IRecipe> p_194327_2_, bool p_194327_3_);

  private:
    void func_194326_a();
    void func_194329_b();
    int func_194324_a(int p_194324_1_, bool p_194324_2_);
    void func_194323_a(int p_194323_1_, IntList p_194323_2_);
    void func_194325_a(Slot p_194325_1_, ItemStack p_194325_2_);
    bool func_194328_c();
};
