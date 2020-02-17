#include "DataFixer.h"

namespace DataFixesManager
{
	DataFixer createFixer();
	std::shared_ptr<NBTTagCompound> processItemStack(IDataFixer* fixer, std::shared_ptr<NBTTagCompound> compound, int32_t version, std::string key);
	std::shared_ptr<NBTTagCompound> processInventory(IDataFixer* fixer, std::shared_ptr<NBTTagCompound> compound, int32_t version, std::string key);
}
