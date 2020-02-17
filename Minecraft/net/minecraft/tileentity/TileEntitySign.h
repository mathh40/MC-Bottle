#include "TileEntity.h"
#include "text/TextComponentString.h"

class TileEntitySign :public TileEntity
{
public:
	NBTTagCompound* writeToNBT(NBTTagCompound* compound) override;
	void readFromNBT(NBTTagCompound* compound) override;
   std::optional<SPacketUpdateTileEntity> getUpdatePacket() override;
   NBTTagCompound* getUpdateTag() override;
   bool onlyOpsCanSetNbt() override;
   bool getIsEditable() const;
   void setEditable(bool isEditableIn);
   void setPlayer(EntityPlayer* playerIn);
   EntityPlayer* getPlayer() const;
   bool executeCommand(const EntityPlayer* playerIn);
   CommandResultStats getStats();

   class Command : ICommandSender
   {
   public:

      Command(TileEntitySign* entity)
      std::string getName()
      {
         return "Sign";
      }

      bool canUseCommand(int permLevel, std::string_view commandName) {
         return true;
      }

      BlockPos getPosition() {
         return entity->pos;
      }

      Vec3d getPositionVector() {
         return Vec3d((double)entity->pos.getx() + 0.5, (double)entity->pos.gety() + 0.5, (double)entity->pos.getz() + 0.5);
      }

      World* getEntityWorld()
      {
         return entity->world;
      }

      MinecraftServer* getServer()
      {
         return entity->world.getMinecraftServer();
      }

   private:
      TileEntitySign* entity;
   };

	
	ITextComponent* signText = new ITextComponent[]{TextComponentString(""), TextComponentString(""), TextComponentString(""), TextComponentString("")};
	int32_t lineBeingEdited = -1;
protected:
	void setWorldCreate(World* worldIn) override;	
private:
	bool isEditable = true;
	EntityPlayer* player;
	CommandResultStats stats;
};