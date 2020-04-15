#include "ItemEnchantedBook.h"

#include "ItemStack.h"

bool ItemEnchantedBook::hasEffect(ItemStack stack)
{
    return true;
}

bool ItemEnchantedBook::isEnchantable(ItemStack stack) const
{
    return false;
}

EnumRarity ItemEnchantedBook::getRarity(ItemStack stack)
{
    return getEnchantments(stack).isEmpty() ? Item::getRarity(stack) : EnumRarity::UNCOMMON;
}

NBTTagList * ItemEnchantedBook::getEnchantments(ItemStack p_92110_0_)
{
    NBTTagCompound* nbttagcompound = p_92110_0_->getTagCompound();
    return nbttagcompound != nullptr ? nbttagcompound->getTagList("StoredEnchantments", 10) : new NBTTagList();
}

void ItemEnchantedBook::addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn)
{
    Item::addInformation(stack, worldIn, tooltip, flagIn);
    NBTTagList* nbttaglist = getEnchantments(stack);
    
    for(int32_t i = 0; i < nbttaglist->tagCount(); ++i) 
    {
        NBTTagCompound* nbttagcompound = nbttaglist->getCompoundTagAt(i);
        int32_t j = nbttagcompound->getShort("id");
        Enchantment enchantment = Enchantment::getEnchantmentByID(j);
        if (enchantment != nullptr) 
        {
            tooltip.push_back(enchantment.getTranslatedName(nbttagcompound->getShort("lvl")));
        }
    }
}

void ItemEnchantedBook::addEnchantment(ItemStack p_92115_0_, EnchantmentData stack)
{
    NBTTagList* nbttaglist = getEnchantments(p_92115_0_);
    bool flag = true;

    for(int i = 0; i < nbttaglist->tagCount(); ++i) 
    {
        NBTTagCompound* nbttagcompound = nbttaglist->getCompoundTagAt(i);
        if (Enchantment::getEnchantmentByID(nbttagcompound->getShort("id")) == stack.enchantment) 
        {
            if (nbttagcompound->getShort("lvl") < stack.enchantmentLevel) 
            {
                nbttagcompound->setShort("lvl", (short)stack.enchantmentLevel);
            }

            flag = false;
            break;
        }
    }

    if (flag) 
    {
        NBTTagCompound* nbttagcompound1 = new NBTTagCompound();
        nbttagcompound1->setShort("id", (short)Enchantment.getEnchantmentID(stack.enchantment));
        nbttagcompound1->setShort("lvl", (short)stack.enchantmentLevel);
        nbttaglist->appendTag(nbttagcompound1);
    }

    if (!p_92115_0_.hasTagCompound()) 
    {
        p_92115_0_.setTagCompound(new NBTTagCompound());
    }

    p_92115_0_.getTagCompound()->setTag("StoredEnchantments", nbttaglist);
}

ItemStack ItemEnchantedBook::getEnchantedItemStack(EnchantmentData p_92111_0_)
{
    ItemStack itemstack = ItemStack(Items::ENCHANTED_BOOK);
    addEnchantment(itemstack, p_92111_0_);
    return itemstack;
}

void ItemEnchantedBook::getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items)
{
    Iterator var3;
    Enchantment enchantment;
    if (tab == CreativeTabs.SEARCH) 
    {
        var3 = Enchantment.REGISTRY.iterator();

        while(true) 
        {
            do 
            {
                if (!var3.hasNext()) 
                {
                    return;
                }

                enchantment = (Enchantment)var3.next();
            } while(enchantment.type == nullptr);

            for(int i = enchantment.getMinLevel(); i <= enchantment.getMaxLevel(); ++i) {
                items.add(getEnchantedItemStack(new EnchantmentData(enchantment, i)));
            }
        }
    }
    else if (tab.getRelevantEnchantmentTypes().length != 0) 
    {
        var3 = Enchantment.REGISTRY.iterator();

        while(var3.hasNext()) 
        {
            enchantment = (Enchantment)var3.next();
            if (tab.hasRelevantEnchantmentType(enchantment.type)) 
            {
                items.add(getEnchantedItemStack(new EnchantmentData(enchantment, enchantment.getMaxLevel())));
            }
        }
    }
}
