#pragma once
#include <filesystem>
#include "NBTTagCompound.h"

namespace CompressedStreamTools
{
	std::unique_ptr<NBTTagCompound> readCompressed(std::istream &is);
	void writeCompressed(NBTTagCompound* compound, std::ostream &outputStream);
	void safeWrite(NBTTagCompound* compound, std::filesystem::path fileIn);
	void write(NBTTagCompound* compound, std::filesystem::path fileIn);
	std::unique_ptr<NBTTagCompound> read(std::filesystem::path fileIn);
	std::unique_ptr<NBTTagCompound> read(std::istream &inputStream);
	std::unique_ptr<NBTTagCompound> read(std::istream &input, NBTSizeTracker* accounter);
	void write(NBTTagCompound* compound, std::ostream &output);

}
