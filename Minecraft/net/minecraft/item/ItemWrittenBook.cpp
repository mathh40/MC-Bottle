#include "ItemWrittenBook.h"

#include "../../../../nlohmann_json/test/thirdparty/doctest/doctest.h"
#include "../nbt/NBTTagCompound.h"
#include "../nbt/NBTTagString.h"
#include "../stats/StatList.h"
#include "ItemStack.h"
#include "ItemWritableBook.h"
#include "StringUtils.h"
#include "World.h"
#include "text/TextComponentString.h"
#include "text/translation/I18n.h"

ItemWrittenBook::ItemWrittenBook()
{
    setMaxStackSize(1);
}

bool ItemWrittenBook::validBookTagContents(NBTTagCompound *nbt)
{
    if (!ItemWritableBook::isNBTValid(nbt))
    {
        return false;
    }
    else if (!nbt->hasKey("title", 8))
    {
        return false;
    }
    else
    {
        std::string s = nbt->getString("title");
        return s != nullptr && s.length() <= 32 ? nbt->hasKey("author", 8) : false;
    }
}

int32_t ItemWrittenBook::getGeneration(ItemStack book)
{
    return book.getTagCompound()->getInteger("generation");
}

std::string ItemWrittenBook::getItemStackDisplayName(ItemStack stack) const
{
    if (stack.hasTagCompound())
    {
        NBTTagCompound *nbttagcompound = stack.getTagCompound();
        std::string s                  = nbttagcompound->getString("title");
        if (!StringUtils::isNullOrEmpty(s))
        {
            return s;
        }
    }

    return Item::getItemStackDisplayName(stack);
}

void ItemWrittenBook::addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                                     ITooltipFlag *flagIn)
{
    if (stack.hasTagCompound())
    {
        NBTTagCompound *nbttagcompound = stack.getTagCompound();
        std::string s                  = nbttagcompound->getString("author");
        if (!StringUtils::isNullOrEmpty(s))
        {
            tooltip.emplace(TextFormatting::GRAY + I18n::translateToLocalFormatted("book.byAuthor", s));
        }

        tooltip.emplace(TextFormatting::GRAY +
                        I18n::translateToLocal("book.generation." + nbttagcompound->getInteger("generation")));
    }
}

ActionResult ItemWrittenBook::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    if (!worldIn->isRemote)
    {
        resolveContents(itemstack, playerIn);
    }

    playerIn->openBook(itemstack, handIn);
    playerIn->addStat(StatList::getObjectUseStats(this));
    return ActionResult(EnumActionResult::SUCCESS, itemstack);
}

bool ItemWrittenBook::hasEffect(ItemStack stack)
{
    return true;
}

void ItemWrittenBook::resolveContents(ItemStack stack, EntityPlayer *player)
{
    if (stack.getTagCompound() != nullptr)
    {
        NBTTagCompound *nbttagcompound = stack.getTagCompound();
        if (!nbttagcompound->getBoolean("resolved"))
        {
            nbttagcompound->setBoolean("resolved", true);
            if (validBookTagContents(nbttagcompound))
            {
                NBTTagList *nbttaglist = nbttagcompound->getTagList("pages", 8);

                for (auto i = 0; i < nbttaglist->tagCount(); ++i)
                {
                    std::string s = nbttaglist->getStringTagAt(i);

                    Object itextcomponent;
                    try
                    {
                        ITextComponent *itextcomponent = ITextComponent::Serializer.fromJsonLenient(s);
                        itextcomponent = TextComponentUtils::processComponent(player, itextcomponent, player);
                    }
                    catch (std::exception &var9)
                    {
                        itextcomponent = TextComponentString(s);
                    }

                    nbttaglist->set(
                        i, NBTTagString(ITextComponent::Serializer.componentToJson((ITextComponent *)itextcomponent)));
                }

                nbttagcompound->setTag("pages", nbttaglist);
                if (Util:: instanceof <EntityPlayerMP>(player) && player->getHeldItemMainhand() == stack)
                {
                    Slot slot =
                        player->openContainer.getSlotFromInventory(player->inventory, player->inventory.currentItem);
                    ((EntityPlayerMP *)player).connection.sendPacket(new SPacketSetSlot(0, slot.slotNumber, stack));
                }
            }
        }
    }
}
