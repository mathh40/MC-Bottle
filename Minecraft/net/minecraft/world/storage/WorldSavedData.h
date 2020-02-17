#pragma once
#include <string>
#include <memory>

class NBTTagCompound;

class WorldSavedData
{
public:
	std::string mapName;
	explicit WorldSavedData(std::string_view name);
	virtual void readFromNBT(NBTTagCompound* var1) = 0;
	virtual NBTTagCompound* writeToNBT(NBTTagCompound* var1);
	void markDirty();
	void setDirty(bool isDirty);
	bool isDirty() const;
private: 
	bool dirty;

};
