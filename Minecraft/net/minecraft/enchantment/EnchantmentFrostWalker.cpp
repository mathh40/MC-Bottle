#include "EnchantmentFrostWalker.h"

#include "../entity/EntityLivingBase.h"

EnchantmentFrostWalker::EnchantmentFrostWalker(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot>slots):
    Enchantment(rarityIn, EnumEnchantmentType::ARMOR_FEET, slots) {
    setName("frostWalker");
}

int32_t EnchantmentFrostWalker::getMinEnchantability(int32_t enchantmentLevel) const {
    return enchantmentLevel * 10;
}

int32_t EnchantmentFrostWalker::getMaxEnchantability(int32_t enchantmentLevel) const {
    return getMinEnchantability(enchantmentLevel) + 15;
}

bool EnchantmentFrostWalker::isTreasureEnchantment() const {
    return true;
}

int32_t EnchantmentFrostWalker::getMaxLevel() const {
    return 2;
}

void EnchantmentFrostWalker::
freezeNearby(EntityLivingBase *living, World *worldIn, const BlockPos &pos, int32_t level) {
    if (living->onGround) {
        float f = MathHelper::min(16, 2 + level);
        BlockPos block_pos = BlockPos(0, 0, 0);
        auto var6 = getAllInBoxMutable(pos.add((double)(-f), -1.0, (double)(-f)),
                                                    pos.add((double)f, -1.0, (double)f));
        for(auto block_pos1 : var6) {
            if (block_pos1.distanceSqToCenter(living->posX, living->posY, living->posZ) <= (double)(f * f
                )) {
                block_pos.setPos(block_pos1.getx(), block_pos1.gety() + 1,
                                                block_pos1.getz());
                IBlockState* iblockstate = worldIn->getBlockState(block_pos);
                if (iblockstate->getMaterial() == Material::AIR) {
                    IBlockState* iblockstate1 = worldIn->getBlockState(block_pos1);
                    if (iblockstate1->getMaterial() == Material::WATER && iblockstate1->
                        getValue(BlockLiquid::LEVEL) == 0 && worldIn->mayPlace(
                            Blocks::FROSTED_ICE, block_pos1, false, EnumFacing::DOWN, nullptr)) {
                        worldIn->setBlockState(block_pos1, Blocks::FROSTED_ICE.getDefaultState());
                        worldIn->scheduleUpdate(block_pos1, Blocks::FROSTED_ICE,
                                               MathHelper::getInt(living->getRNG(), 60, 120));
                    }
                }
            }
        }
    }

}

bool EnchantmentFrostWalker::canApplyTogether(const Enchantment &ench) const {
    return Enchantment::canApplyTogether(ench) && ench != Enchantments::DEPTH_STRIDER;
}
