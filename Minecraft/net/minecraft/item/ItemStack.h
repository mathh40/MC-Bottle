#pragma once

#include "../block/Block.h"
#include "EnumAction.h"
#include "EnumActionResult.h"
#include "EnumRarity.h"
#include "ItemCompass.h"
#include "text/ITextComponent.h"
#include <cstdint>

class EntityPlayerMP;
class DataFixer;
class NBTTagList;
class NBTBase;
class Enchantment;

class ItemStack
{
public:
    ItemStack(const Block* blockIn);
    ItemStack(const Block* blockIn, int32_t amount);
    ItemStack(const Block* blockIn, int32_t amount, int32_t meta);
    ItemStack(const Item* itemIn);
    ItemStack(const Item* itemIn, int32_t amount);
    ItemStack(const Item* itemIn, int32_t amount, int32_t meta);

    ItemStack(NBTTagCompound* compound);
    bool isEmpty() const;
    static void registerFixes(DataFixer fixer);
    ItemStack splitStack(int32_t amount) const;
    Item* getItem() const;
    EnumActionResult onItemUse(EntityPlayer* playerIn, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing side, float hitX, float hitY, float hitZ);
    float getDestroySpeed(IBlockState* blockIn) const;
    ActionResult useItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand hand) const;
    ItemStack onItemUseFinish(World* worldIn, EntityLivingBase* entityLiving) const;
    NBTTagCompound* writeToNBT(NBTTagCompound* nbt);
    int32_t getMaxStackSize() const;
    bool isStackable() const;
    bool isItemStackDamageable() const;
    bool getHasSubtypes() const;
    bool isItemDamaged() const;
    int32_t getItemDamage() const;
    int32_t getMetadata() const;
    void setItemDamage(int32_t meta);
    int32_t getMaxDamage() const;
    bool attemptDamageItem(int32_t amount, pcg32& rand, EntityPlayerMP* damager);
    void damageItem(int32_t amount, EntityLivingBase* entityIn);
    void hitEntity(EntityLivingBase* entityIn, EntityPlayer* playerIn);
    void onBlockDestroyed(World* worldIn, IBlockState* blockIn, BlockPos pos, EntityPlayer* playerIn);
    bool canHarvestBlock(IBlockState* blockIn) const;
    bool interactWithEntity(EntityPlayer* playerIn, EntityLivingBase* entityIn, EnumHand hand) const;
    ItemStack copy() const;
    static bool areItemStackTagsEqual(ItemStack stackA, ItemStack stackB);
    static bool areItemStacksEqual(ItemStack stackA, ItemStack stackB);
    static bool areItemsEqual(ItemStack stackA, ItemStack stackB);
    static bool areItemsEqualIgnoreDurability(ItemStack stackA, ItemStack stackB);
    bool isItemEqual(ItemStack other) const;
    bool isItemEqualIgnoreDurability(ItemStack stack) const;
    std::string getTranslationKey() const;
    std::string toString() const;
    void updateAnimation(World* worldIn, Entity* entityIn, int32_t inventorySlot, bool isCurrentItem);
    void onCrafting(World* worldIn, EntityPlayer* playerIn, int32_t amount);
    int32_t getMaxItemUseDuration() const;
    EnumAction getItemUseAction() const;
    void onPlayerStoppedUsing(World* worldIn, EntityLivingBase* entityLiving, int32_t timeLeft) const;
    bool hasTagCompound() const;
    NBTTagCompound* getTagCompound() const;
    NBTTagCompound* getOrCreateSubCompound(std::string_view key);
    NBTTagCompound* getSubCompound(std::string_view key) const;
    void removeSubCompound(std::string_view key) const;
    NBTTagList* getEnchantmentTagList() const;
    void setTagCompound(NBTTagCompound* nbt);
    std::string getDisplayName() const;
    ItemStack setTranslatableName(std::string_view p_190924_1_);
    ItemStack setStackDisplayName(std::string_view displayName);
    void clearCustomName();
    bool hasDisplayName() const;
    std::vector<std::string> getTooltip(EntityPlayer* playerIn, ITooltipFlag* advanced);
    bool hasEffect() const;
    EnumRarity getRarity() const;
    bool isItemEnchantable() const;
    void addEnchantment(const Enchantment& ench, int32_t level);
    bool isItemEnchanted() const;
    void setTagInfo(std::string_view key, NBTBase* value);
    bool canEditBlocks() const;
    bool isOnItemFrame() const;
    void setItemFrame(EntityItemFrame* frame);
    EntityItemFrame* getItemFrame() const;
    int32_t getRepairCost() const;
    void setRepairCost(int32_t cost);
    std::unordered_multimap<std::string,AttributeModifier> getAttributeModifiers(EntityEquipmentSlot equipmentSlot) const;
    void addAttributeModifier(std::string_view attributeName, AttributeModifier modifier, EntityEquipmentSlot* equipmentSlot);
    ITextComponent* getTextComponent();
    bool canDestroy(Block* blockIn);
    bool canPlaceOn(Block* blockIn);
    int32_t getAnimationsToGo() const;
    void setAnimationsToGo(int32_t animations);
    int32_t getCount() const;
    void setCount(int32_t size);
    void grow(int32_t quantity);
    void shrink(int32_t quantity);


    static ItemStack EMPTY;
private:
    void updateEmptyState();
    bool isItemStackEqual(ItemStack other) const;

    int32_t stackSize;
    int32_t animationsToGo;
    const Item* item;
    NBTTagCompound* stackTagCompound;
    bool bisEmpty;
    int32_t itemDamage;
    EntityItemFrame* itemFrame;
    const Block* canDestroyCacheBlock;
    bool canDestroyCacheResult;
    const Block* canPlaceOnCacheBlock;
    bool canPlaceOnCacheResult;
};
