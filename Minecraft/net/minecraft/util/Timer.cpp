#include "Timer.h"
#include "../client/Minecraft.h"

Timer::Timer(float tps)
{
	tickLength = 1000.0F / tps;
	lastSyncSysClock = Minecraft.getSystemTime();
}

void Timer::updateTimer()
{
	long i = Minecraft.getSystemTime();
	elapsedPartialTicks = (float)(i - lastSyncSysClock) / tickLength;
	lastSyncSysClock = i;
	renderPartialTicks += elapsedPartialTicks;
	elapsedTicks = (int)renderPartialTicks;
	renderPartialTicks -= (float)elapsedTicks;
}


