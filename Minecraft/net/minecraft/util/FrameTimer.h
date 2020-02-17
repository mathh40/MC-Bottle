#pragma once
#include <array>
#include <atomic>
#include <spdlog/async_logger.h>

class FrameTimer
{
public:
	void addFrame(long runningTime);
	int getLagometerValue(long time, int multiplier);
	int getLastIndex();
	int getIndex();
	int parseIndex(int rawIndex);
	std::array<long, 240> getFrames();
private:
	std::array<long, 240> frames;
	int lastIndex = 0;
	int counter = 0;
	int index = 0;
};
