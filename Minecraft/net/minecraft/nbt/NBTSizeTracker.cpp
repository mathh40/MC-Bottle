#include "NBTSizeTracker.h"
#include <string>
#include <stdexcept>

const std::unique_ptr<NBTSizeTracker> NBTSizeTracker::Infinite = std::make_unique<NBTSizeTracker>(0L);

NBTSizeTracker::NBTSizeTracker(uint64_t max)
	:max(max)
{

}

void NBTSizeTracker::read(uint64_t bits)
{
	readi += bits / 8L;
	if (readi > max) {
		throw std::runtime_error("Tried to read NBT tag that was too big; tried to allocate: " + std::to_string(readi) + "bytes where max allowed: " + std::to_string(max));
	}
}
