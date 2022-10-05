#include "EntityAIVillagerInteract.h"

#include "../../inventory/InventoryBasic.h"
#include "Util.h"
#include "math/MathHelper.h"

class EntityItem;

EntityAIVillagerInteract::EntityAIVillagerInteract(EntityVillager *villagerIn)
    : EntityAIWatchClosest2(villagerIn, typeid(EntityVillager), 3.0F, 0.02F), villager(villagerIn)
{
}

void EntityAIVillagerInteract::startExecuting()
{
    EntityAIWatchClosest2::startExecuting();
    if (villager->canAbondonItems() && Util:: instanceof <EntityVillager>(closestEntity) &&
                                                             ((EntityVillager *)closestEntity)->wantsMoreFood())
    {
        interactionDelay = 10;
    }
    else
    {
        interactionDelay = 0;
    }
}

void EntityAIVillagerInteract::updateTask()
{
    EntityAIWatchClosest2::updateTask();
    if (interactionDelay > 0)
    {
        --interactionDelay;
        if (interactionDelay == 0)
        {
            InventoryBasic *inventorybasic = villager->getVillagerInventory();

            for (auto i = 0; i < inventorybasic->getSizeInventory(); ++i)
            {
                ItemStack itemstack  = inventorybasic->getStackInSlot(i);
                ItemStack itemstack1 = ItemStack::EMPTY;
                if (!itemstack.isEmpty())
                {
                    Item *item = itemstack.getItem();
                    int32_t j;
                    if ((item == Items::BREAD || item == Items::POTATO || item == Items::CARROT ||
                         item == Items::BEETROOT) &&
                        itemstack.getCount() > 3)
                    {
                        j = itemstack.getCount() / 2;
                        itemstack.shrink(j);
                        itemstack1 = ItemStack(item, j, itemstack.getMetadata());
                    }
                    else if (item == Items::WHEAT && itemstack.getCount() > 5)
                    {
                        j         = itemstack.getCount() / 2 / 3 * 3;
                        int32_t k = j / 3;
                        itemstack.shrink(j);
                        itemstack1 = ItemStack(Items::BREAD, k, 0);
                    }

                    if (itemstack.isEmpty())
                    {
                        inventorybasic->setInventorySlotContents(i, ItemStack::EMPTY);
                    }
                }

                if (!itemstack1.isEmpty())
                {
                    double d0 = villager->posY - 0.30000001192092896 + (double)villager->getEyeHeight();
                    EntityItem *entityitem =
                        new EntityItem(villager->world, villager->posX, d0, villager->posZ, itemstack1);
                    float f  = 0.3F;
                    float f1 = villager->rotationYawHead;
                    float f2 = villager->rotationPitch;
                    entityitem->motionX =
                        (double)(-MathHelper::sin(f1 * 0.017453292F) * MathHelper::cos(f2 * 0.017453292F) * 0.3F);
                    entityitem->motionZ =
                        (double)(MathHelper::cos(f1 * 0.017453292F) * MathHelper::cos(f2 * 0.017453292F) * 0.3F);
                    entityitem->motionY = (double)(-MathHelper::sin(f2 * 0.017453292F) * 0.3F + 0.1F);
                    entityitem->setDefaultPickupDelay();
                    villager->world.spawnEntity(entityitem);
                    break;
                }
            }
        }
    }
}
