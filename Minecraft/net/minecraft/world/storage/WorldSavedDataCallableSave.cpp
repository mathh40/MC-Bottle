#include "WorldSavedDataCallableSave.h"

WorldSavedDataCallableSave::WorldSavedDataCallableSave(WorldSavedData *dataIn) : data(dataIn)
{
}

void WorldSavedDataCallableSave::run() const
{
    data->markDirty();
}
