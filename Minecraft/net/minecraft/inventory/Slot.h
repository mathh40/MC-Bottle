#pragma once
#include <cstdint>
#include <string>
class EntityPlayer;
class ItemStack;
class IInventory;
class Slot
{
  public:
    virtual ~Slot() = default;

    Slot(IInventory *inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition);
    virtual void onSlotChange(ItemStack p_75220_1_, ItemStack p_75220_2_);
    virtual ItemStack onTake(EntityPlayer *thePlayer, ItemStack stack);
    virtual bool isItemValid(ItemStack stack);
    ItemStack getStack() const;
    bool getHasStack() const;
    void putStack(ItemStack stack) const;
    void onSlotChanged() const;
    virtual int32_t getSlotStackLimit() const;
    int32_t getItemStackLimit(ItemStack stack) const;
    virtual std::string getSlotTexture();
    virtual ItemStack decrStackSize(int32_t amount);
    bool isHere(IInventory *inv, int32_t slotIn) const;
    virtual bool canTakeStack(EntityPlayer *playerIn);
    virtual bool isEnabled();

    IInventory *inventory;
    int32_t slotNumber;
    int32_t xPos;
    int32_t yPos;

  protected:
    virtual void onCrafting(ItemStack stack, int32_t amount);
    virtual void onSwapCraft(int32_t p_190900_1_);
    virtual void onCrafting(ItemStack stack);

  private:
    int32_t slotIndex;
};
