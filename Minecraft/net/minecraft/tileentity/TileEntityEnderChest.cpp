#include "TileEntityEnderChest.h"

void TileEntityEnderChest::update()
{
   if (++ticksSinceSync % 20 * 4 == 0) 
   {
      world->addBlockEvent(pos, Blocks::ENDER_CHEST, 1, numPlayersUsing);
   }

   prevLidAngle = lidAngle;
   int i = pos.getx();
   int j = pos.gety();
   int k = pos.getz();
   float f = 0.1F;
   double d3;
   if (numPlayersUsing > 0 && lidAngle == 0.0F) 
   {
      double d0 = (double)i + 0.5;
      d3 = (double)k + 0.5;
      world->playSound(nullptr, d0, (double)j + 0.5, d3, SoundEvents::BLOCK_ENDERCHEST_OPEN, SoundCategory::BLOCKS, 0.5F, MathHelper::nextFloat(world->rand) * 0.1F + 0.9F);
   }

   if (numPlayersUsing == 0 && lidAngle > 0.0F || numPlayersUsing > 0 && lidAngle < 1.0F) 
   {
      float f2 = lidAngle;
      if (numPlayersUsing > 0) 
      {
         lidAngle += 0.1F;
      }
      else 
      {
         lidAngle -= 0.1F;
      }

      if (lidAngle > 1.0F) 
      {
         lidAngle = 1.0F;
      }

      float f1 = 0.5F;
      if (lidAngle < 0.5F && f2 >= 0.5F) 
      {
         d3 = (double)i + 0.5;
         double d2 = (double)k + 0.5;
         world->playSound(nullptr, d3, (double)j + 0.5, d2, SoundEvents::BLOCK_ENDERCHEST_CLOSE, SoundCategory::BLOCKS, 0.5F, MathHelper::nextFloat(world->rand) * 0.1F + 0.9F);
      }

      if (lidAngle < 0.0F) 
      {
         lidAngle = 0.0F;
      }
   }
}

bool TileEntityEnderChest::receiveClientEvent(int32_t id, int32_t type)
{
   if (id == 1) 
   {
      numPlayersUsing = type;
      return true;
   }
   else 
   {
      return TileEntity::receiveClientEvent(id, type);
   }
}

void TileEntityEnderChest::invalidate()
{
   updateContainingBlockInfo();
   TileEntity::invalidate();
}

void TileEntityEnderChest::openChest()
{
   ++numPlayersUsing;
   world->addBlockEvent(pos, Blocks::ENDER_CHEST, 1, numPlayersUsing);
}

void TileEntityEnderChest::closeChest()
{
   --numPlayersUsing;
   world->addBlockEvent(pos, Blocks::ENDER_CHEST, 1, numPlayersUsing);
}

bool TileEntityEnderChest::canBeUsed(EntityPlayer* player)
{
   if (world->getTileEntity(pos) != this) 
   {
      return false;
   }
   else 
   {
      return player->getDistanceSq((double)pos.getx() + 0.5, (double)pos.gety() + 0.5, (double)pos.getz() + 0.5) <= 64.0;
   }
}
