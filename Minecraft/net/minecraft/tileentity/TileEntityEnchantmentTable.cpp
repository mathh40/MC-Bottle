#include "TileEntityEnchantmentTable.h"
#include "text/TextComponentTranslation.h"
#include "text/TextComponentString.h"

NBTTagCompound* TileEntityEnchantmentTable::writeToNBT(NBTTagCompound* compound)
{
	TileEntity::writeToNBT(compound);
	if (hasCustomName()) 
	{
		compound->setString("CustomName", customName);
	}

	return compound;
}

void TileEntityEnchantmentTable::readFromNBT(NBTTagCompound* compound)
{
	TileEntity::readFromNBT(compound);
	if (compound->hasKey("CustomName", 8)) 
	{
		customName = compound->getString("CustomName");
	}
}

void TileEntityEnchantmentTable::update()
{
   bookSpreadPrev = bookSpread;
   bookRotationPrev = bookRotation;
   EntityPlayer* entityplayer = world->getClosestPlayer((double)((float)pos.getx() + 0.5F), (double)((float)pos.gety() + 0.5F), (double)((float)pos.getz() + 0.5F), 3.0, false);
   if (entityplayer != nullptr) 
   {
      double d0 = entityplayer->posx - (double)((float)pos.getx() + 0.5F);
      double d1 = entityplayer->posz - (double)((float)pos.getz() + 0.5F);
      tRot = (float)MathHelper::atan2(d1, d0);
      bookSpread += 0.1F;
      if (bookSpread < 0.5F || rand(40) == 0) 
      {
         float f1 = flipT;

         do 
         {
            flipT += (float)(rand(4) - rand(4));
         }
         while(f1 == flipT);
      }
   }
   else 
   {
      tRot += 0.02F;
      bookSpread -= 0.1F;
   }

   while(bookRotation >= 3.1415927F) 
   {
      bookRotation -= 6.2831855F;
   }

   while(bookRotation < -3.1415927F) 
   {
      bookRotation += 6.2831855F;
   }

   while(tRot >= 3.1415927F) 
   {
      tRot -= 6.2831855F;
   }

   while(tRot < -3.1415927F) 
   {
      tRot += 6.2831855F;
   }

   float f2;
   for(f2 = tRot - bookRotation; f2 >= 3.1415927F; f2 -= 6.2831855F) 
   {
   }

   while(f2 < -3.1415927F) 
   {
      f2 += 6.2831855F;
   }

   bookRotation += f2 * 0.4F;
   bookSpread = MathHelper::clamp(bookSpread, 0.0F, 1.0F);
   ++tickCount;
   pageFlipPrev = pageFlip;
   float f = (flipT - pageFlip) * 0.4F;
   float f3 = 0.2F;
   f = MathHelper::clamp(f, -0.2F, 0.2F);
   flipA += (f - flipA) * 0.9F;
   pageFlip += flipA;
}

std::string TileEntityEnchantmentTable::getName() const
{
   return hasCustomName() ? customName : "container.enchant";
}

bool TileEntityEnchantmentTable::hasCustomName() const
{
   return !customName.empty();
}

void TileEntityEnchantmentTable::setCustomName(std::string customNameIn)
{
   customName = customNameIn;
}

ITextComponent* TileEntityEnchantmentTable::getDisplayName()
{
   return (ITextComponent)(hasCustomName() ? new TextComponentString(getName()) : new TextComponentTranslation(getName(), new Object[0]));
}

Container TileEntityEnchantmentTable::createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn)
{
   return new ContainerEnchantment(playerInventory, world, pos);
}

std::string TileEntityEnchantmentTable::getGuiID() const
{
   return "minecraft:enchanting_table";
}
