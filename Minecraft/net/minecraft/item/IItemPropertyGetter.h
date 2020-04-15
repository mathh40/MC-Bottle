class EntityLivingBase;
class World;
class ItemStack;

using IItemPropertyGetter = std::function<float(ItemStack stack, World* worldIn, EntityLivingBase* entityIn)>;
