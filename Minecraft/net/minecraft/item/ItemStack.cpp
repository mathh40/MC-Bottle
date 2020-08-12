#include "ItemStack.h"



#include "Item.h"
#include "Util.h"
#include "../stats/StatList.h"
#include "datafix/DataFixer.h"
#include "datafix/FixTypes.h"
#include "math/MathHelper.h"
#include "text/TextComponentString.h"
#include "text/event/HoverEvent.h"
#include "text/translation/I18n.h"

ItemStack ItemStack::EMPTY = ItemStack(nullptr);

ItemStack::ItemStack(const Block *blockIn)
    :ItemStack(blockIn,1)
{
}

ItemStack::ItemStack(const Block *blockIn, int32_t amount)
    :ItemStack(blockIn, amount, 0)
{
}

ItemStack::ItemStack(const Block blockIn, int32_t amount, int32_t meta)
    :ItemStack(Item::getItemFromBlock(blockIn), amount, meta)
{
}

ItemStack::ItemStack(const Item *itemIn)
    :ItemStack(itemIn, 1)
{
}

ItemStack::ItemStack(const Item *itemIn, int32_t amount)
    :ItemStack(itemIn, amount, 0)
{
}

ItemStack::ItemStack(const Item *itemIn, int32_t amount, int32_t meta)
    :item(itemIn),itemDamage(meta),stackSize(amount)
{
    if (itemDamage < 0) 
    {
        itemDamage = 0;
    }

    updateEmptyState();
}

ItemStack::ItemStack(NBTTagCompound *compound)
{
    item = Item::getByNameOrId(compound->getString("id"));
    stackSize = compound->getByte("Count");
    itemDamage = MathHelper::max<int8_t>(0, compound->getShort("Damage"));
    if (compound->hasKey("tag", 10)) 
    {
        stackTagCompound = compound->getCompoundTag("tag");
    if (item != nullptr) 
    {
        item->updateItemStackNBT(compound);
    }
    }

    updateEmptyState();
}

bool ItemStack::isEmpty() const
{
    if (*this == EMPTY) 
    {
        return true;
    }
    else if (item != nullptr && item != Item::getItemFromBlock(Blocks::AIR)) 
    {
        if (stackSize <= 0) 
        {
            return true;
        }
        else 
        {
            return itemDamage < -32768 || itemDamage > 65535;
        }
    }
    else 
    {
        return true;
    }
}

void ItemStack::registerFixes(DataFixer fixer)
{
    fixer.registerWalker(FixTypes::ITEM_INSTANCE, BlockEntityTag());
    fixer.registerWalker(FixTypes::ITEM_INSTANCE, EntityTag());
}

ItemStack ItemStack::splitStack(int32_t amount)
{
    int32_t i = MathHelper::min(amount, stackSize);
    ItemStack itemstack = copy();
    itemstack.setCount(i);
    shrink(i);
    return itemstack;
}

Item * ItemStack::getItem() const
{
    return isEmpty ? Item::getItemFromBlock(Blocks::AIR) : item;
}

EnumActionResult ItemStack::onItemUse(EntityPlayer *playerIn, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing side, float hitX, float hitY, float hitZ)
{
    EnumActionResult enumactionresult = getItem()->onItemUse(playerIn, worldIn, pos, hand, side, hitX, hitY, hitZ);
    if (enumactionresult == EnumActionResult::SUCCESS) 
    {
        playerIn->addStat(StatList::getObjectUseStats(item));
    }

    return enumactionresult;
}

float ItemStack::getDestroySpeed(IBlockState *blockIn)
{
    return getItem()->getDestroySpeed(*this, blockIn);
}

ActionResult ItemStack::useItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand hand)
{
    return getItem()->onItemRightClick(worldIn, playerIn, hand);
}

ItemStack ItemStack::onItemUseFinish(World *worldIn, EntityLivingBase *entityLiving)
{
    return getItem()->onItemUseFinish(*this, worldIn, entityLiving);
}

NBTTagCompound * ItemStack::writeToNBT(NBTTagCompound *nbt)
{
    auto resourcelocation = Item::REGISTRY.getNameForObject(item);
    nbt->setString("id", !resourcelocation.has_value() ? "minecraft:air" : resourcelocation->to_string());
    nbt->setByte("Count", stackSize);
    nbt->setShort("Damage", itemDamage);
    if (stackTagCompound != nullptr) 
    {
        nbt->setTag("tag", stackTagCompound);
    }

    return nbt;
}

int32_t ItemStack::getMaxStackSize()
{
    return getItem()->getItemStackLimit();
}

bool ItemStack::isStackable()
{
    return getMaxStackSize() > 1 && (!isItemStackDamageable() || !isItemDamaged());
}

bool ItemStack::isItemStackDamageable()
{
    if (isEmpty) 
    {
        return false;
    }
    else if (item->getMaxDamage() <= 0) 
    {
        return false;
    }
    else 
    {
        return !hasTagCompound() || !getTagCompound()->getBoolean("Unbreakable");
    }
}

bool ItemStack::getHasSubtypes()
{
    return getItem()->getHasSubtypes();
}

bool ItemStack::isItemDamaged()
{
    return isItemStackDamageable() && itemDamage > 0;
}

int32_t ItemStack::getItemDamage() const
{
    return itemDamage;
}

int32_t ItemStack::getMetadata() const
{
    return itemDamage;
}

void ItemStack::setItemDamage(int32_t meta)
{
    itemDamage = meta;
    if (itemDamage < 0) 
    {
        itemDamage = 0;
    }
}

int32_t ItemStack::getMaxDamage()
{
    return getItem()->getMaxDamage();
}

bool ItemStack::attemptDamageItem(int32_t amount, pcg32 &rand, EntityPlayerMP *damager)
{
    if (!isItemStackDamageable()) 
    {
        return false;
    }
    else 
    {
        if (amount > 0) 
        {
            int32_t i = EnchantmentHelper.getEnchantmentLevel(Enchantments::UNBREAKING, this);
            int32_t j = 0;

            for(int32_t k = 0; i > 0 && k < amount; ++k) 
            {
                if (EnchantmentDurability.negateDamage(this, i, rand)) 
                {
                    ++j;
                }
            }

            amount -= j;
            if (amount <= 0) 
            {
                return false;
            }
        }

        if (damager != nullptr && amount != 0) 
        {
            CriteriaTriggers.ITEM_DURABILITY_CHANGED.trigger(damager, this, itemDamage + amount);
        }

        itemDamage += amount;
        return itemDamage > getMaxDamage();
    }
}

void ItemStack::damageItem(int32_t amount, EntityLivingBase *entityIn)
{
    if ((!(Util::instanceof<EntityPlayer>(entityIn)) || !((EntityPlayer*)entityIn)->capabilities.isCreativeMode) && isItemStackDamageable() && attemptDamageItem(amount, entityIn->getRNG(), Util::instanceof<EntityPlayerMP>(entityIn) ? (EntityPlayerMP*)entityIn : nullptr)) 
    {
        entityIn->renderBrokenItemStack(this);
        shrink(1);
        if (Util::instanceof<EntityPlayer>(entityIn)) 
        {
            EntityPlayer* entityplayer = (EntityPlayer*)entityIn;
            entityplayer->addStat(StatList::getObjectBreakStats(item));
        }

        itemDamage = 0;
    }
}

void ItemStack::hitEntity(EntityLivingBase *entityIn, EntityPlayer *playerIn)
{
    bool flag = item->hitEntity(*this, entityIn, playerIn);
    if (flag) 
    {
        playerIn->addStat(StatList::getObjectUseStats(item));
    }
}

void ItemStack::onBlockDestroyed(World *worldIn, IBlockState *blockIn, BlockPos pos, EntityPlayer *playerIn)
{
    bool flag = getItem()->onBlockDestroyed(*this, worldIn, blockIn, pos, playerIn);
    if (flag) 
    {
        playerIn->addStat(StatList::getObjectUseStats(item));
    }
}

bool ItemStack::canHarvestBlock(IBlockState *blockIn)
{
    return getItem()->canHarvestBlock(blockIn);
}

bool ItemStack::interactWithEntity(EntityPlayer *playerIn, EntityLivingBase *entityIn, EnumHand hand)
{
    return getItem()->itemInteractionForEntity(*this, playerIn, entityIn, hand);
}

ItemStack ItemStack::copy()
{
    ItemStack itemstack = ItemStack(item, stackSize, itemDamage);
    itemstack.setAnimationsToGo(getAnimationsToGo());
    if (stackTagCompound != nullptr) 
    {
        itemstack.stackTagCompound = stackTagCompound;
    }

    return itemstack;
}

bool ItemStack::areItemStackTagsEqual(ItemStack stackA, ItemStack stackB)
{
    if (stackA.isEmpty() && stackB.isEmpty()) 
    {
        return true;
    }
    else if (!stackA.isEmpty() && !stackB.isEmpty()) 
    {
        if (stackA.stackTagCompound == nullptr && stackB.stackTagCompound != nullptr) 
        {
            return false;
        }
        else 
        {
            return stackA.stackTagCompound == nullptr || stackA.stackTagCompound == stackB.stackTagCompound;
        }
    }
    else 
    {
        return false;
    }
}

bool ItemStack::areItemStacksEqual(ItemStack stackA, ItemStack stackB)
{
    if (stackA.isEmpty() && stackB.isEmpty()) 
    {
        return true;
    }
    else 
    {
        return !stackA.isEmpty() && !stackB.isEmpty() ? stackA.isItemStackEqual(stackB) : false;
    }
}

bool ItemStack::areItemsEqual(ItemStack stackA, ItemStack stackB)
{
    if (stackA == stackB) 
    {
        return true;
    }
    else 
    {
        return !stackA.isEmpty() && !stackB.isEmpty() ? stackA.isItemEqual(stackB) : false;
    }
}

bool ItemStack::areItemsEqualIgnoreDurability(ItemStack stackA, ItemStack stackB)
{
    if (stackA == stackB) 
    {
        return true;
    }
    else 
    {
        return !stackA.isEmpty() && !stackB.isEmpty() ? stackA.isItemEqualIgnoreDurability(stackB) : false;
    }
}

bool ItemStack::isItemEqual(ItemStack other) const
{
    return !other.isEmpty() && item == other.item && itemDamage == other.itemDamage;
}

bool ItemStack::isItemEqualIgnoreDurability(ItemStack stack)
{
    if (!isItemStackDamageable()) 
    {
        return isItemEqual(stack);
    }
    else 
    {
        return !stack.isEmpty() && item == stack.item;
    }
}

std::string ItemStack::getTranslationKey()
{
    return getItem()->getTranslationKey(*this);
}

std::string ItemStack::toString()
{
    return stackSize + "x" + getItem()->getTranslationKey() + "@" + std::to_string(itemDamage);
}

void ItemStack::updateAnimation(World *worldIn, Entity *entityIn, int32_t inventorySlot, bool isCurrentItem)
{
    if (animationsToGo > 0) 
    {
        --animationsToGo;
    }

    if (item != nullptr) 
    {
        item->onUpdate(*this, worldIn, entityIn, inventorySlot, isCurrentItem);
    }
}

void ItemStack::onCrafting(World *worldIn, EntityPlayer *playerIn, int32_t amount)
{
    playerIn->addStat(StatList::getCraftStats(item), amount);
    getItem()->onCreated(*this, worldIn, playerIn);
}

int32_t ItemStack::getMaxItemUseDuration()
{
    return getItem()->getMaxItemUseDuration(*this);
}

void ItemStack::onPlayerStoppedUsing(World *worldIn, EntityLivingBase *entityLiving, int32_t timeLeft)
{
    getItem()->onPlayerStoppedUsing(*this, worldIn, entityLiving, timeLeft);
}

bool ItemStack::hasTagCompound() const
{
    return !isEmpty && stackTagCompound != nullptr;
}

NBTTagCompound * ItemStack::getTagCompound() const
{
    return stackTagCompound;
}

NBTTagCompound * ItemStack::getOrCreateSubCompound(std::string_view key)
{
    if (stackTagCompound != nullptr && stackTagCompound->hasKey(key, 10)) 
    {
        return stackTagCompound->getCompoundTag(key);
    }
    else 
    {
        NBTTagCompound* nbttagcompound = new NBTTagCompound();
        setTagInfo(key, nbttagcompound);
        return nbttagcompound;
    }
}

NBTTagCompound * ItemStack::getSubCompound(std::string_view key) const
{
    return stackTagCompound != nullptr && stackTagCompound->hasKey(key, 10) ? stackTagCompound->getCompoundTag(key) : nullptr;
}

void ItemStack::removeSubCompound(std::string_view key) const
{
    if (stackTagCompound != nullptr && stackTagCompound->hasKey(key, 10)) 
    {
        stackTagCompound->removeTag(key);
    }
}

NBTTagList * ItemStack::getEnchantmentTagList() const
{
    return stackTagCompound != nullptr ? stackTagCompound->getTagList("ench", 10) : new NBTTagList();
}

void ItemStack::setTagCompound(NBTTagCompound *nbt)
{
    stackTagCompound = nbt;
}

std::string ItemStack::getDisplayName()
{
    NBTTagCompound* nbttagcompound = getSubCompound("display");
    if (nbttagcompound != nullptr) 
    {
        if (nbttagcompound->hasKey("Name", 8)) 
        {
            return nbttagcompound->getString("Name");
        }

        if (nbttagcompound->hasKey("LocName", 8)) 
        {
            return I18n::translateToLocal(nbttagcompound->getString("LocName"));
        }
    }

    return getItem()->getItemStackDisplayName(*this);
}

ItemStack ItemStack::setTranslatableName(std::string_view p_190924_1_)
{
    getOrCreateSubCompound("display")->setString("LocName", p_190924_1_);
    return *this;
}

ItemStack ItemStack::setStackDisplayName(std::string_view displayName)
{
    getOrCreateSubCompound("display")->setString("Name", displayName);
    return *this;
}

void ItemStack::clearCustomName()
{
    NBTTagCompound* nbttagcompound = getSubCompound("display");
    if (nbttagcompound != nullptr) 
    {
        nbttagcompound->removeTag("Name");
        if (nbttagcompound->isEmpty()) 
        {
            removeSubCompound("display");
        }
    }

    if (stackTagCompound != nullptr && stackTagCompound->isEmpty()) 
    {
        stackTagCompound = nullptr;
    }
}

bool ItemStack::hasDisplayName() const
{
    NBTTagCompound* nbttagcompound = getSubCompound("display");
    return nbttagcompound != nullptr && nbttagcompound->hasKey("Name", 8);
}

std::vector<std::string> ItemStack::getTooltip(EntityPlayer* playerIn, ITooltipFlag* advanced)
{
    std::vector<std::string> list;
    auto s = getDisplayName();
    if (hasDisplayName()) 
    {
        s = TextFormatting::ITALIC + s;
    }

    s = s + TextFormatting::RESET;
    if (advanced->isAdvanced()) 
    {
        std::string s1;
        if (!s.empty()) 
        {
            s = s + " (";
            s1 = ")";
        }

        auto i = Item::getIdFromItem(item);
        if (getHasSubtypes()) 
        {
            s = s + fmt::format("#%04d/%d%s", i, itemDamage, s1);
        }
        else 
        {
            s = s + fmt::format("#%04d%s", i, s1);
        }
    }
    else if (!hasDisplayName() && item == Items::FILLED_MAP) 
    {
        s = s + " #" + itemDamage;
    }

    list.push_back(s);
    int32_t i1 = 0;
    if (hasTagCompound() && stackTagCompound->hasKey("HideFlags", 99)) 
    {
        i1 = stackTagCompound->getInteger("HideFlags");
    }

    if ((i1 & 32) == 0) 
    {
        getItem()->addInformation(*this, playerIn == nullptr ? nullptr : playerIn->world, list, advanced);
    }

    int32_t k1;
    NBTTagList* nbttaglist2;
    int32_t l1;
    if (hasTagCompound()) 
    {
        if ((i1 & 1) == 0) 
        {
            nbttaglist2 = getEnchantmentTagList();

            for(k1 = 0; k1 < nbttaglist2->tagCount(); ++k1) 
            {
                NBTTagCompound* nbttagcompound = nbttaglist2->getCompoundTagAt(k1);
                int k = nbttagcompound->getShort("id");
                int l = nbttagcompound->getShort("lvl");
                auto enchantment = Enchantment::getEnchantmentByID(k);
                if (enchantment != nullptr) 
                {
                    list.push_back(enchantment.getTranslatedName(l));
                }
            }
        }

        if (stackTagCompound->hasKey("display", 10)) 
        {
            NBTTagCompound* nbttagcompound1 = stackTagCompound->getCompoundTag("display");
            if (nbttagcompound1->hasKey("color", 3)) 
            {
                if (advanced->isAdvanced()) 
                {
                    list.push_back(I18n::translateToLocalFormatted("item.color", fmt::format("#%06X", nbttagcompound1->getInteger("color"))));
                }
                else 
                {
                    list.push_back(TextFormatting::ITALIC + I18n::translateToLocal("item.dyed"));
                }
            }

            if (nbttagcompound1->getTagId("Lore") == 9) 
            {
                NBTTagList* nbttaglist3 = nbttagcompound1->getTagList("Lore", 8);
                if (!nbttaglist3->isEmpty()) 
                {
                    for(l1 = 0; l1 < nbttaglist3->tagCount(); ++l1) 
                    {
                        list.push_back(TextFormatting::DARK_PURPLE + "" + TextFormatting::ITALIC + nbttaglist3->getStringTagAt(l1));
                    }
                }
            }
        }
    }

    auto var22 = EntityEquipmentSlot.values();
    k1 = var22.size();

    for(l1 = 0; l1 < k1; ++l1) 
    {
        EntityEquipmentSlot entityequipmentslot = var22[l1];
        Multimap multimap = getAttributeModifiers(entityequipmentslot);
        if (!multimap.isEmpty() && (i1 & 2) == 0) {
            list.push_back("");
            list.push_back(I18n::translateToLocal("item.modifiers." + entityequipmentslot.getName()));
            Iterator var28 = multimap.entries().iterator();

            while(var28.hasNext()) 
            {
                Entry entry = (Entry)var28.next();
                AttributeModifier attributemodifier = (AttributeModifier)entry.getValue();
                double d0 = attributemodifier.getAmount();
                bool flag = false;
                if (playerIn != nullptr) 
                {
                    if (attributemodifier.getID() == Item::ATTACK_DAMAGE_MODIFIER) 
                    {
                        d0 += playerIn->getEntityAttribute(SharedMonsterAttributes::ATTACK_DAMAGE).getBaseValue();
                        d0 += (double)EnchantmentHelper::getModifierForCreature(this, EnumCreatureAttribute::UNDEFINED);
                        flag = true;
                    }
                    else if (attributemodifier.getID() == Item::ATTACK_SPEED_MODIFIER) 
                    {
                        d0 += playerIn->getEntityAttribute(SharedMonsterAttributes::ATTACK_SPEED).getBaseValue();
                        flag = true;
                    }
                }

                double d1 = 0.0;
                if (attributemodifier.getOperation() != 1 && attributemodifier.getOperation() != 2) 
                {
                    d1 = d0;
                }
                else 
                {
                    d1 = d0 * 100.0;
                }

                if (flag) 
                {
                    list.push_back(" " + I18n::translateToLocalFormatted("attribute.modifier.equals." + attributemodifier.getOperation(), fmt::format("{:.{}f}",d1, 2), I18n::translateToLocal("attribute.name." + entry.getKey())));
                }
                else if (d0 > 0.0) 
                {
                    list.push_back(TextFormatting::BLUE + " " + I18n::translateToLocalFormatted("attribute.modifier.plus." + attributemodifier.getOperation(),  fmt::format("{:.{}f}",d1, 2), I18n::translateToLocal("attribute.name." + entry.getKey())));
                }
                else if (d0 < 0.0) 
                {
                    d1 *= -1.0;
                    list.push_back(TextFormatting::RED + " " + I18n::translateToLocalFormatted("attribute.modifier.take." + attributemodifier.getOperation(),  fmt::format("{:.{}f}",d1, 2), I18n::translateToLocal("attribute.name." + entry.getKey())));
                }
            }
        }
    }

    if (hasTagCompound() && getTagCompound()->getBoolean("Unbreakable") && (i1 & 4) == 0) 
    {
        list.push_back(TextFormatting::BLUE + I18n::translateToLocal("item.unbreakable"));
    }

    Block* block1 = nullptr;
    if (hasTagCompound() && stackTagCompound->hasKey("CanDestroy", 9) && (i1 & 8) == 0) 
    {
        nbttaglist2 = stackTagCompound->getTagList("CanDestroy", 8);
        if (!nbttaglist2->isEmpty()) 
        {
            list.push_back("");
            list.push_back(TextFormatting::GRAY + I18n::translateToLocal("item.canBreak"));

            for(k1 = 0; k1 < nbttaglist2->tagCount(); ++k1) 
            {
                block1 = Block::getBlockFromName(nbttaglist2->getStringTagAt(k1));
                if (block1 != nullptr) 
                {
                    list.push_back(TextFormatting::DARK_GRAY + block1->getLocalizedName());
                }
                else 
                {
                    list.push_back(TextFormatting::DARK_GRAY + "missingno");
                }
            }
        }
    }

    if (hasTagCompound() && stackTagCompound->hasKey("CanPlaceOn", 9) && (i1 & 16) == 0) 
    {
        nbttaglist2 = stackTagCompound->getTagList("CanPlaceOn", 8);
        if (!nbttaglist2->isEmpty()) 
        {
            list.push_back("");
            list.push_back(TextFormatting::GRAY + I18n::translateToLocal("item.canPlace"));

            for(k1 = 0; k1 < nbttaglist2->tagCount(); ++k1) 
            {
                block1 = Block::getBlockFromName(nbttaglist2->getStringTagAt(k1));
                if (block1 != nullptr) 
                {
                    list.push_back(TextFormatting::DARK_GRAY + block1->getLocalizedName());
                }
                else 
                {
                    list.push_back(TextFormatting::DARK_GRAY + "missingno");
                }
            }
        }
    }

    if (advanced->isAdvanced()) 
    {
        if (isItemDamaged()) 
        {
            list.push_back(I18n::translateToLocalFormatted("item.durability", getMaxDamage() - getItemDamage(), getMaxDamage()));
        }

        list.push_back(TextFormatting::DARK_GRAY + ((ResourceLocation)Item::REGISTRY.getNameForObject(item)).to_string());
        if (hasTagCompound()) 
        {
            list.push_back(TextFormatting::DARK_GRAY + I18n::translateToLocalFormatted("item.nbt_tags", getTagCompound()->getKeySet().size()));
        }
    }

    return list;
}

bool ItemStack::hasEffect()
{
    return getItem()->hasEffect(*this);
}

EnumRarity ItemStack::getRarity()
{
    return getItem()->getRarity(*this);
}

bool ItemStack::isItemEnchantable()
{
    if (!getItem()->isEnchantable(*this)) 
    {
        return false;
    }
    else 
    {
        return !isItemEnchanted();
    }
}

void ItemStack::addEnchantment(Enchantment ench, int32_t level)
{
    if (stackTagCompound == nullptr) 
    {
        setTagCompound(new NBTTagCompound());
    }

    if (!stackTagCompound->hasKey("ench", 9)) 
    {
        stackTagCompound->setTag("ench", new NBTTagList());
    }

    NBTTagList* nbttaglist = stackTagCompound->getTagList("ench", 10);
    NBTTagCompound* nbttagcompound = new NBTTagCompound();
    nbttagcompound->setShort("id", Enchantment::getEnchantmentID(ench));
    nbttagcompound->setShort("lvl", level);
    nbttaglist->appendTag(nbttagcompound);
}

bool ItemStack::isItemEnchanted() const
{
    if (stackTagCompound != nullptr && stackTagCompound->hasKey("ench", 9)) 
    {
        return !stackTagCompound->getTagList("ench", 10)->isEmpty();
    }
    else 
    {
        return false;
    }
}

void ItemStack::setTagInfo(std::string_view key, NBTBase *value)
{
    if (stackTagCompound == nullptr) 
    {
        setTagCompound(new NBTTagCompound());
    }

    stackTagCompound->setTag(key, value);
}

bool ItemStack::canEditBlocks()
{
    return getItem()->canItemEditBlocks();
}

bool ItemStack::isOnItemFrame()
{
    return itemFrame != nullptr;
}

void ItemStack::setItemFrame(EntityItemFrame* frame)
{
    itemFrame = frame;
}

EntityItemFrame* ItemStack::getItemFrame()
{
    return isEmpty ? nullptr : itemFrame;
}

int32_t ItemStack::getRepairCost() const
{
    return hasTagCompound() && stackTagCompound->hasKey("RepairCost", 3) ? stackTagCompound->getInteger("RepairCost") : 0;
}

void ItemStack::setRepairCost(int32_t cost)
{
    if (!hasTagCompound()) 
    {
        stackTagCompound = new NBTTagCompound();
    }

    stackTagCompound->setInteger("RepairCost", cost);
}

std::unordered_multimap<std::string,AttributeModifier> ItemStack::getAttributeModifiers(EntityEquipmentSlot equipmentSlot)
{
    std::unordered_multimap<std::string,AttributeModifier> multimap;
    if (hasTagCompound() && stackTagCompound->hasKey("AttributeModifiers", 9)) 
    {
        NBTTagList* nbttaglist = stackTagCompound->getTagList("AttributeModifiers", 10);

        for(int i = 0; i < nbttaglist->tagCount(); ++i) 
        {
            NBTTagCompound* nbttagcompound = nbttaglist->getCompoundTagAt(i);
            AttributeModifier attributemodifier = SharedMonsterAttributes::readAttributeModifierFromNBT(nbttagcompound);
            if (attributemodifier != nullptr && (!nbttagcompound->hasKey("Slot", 8) || nbttagcompound->getString("Slot") == (equipmentSlot.getName())) && attributemodifier.getID().getLeastSignificantBits() != 0L && attributemodifier.getID().getMostSignificantBits() != 0L) 
            {
                multimap.emplace(nbttagcompound->getString("AttributeName"), attributemodifier);
            }
        }
    }
    else 
    {
        multimap = getItem()->getItemAttributeModifiers(equipmentSlot);
    }

    return (Multimap)multimap;
}

void ItemStack::addAttributeModifier(std::string_view attributeName, AttributeModifier modifier, EntityEquipmentSlot* equipmentSlot)
{
    if (stackTagCompound == nullptr) 
    {
        stackTagCompound = new NBTTagCompound();
    }

    if (!stackTagCompound->hasKey("AttributeModifiers", 9)) 
    {
        stackTagCompound->setTag("AttributeModifiers", new NBTTagList());
    }

    NBTTagList* nbttaglist = stackTagCompound->getTagList("AttributeModifiers", 10);
    NBTTagCompound* nbttagcompound = SharedMonsterAttributes.writeAttributeModifierToNBT(modifier);
    nbttagcompound->setString("AttributeName", attributeName);
    if (equipmentSlot != nullptr) 
    {
        nbttagcompound->setString("Slot", equipmentSlot.getName());
    }

    nbttaglist->appendTag(nbttagcompound);
}

ITextComponent * ItemStack::getTextComponent()
{
    TextComponentString* textcomponentstring = new TextComponentString(getDisplayName());
    if (hasDisplayName()) 
    {
        textcomponentstring->getStyle().setItalic(true);
    }

    ITextComponent* itextcomponent = (new TextComponentString("["))->appendSibling(textcomponentstring).appendText("]");
    if (!isEmpty) 
    {
        NBTTagCompound* nbttagcompound = writeToNBT(new NBTTagCompound());
        itextcomponent->getStyle().setHoverEvent(HoverEvent(HoverEvent::Action::SHOW_ITEM, TextComponentString(nbttagcompound->to_string())));
        itextcomponent->getStyle().setColor(getRarity().color);
    }

    return itextcomponent;
}

bool ItemStack::canDestroy(Block *blockIn)
{
    if (blockIn == canDestroyCacheBlock) 
    {
        return canDestroyCacheResult;
    }
    else 
    {
        canDestroyCacheBlock = blockIn;
        if (hasTagCompound() && stackTagCompound->hasKey("CanDestroy", 9)) 
        {
            auto nbttaglist = stackTagCompound->getTagList("CanDestroy", 8);

            for(auto i = 0; i < nbttaglist->tagCount(); ++i) 
            {
                auto block = Block::getBlockFromName(nbttaglist->getStringTagAt(i));
                if (block == blockIn) 
                {
                    canDestroyCacheResult = true;
                    return true;
                }
            }
        }

        canDestroyCacheResult = false;
        return false;
    }
}

bool ItemStack::canPlaceOn(Block *blockIn)
{
    if (blockIn == canPlaceOnCacheBlock) 
    {
        return canPlaceOnCacheResult;
    }
    else 
    {
        canPlaceOnCacheBlock = blockIn;
        if (hasTagCompound() && stackTagCompound->hasKey("CanPlaceOn", 9)) 
        {
            auto nbttaglist = stackTagCompound->getTagList("CanPlaceOn", 8);

            for(auto i = 0; i < nbttaglist->tagCount(); ++i) 
            {
                auto block = Block::getBlockFromName(nbttaglist->getStringTagAt(i));
                if (block == blockIn) 
                {
                    canPlaceOnCacheResult = true;
                    return true;
                }
            }
        }

        canPlaceOnCacheResult = false;
        return false;
    }
}

int32_t ItemStack::getAnimationsToGo() const
{
    return animationsToGo;
}

void ItemStack::setAnimationsToGo(int32_t animations)
{
    animationsToGo = animations;
}

int32_t ItemStack::getCount() const
{
    return isEmpty ? 0 : stackSize;
}

void ItemStack::setCount(int32_t size)
{
    stackSize = size;
    updateEmptyState();
}

void ItemStack::grow(int32_t quantity)
{
    setCount(stackSize + quantity);
}

void ItemStack::shrink(int32_t quantity)
{
    grow(-quantity);
}

EnumAction ItemStack::getItemUseAction()
{
    return getItem()->getItemUseAction(*this);
}

void ItemStack::updateEmptyState()
{
    bisEmpty = isEmpty();
}

bool ItemStack::isItemStackEqual(ItemStack other)
{
    if (stackSize != other.stackSize) 
    {
        return false;
    }
    else if (getItem() != other.getItem()) 
    {
        return false;
    }
    else if (itemDamage != other.itemDamage) 
    {
        return false;
    }
    else if (stackTagCompound == nullptr && other.stackTagCompound != nullptr) 
    {
        return false;
    }
    else 
    {
        return stackTagCompound == nullptr || stackTagCompound == other.stackTagCompound;
    }
}
