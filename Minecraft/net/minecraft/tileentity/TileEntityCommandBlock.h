#include "TileEntity.h"
#include "CommandBlockBaseLogic.h"
#include "../command/CommandResultStats.h"

class ByteBuffer;

class TileEntityCommandBlock :public TileEntity
{
public:

	enum class Mode
	{
		SEQUENCE,
		AUTO,
		REDSTONE
	};
	
	NBTTagCompound* writeToNBT(NBTTagCompound* compound) override;
	void readFromNBT(NBTTagCompound* compound) override;
	std::optional<SPacketUpdateTileEntity> getUpdatePacket() override;
	bool onlyOpsCanSetNbt() override;
	CommandBlockBaseLogic& getCommandBlockLogic() const;
	CommandResultStats getCommandResultStats();
	void setPowered(bool poweredIn);
	bool isPowered() const;
	bool isAuto() const;
	void setAuto(bool autoIn);
	bool isConditionMet() const;
	bool setConditionMet();
	bool isSendToClient() const;
	void setSendToClient(bool p_184252_1_);
	TileEntityCommandBlock::Mode getMode();
	bool isConditional();
	void validate() override;
private:
	bool powered;
	bool automat;
	bool conditionMet;
	bool sendToClient;

	class CommandBlockLogic :public CommandBlockBaseLogic
	{
	public:
		explicit CommandBlockLogic(TileEntityCommandBlock* block);
		BlockPos getPosition() const;
		Vec3d getPositionVector() const;
		World* getEntityWorld() const;
		void setCommand(std::string_view command) override;
		void updateCommand() override;
		int32_t getCommandBlockType() override;
		void fillInInfo(ByteBuffer& buf) override;
		MinecraftServer* getServer();
	private:
		TileEntityCommandBlock* block;
	};

	CommandBlockLogic commandBlockLogic;
};
