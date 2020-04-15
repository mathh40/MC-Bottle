#pragma once
#include <crossguid/guid.hpp>
#include "ActionResult.h"
#include "EnumAction.h"
#include "EnumActionResult.h"
#include "EnumRarity.h"
#include "IItemPropertyGetter.h"
#include "ResourceLocation.h"
#include "../../../../pcg-cpp/pcg_random.hpp"
#include "math/RayTraceResult.h"
#include "registry/RegistryNamespaced.h"

class Entity;
class IBlockState;
class EntityPlayer;
class BlockPos;
enum class EnumHand;
class EnumFacing;
class NBTTagCompound;
class Block;

class Item
{
public:

    Item();
    Item* setMaxStackSize(int32_t maxStackSize);
    virtual EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ);
    virtual float getDestroySpeed(ItemStack stack, IBlockState* state);
    virtual ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn);
    virtual ItemStack onItemUseFinish(ItemStack stack, World* worldIn, EntityLivingBase* entityLiving);
    int32_t getItemStackLimit() const;
    virtual int32_t getMetadata(int32_t damage) const;
    bool getHasSubtypes() const;
    int32_t getMaxDamage() const;
    bool isDamageable() const;
    virtual bool hitEntity(ItemStack stack, EntityLivingBase* target, EntityLivingBase* attacker);
    virtual bool onBlockDestroyed(ItemStack stack, World* worldIn, IBlockState* state, BlockPos pos, EntityLivingBase* entityLiving);
    bool canHarvestBlock(IBlockState* blockIn);
    bool itemInteractionForEntity(ItemStack stack, EntityPlayer* playerIn, EntityLivingBase* target, EnumHand hand);
    Item* setFull3D();
    virtual bool isFull3D() const;
    virtual bool shouldRotateAroundWhenRendering();
    Item* setTranslationKey(std::string_view key);
    std::string getUnlocalizedNameInefficiently(ItemStack stack) const;
    virtual std::string getTranslationKey() const;
    virtual std::string getTranslationKey(ItemStack stack) const;
    Item* setContainerItem(Item* containerItem);
    bool getShareTag();
    Item* getContainerItem() const;
    bool hasContainerItem() const;
    void onUpdate(ItemStack stack, World* worldIn, Entity* entityIn, int32_t itemSlot, bool isSelected);
    void onCreated(ItemStack stack, World* worldIn, EntityPlayer* playerIn);
    bool isMap();
    virtual EnumAction getItemUseAction(ItemStack stack);
    virtual int32_t getMaxItemUseDuration(ItemStack stack);
    virtual void onPlayerStoppedUsing(ItemStack stack, World* worldIn, EntityLivingBase* entityLiving, int32_t timeLeft);
    virtual void addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn);
    virtual std::string getItemStackDisplayName(ItemStack stack) const;
    virtual bool hasEffect(ItemStack stack);
    virtual EnumRarity getRarity(ItemStack stack);
    virtual bool isEnchantable(ItemStack stack) const;
    static int32_t getIdFromItem(Item *itemIn);
    static Item* getItemById(int32_t id);
    static Item* getItemFromBlock(Block* blockIn);
    static Item* getByNameOrId(std::string_view id);
    void addPropertyOverride(ResourceLocation key, IItemPropertyGetter getter);
    IItemPropertyGetter getPropertyGetter(ResourceLocation key);
    bool hasCustomProperties();
    bool updateItemStackNBT(NBTTagCompound* nbt);
    virtual int32_t getItemEnchantability();
    virtual void getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items);
    virtual CreativeTabs& getCreativeTab();
    Item* setCreativeTab(CreativeTabs& tab);
    bool canItemEditBlocks();
    virtual bool getIsRepairable(ItemStack toRepair, ItemStack repair);
    virtual std::unordered_multimap<std::string,AttributeModifier> getItemAttributeModifiers(EntityEquipmentSlot equipmentSlot);
    ItemStack getDefaultInstance();

    static void registerItems();

    static RegistryNamespaced<ResourceLocation,Item *> REGISTRY;

protected:
    Item* setHasSubtypes(bool hasSubtypes);
    Item* setMaxDamage(int32_t maxDamageIn);
    RayTraceResult rayTrace(World* worldIn, EntityPlayer* playerIn, bool useLiquids);
    bool isInCreativeTab(const CreativeTabs& targetTab);
    static void registerItemBlock(Block* blockIn, Item* itemIn);



    static xg::Guid ATTACK_DAMAGE_MODIFIER;
    static xg::Guid ATTACK_SPEED_MODIFIER;
    static pcg32 itemRand;
    int32_t maxStackSize = 64;
    bool bFull3D;
    bool hasSubtypes;
private:
    static void registerItemBlock(Block* blockIn);
    static void registerItem(int32_t id, std::string textualID, Item* itemIn);
    static void registerItem(int32_t id, ResourceLocation textualID, Item* itemIn);



    static std::unordered_map<Block*,Item*> BLOCK_TO_ITEM;
    static IItemPropertyGetter DAMAGED_GETTER;
    static IItemPropertyGetter DAMAGE_GETTER;
    static IItemPropertyGetter LEFTHANDED_GETTER;
    static IItemPropertyGetter COOLDOWN_GETTER;
    IRegistry* properties;
    CreativeTabs tabToDisplayOn;
    int32_t maxDamage;
    Item* containerItem;
    std::string translationKey;
};
