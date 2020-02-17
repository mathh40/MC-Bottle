#pragma once
#include <memory>

class NBTSizeTracker
{
public:
	static std::unique_ptr<NBTSizeTracker> Infinite;

	NBTSizeTracker(int64_t max);

	void read(int64_t bits);
private:
	int64_t max;
	int64_t readi;
};
