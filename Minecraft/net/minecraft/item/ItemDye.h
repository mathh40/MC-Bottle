#pragma once
#include "Item.h"

class ItemDye : public Item
{
  public:
    static constexpr std::array<int32_t, 16> DYE_COLORS = {1973019, 11743532, 3887386,  5320730,  2437522, 8073150,
                                                           2651799, 11250603, 4408131,  14188952, 4312372, 14602026,
                                                           6719955, 12801229, 15435844, 15790320};
    ItemDye();
    std::string getTranslationKey(ItemStack stack) const override;
    EnumActionResult onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
                               float hitX, float hitY, float hitZ) override;
    bool applyBonemeal(ItemStack stack, World *worldIn, BlockPos target);
    void spawnBonemealParticles(World *worldIn, BlockPos pos, int32_t amount);
    bool itemInteractionForEntity(ItemStack stack, EntityPlayer *playerIn, EntityLivingBase *target,
                                  EnumHand hand) override;
    void getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items) override;

  private:
};
