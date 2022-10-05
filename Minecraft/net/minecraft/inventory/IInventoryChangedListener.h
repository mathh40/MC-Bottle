class IInventory;

class IInventoryChangedListener
{
  public:
    virtual ~IInventoryChangedListener()              = default;
    virtual void onInventoryChanged(IInventory *var1) = 0;
};
