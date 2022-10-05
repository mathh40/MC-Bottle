#include "ItemRecord.h"

#include "../stats/StatList.h"
#include "ItemStack.h"
#include "World.h"
#include "text/translation/I18n.h"

EnumActionResult ItemRecord::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                       EnumFacing facing, float hitX, float hitY, float hitZ)
{
    IBlockState *iblockstate = worldIn->getBlockState(pos);
    if (iblockstate->getBlock() == Blocks.JUKEBOX && !iblockstate->getValue(BlockJukebox::HAS_RECORD))
    {
        if (!worldIn->isRemote)
        {
            ItemStack itemstack = player->getHeldItem(hand);
            ((BlockJukebox)Blocks.JUKEBOX).insertRecord(worldIn, pos, iblockstate, itemstack);
            worldIn->playEvent(nullptr, 1010, pos, Item::getIdFromItem(this));
            itemstack.shrink(1);
            player->addStat(StatList::RECORD_PLAYED);
        }

        return EnumActionResult::SUCCESS;
    }
    else
    {
        return EnumActionResult::PASS;
    }
}

ItemRecord::ItemRecord(std::string_view recordName, SoundEvent soundIn)
    : displayName("item.record." + recordName + ".desc"), sound(soundIn)
{
    maxStackSize = 1;
    setCreativeTab(CreativeTabs::MISC);
    RECORDS.emplace(sound, this);
}

void ItemRecord::addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                                ITooltipFlag *flagIn)
{
    tooltip.emplace_back(getRecordNameLocal());
}

std::string ItemRecord::getRecordNameLocal() const
{
    return I18n::translateToLocal(displayName);
}

EnumRarity ItemRecord::getRarity(ItemStack stack) const
{
    return EnumRarity::RARE;
}

ItemRecord ItemRecord::getBySound(SoundEvent soundIn)
{
    return RECORDS[soundIn];
}

SoundEvent ItemRecord::getSound() const
{
    return sound;
}
