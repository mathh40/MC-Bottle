#include "TileEntity.h"
#include "ITickable.h"
class TileEntityEnderChest :public TileEntity , ITickable
{
public:
	void update() override;
	bool receiveClientEvent(int32_t id, int32_t type) override;
	void invalidate() override;
	void openChest();
	void closeChest();
	bool canBeUsed(EntityPlayer* player);
	
	float lidAngle;
	float prevLidAngle;
	int32_t numPlayersUsing;
private:
	int32_t ticksSinceSync;
};