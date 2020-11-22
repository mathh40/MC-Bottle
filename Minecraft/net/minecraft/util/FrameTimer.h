#pragma once
#include <array>
#include <atomic>

class FrameTimer
{
public:
	void addFrame(long runningTime);
	int getLagometerValue(long time, int multiplier);
	int getLastIndex() const;
	int getIndex() const;
	int parseIndex(int rawIndex);
	std::array<long, 240> getFrames() const;
private:
	std::array<long, 240> frames;
	int lastIndex = 0;
	int counter = 0;
	int index = 0;
};
