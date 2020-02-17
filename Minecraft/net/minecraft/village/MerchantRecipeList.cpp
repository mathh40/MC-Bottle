#include "MerchantRecipeList.h"
#include <optional>
#include "NBTTagList.h"

MerchantRecipeList::MerchantRecipeList(NBTTagCompound* compound)
{
	readRecipiesFromTags(compound);
}

bool MerchantRecipeList::areItemStacksExactlyEqual(ItemStack stack1, ItemStack stack2)
{
   return ItemStack.areItemsEqual(stack1, stack2) && (!stack2.hasTagCompound() || stack1.hasTagCompound() && NBTUtil::areNBTEquals(stack2.getTagCompound(), stack1.getTagCompound(), false));
}

void MerchantRecipeList::writeToBuf(PacketBuffer& buffer)
{
   buffer.writeByte(list.size() & 255);

      for(int i = 0; i < list.size(); ++i) 
      {
         MerchantRecipe merchantrecipe = (MerchantRecipe)list[i];
         buffer.writeItemStack(merchantrecipe.getItemToBuy());
         buffer.writeItemStack(merchantrecipe.getItemToSell());
         ItemStack itemstack = merchantrecipe.getSecondItemToBuy();
         buffer.writeBoolean(!itemstack.isEmpty());
         if (!itemstack.isEmpty()) {
            buffer.writeItemStack(itemstack);
         }

         buffer.writeBoolean(merchantrecipe.isRecipeDisabled());
         buffer.writeInt(merchantrecipe.getToolUses());
         buffer.writeInt(merchantrecipe.getMaxTradeUses());
      }
}

void MerchantRecipeList::add(const MerchantRecipe& merchantrecipe)
{
   list.emplace_back(merchantrecipe);
}

void MerchantRecipeList::readRecipiesFromTags(NBTTagCompound* compound)
{
   NBTTagList* nbttaglist = compound->getTagList("Recipes", 10);

      for(auto i = 0; i < nbttaglist->tagCount(); ++i) 
      {
         NBTTagCompound* nbttagcompound = nbttaglist->getCompoundTagAt(i);
         add(MerchantRecipe(nbttagcompound));
      }
}

MerchantRecipeList MerchantRecipeList::readFromBuf(const PacketBuffer& buffer)
{
   MerchantRecipeList merchantrecipelist = new MerchantRecipeList();
      auto i = buffer.readByte() & 255;

      for(auto j = 0; j < i; ++j) {
         ItemStack itemstack = buffer.readItemStack();
         ItemStack itemstack1 = buffer.readItemStack();
         ItemStack itemstack2 = ItemStack::EMPTY;
         if (buffer.readBoolean()) {
            itemstack2 = buffer.readItemStack();
         }

         bool flag = buffer.readBoolean();
         auto k = buffer.readInt();
         auto l = buffer.readInt();
         MerchantRecipe merchantrecipe = MerchantRecipe(itemstack, itemstack2, itemstack1, k, l);
         if (flag) {
            merchantrecipe.compensateToolUses();
         }

         merchantrecipelist.add(merchantrecipe);
      }

      return merchantrecipelist;
}

NBTTagCompound* MerchantRecipeList::getRecipiesAsTags()
{
   NBTTagCompound* nbttagcompound = new NBTTagCompound();
      NBTTagList* nbttaglist = new NBTTagList();

      for(auto i = 0; i < list.size(); ++i) {
         MerchantRecipe merchantrecipe = list[i];
         nbttaglist->appendTag(merchantrecipe.writeToTags());
      }

      nbttagcompound->setTag("Recipes", nbttaglist);
      return nbttagcompound;
}

std::optional<MerchantRecipe> MerchantRecipeList::canRecipeBeUsed(ItemStack stack0, ItemStack stack1, int32_t index)
{
   if (index > 0 && index < list.size()) {
      MerchantRecipe merchantrecipe1 = list[index];
      return areItemStacksExactlyEqual(stack0, merchantrecipe1.getItemToBuy()) && (stack1.isEmpty() && !merchantrecipe1.hasSecondItemToBuy() || merchantrecipe1.hasSecondItemToBuy() && areItemStacksExactlyEqual(stack1, merchantrecipe1.getSecondItemToBuy())) && stack0.getCount() >= merchantrecipe1.getItemToBuy().getCount() && (!merchantrecipe1.hasSecondItemToBuy() || stack1.getCount() >= merchantrecipe1.getSecondItemToBuy().getCount()) ? merchantrecipe1 : std::nullopt;
   } else {
      for(int32_t i = 0; i < list.size(); ++i) {
         MerchantRecipe merchantrecipe = list[i];
         if (areItemStacksExactlyEqual(stack0, merchantrecipe.getItemToBuy()) && stack0.getCount() >= merchantrecipe.getItemToBuy().getCount() && (!merchantrecipe.hasSecondItemToBuy() && stack1.isEmpty() || merchantrecipe.hasSecondItemToBuy() && areItemStacksExactlyEqual(stack1, merchantrecipe.getSecondItemToBuy()) && stack1.getCount() >= merchantrecipe.getSecondItemToBuy().getCount())) {
            return merchantrecipe;
         }
      }

      return std::nullopt;
   }
}
