#pragma once

#include <cstdint>
#include "ActionResult.h"
#include "EnumAction.h"
#include "EnumActionResult.h"
#include "EnumRarity.h"
#include "NBTTagCompound.h"
#include "../block/Block.h"
#include "text/ITextComponent.h"

class EntityPlayerMP;
class DataFixer;

class ItemStack
{
public:
    ItemStack(Block* blockIn);
    ItemStack(Block* blockIn, int32_t amount);
    ItemStack(Block blockIn, int32_t amount, int32_t meta);
    ItemStack(Item* itemIn);
    ItemStack(Item* itemIn, int32_t amount);
    ItemStack(Item* itemIn, int32_t amount, int32_t meta);

    ItemStack(NBTTagCompound* compound);
    bool isEmpty();
    static void registerFixes(DataFixer fixer);
    ItemStack splitStack(int32_t amount);
    Item* getItem();
    EnumActionResult onItemUse(EntityPlayer* playerIn, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing side, float hitX, float hitY, float hitZ);
    float getDestroySpeed(IBlockState* blockIn);
    ActionResult useItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand hand);
    ItemStack onItemUseFinish(World* worldIn, EntityLivingBase* entityLiving);
    NBTTagCompound* writeToNBT(NBTTagCompound* nbt);
    int32_t getMaxStackSize();
    bool isStackable();
    bool isItemStackDamageable();
    bool getHasSubtypes();
    bool isItemDamaged();
    int32_t getItemDamage() const;
    int32_t getMetadata() const;
    void setItemDamage(int32_t meta);
    int32_t getMaxDamage();
    bool attemptDamageItem(int32_t amount, pcg32& rand, EntityPlayerMP* damager);
    void damageItem(int32_t amount, EntityLivingBase* entityIn);
    void hitEntity(EntityLivingBase* entityIn, EntityPlayer* playerIn);
    void onBlockDestroyed(World* worldIn, IBlockState* blockIn, BlockPos pos, EntityPlayer* playerIn);
    bool canHarvestBlock(IBlockState* blockIn);
    bool interactWithEntity(EntityPlayer* playerIn, EntityLivingBase* entityIn, EnumHand hand);
    ItemStack copy();
    static bool areItemStackTagsEqual(ItemStack stackA, ItemStack stackB);
    static bool areItemStacksEqual(ItemStack stackA, ItemStack stackB);
    static bool areItemsEqual(ItemStack stackA, ItemStack stackB);
    static bool areItemsEqualIgnoreDurability(ItemStack stackA, ItemStack stackB);
    bool isItemEqual(ItemStack other) const;
    bool isItemEqualIgnoreDurability(ItemStack stack);
    std::string getTranslationKey();
    std::string toString();
    void updateAnimation(World* worldIn, Entity* entityIn, int32_t inventorySlot, bool isCurrentItem);
    void onCrafting(World* worldIn, EntityPlayer* playerIn, int32_t amount);
    int32_t getMaxItemUseDuration();
    EnumAction getItemUseAction();
    void onPlayerStoppedUsing(World* worldIn, EntityLivingBase* entityLiving, int32_t timeLeft);
    bool hasTagCompound() const;
    NBTTagCompound* getTagCompound() const;
    NBTTagCompound* getOrCreateSubCompound(std::string_view key);
    NBTTagCompound* getSubCompound(std::string_view key) const;
    void removeSubCompound(std::string_view key) const;
    NBTTagList* getEnchantmentTagList() const;
    void setTagCompound(NBTTagCompound* nbt);
    std::string getDisplayName();
    ItemStack setTranslatableName(std::string_view p_190924_1_);
    ItemStack setStackDisplayName(std::string_view displayName);
    void clearCustomName();
    bool hasDisplayName() const;
    std::vector<std::string> getTooltip(EntityPlayer* playerIn, ITooltipFlag* advanced);
    bool hasEffect();
    EnumRarity getRarity();
    bool isItemEnchantable();
    void addEnchantment(Enchantment ench, int32_t level);
    bool isItemEnchanted() const;
    void setTagInfo(std::string_view key, NBTBase* value);
    bool canEditBlocks();
    bool isOnItemFrame();
    void setItemFrame(EntityItemFrame* frame);
    EntityItemFrame* getItemFrame();
    int32_t getRepairCost() const;
    void setRepairCost(int32_t cost);
    std::unordered_multimap<std::string,AttributeModifier> getAttributeModifiers(EntityEquipmentSlot equipmentSlot);
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


    static ItemStack EMPTY = ItemStack(nullptr);
    static DecimalFormat DECIMALFORMAT = DecimalFormat("#.##");
private:
    void updateEmptyState();
    bool isItemStackEqual(ItemStack other);

    int32_t stackSize;
    int32_t animationsToGo;
    Item* item;
    NBTTagCompound* stackTagCompound;
    bool bisEmpty;
    int32_t itemDamage;
    EntityItemFrame* itemFrame;
    Block* canDestroyCacheBlock;
    bool canDestroyCacheResult;
    Block* canPlaceOnCacheBlock;
    bool canPlaceOnCacheResult;
};
