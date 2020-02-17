#include "WorldSavedData.h"

class WorldSavedDataCallableSave
{
public:
	explicit WorldSavedDataCallableSave(WorldSavedData* dataIn);
	void run() const;
private:
	WorldSavedData* data;
};
