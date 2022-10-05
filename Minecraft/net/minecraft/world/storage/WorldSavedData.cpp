#include "WorldSavedData.h"

WorldSavedData::WorldSavedData(std::string_view name) : mapName(name)
{
}

void WorldSavedData::markDirty()
{
    setDirty(true);
}

void WorldSavedData::setDirty(bool isDirty)
{
    dirty = isDirty;
}

bool WorldSavedData::isDirty() const
{
    return dirty;
}
