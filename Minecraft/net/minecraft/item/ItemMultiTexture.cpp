#include "ItemMultiTexture.h"

#include "ItemStack.h"

ItemMultiTexture::ItemMultiTexture(Block* p_i47262_1_, Block* p_i47262_2_, ItemMultiTexture::Mapper p_i47262_3_)
    :ItemBlock(p_i47262_1_),unused(p_i47262_2_),nameFunction(p_i47262_3_),setMaxDamage(0),setHasSubtypes(true)
{   
}

ItemMultiTexture::ItemMultiTexture(Block *block, Block *block2, std::vector<std::string> namesByMeta)
    :ItemMultiTexture(block,block2,[=](ItemStack p_apply_1_)->std::string
    {
        auto i = p_apply_1_.getMetadata();
        if (i < 0 || i >= namesByMeta.size()) 
        {
            i = 0;
        }

        return namesByMeta[i];
    })
{
    
}

int32_t ItemMultiTexture::getMetadata(int32_t damage) const
{
    return damage;
}

std::string ItemMultiTexture::getTranslationKey(ItemStack stack) const
{
    return ItemBlock::getTranslationKey() + "." + nameFunction(stack);
}
