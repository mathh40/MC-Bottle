#pragma once
#include "Item.h"
#include "SoundEvent.h"

class ItemRecord : public Item
{
  public:
    EnumActionResult onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
                               float hitX, float hitY, float hitZ) override;
    void addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                        ITooltipFlag *flagIn) override;
    std::string getRecordNameLocal() const;
    EnumRarity getRarity(ItemStack stack) const override;
    static ItemRecord getBySound(SoundEvent soundIn);
    SoundEvent getSound() const;

  protected:
    ItemRecord(std::string_view recordName, SoundEvent soundIn);

  private:
    static std::unordered_map<SoundEvent, ItemRecord> RECORDS;
    SoundEvent sound;
    std::string displayName;
};
