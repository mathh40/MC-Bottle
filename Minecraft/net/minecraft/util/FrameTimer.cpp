#include "FrameTimer.h"
#include <spdlog/spdlog.h>

void FrameTimer::addFrame(long runningTime)
{
	frames[index] = runningTime;
	++index;
	if (index == 240) {
		index = 0;
	}

	if (counter < 240) {
		lastIndex = 0;
		++counter;
	}
	else {
		lastIndex = parseIndex(index + 1);
	}
}

int FrameTimer::getLagometerValue(long time, int multiplier)
{
	double d0 = (double)time / 1.6666666E7;
	return (int)(d0 * (double)multiplier);
}

int FrameTimer::getLastIndex()
{
	return lastIndex;
}

int FrameTimer::getIndex()
{
	return index;
}

int FrameTimer::parseIndex(int rawIndex)
{
	return rawIndex % 240;
}

std::array<long, 240> FrameTimer::getFrames()
{
	return frames;
}
