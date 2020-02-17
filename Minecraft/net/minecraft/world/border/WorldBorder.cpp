#include "WorldBorder.h"
#include "../util/math/BlockPos.h"
#include "../util/math/ChunkPos.h"
#include "../util/math/AxisAlignedBB.h"
#include "../../util/math/MathHelper.h"
#include "IBorderListener.h"
#include "../../util/Util.h"

WorldBorder::WorldBorder()
{
	endDiameter = startDiameter;
	worldSize = 29999984;
	damageAmount = 0.2;
	damageBuffer = 5.0;
	warningTime = 15;
	warningDistance = 5;
}

bool WorldBorder::contains(BlockPos& pos)
{
	return (double)(pos.getx() + 1) > minX() && (double)pos.getx() < maxX() && (double)(pos.getz() + 1) > minZ() && (double)pos.getz() < maxZ();
}

bool WorldBorder::contains(ChunkPos& range)
{
	return (double)range.getXEnd() > minX() && (double)range.getXStart() < maxX() && (double)range.getZEnd() > minZ() && (double)range.getZStart() < maxZ();
}

bool WorldBorder::contains(AxisAlignedBB& bb)
{
	return bb.getmaxX() > minX() && bb.getminX() < maxX() && bb.getmaxZ() > minZ() && bb.getminZ() < maxZ();
}

double WorldBorder::getClosestDistance(Entity* entityIn)
{
	return getClosestDistance(entityIn->posX, entityIn->posZ);
}

double WorldBorder::getClosestDistance(double x, double z)
{
	double d0 = z - minZ();
	double d1 = maxZ() - z;
	double d2 = x - minX();
	double d3 = maxX() - x;
	double d4 = MathHelper::min(d2, d3);
	d4 = MathHelper::min(d4, d0);
	return MathHelper::min(d4, d1);
}

EnumBorderStatus WorldBorder::getStatus() const
{
	if (endDiameter < startDiameter) 
	{
		return EnumBorderStatus::SHRINKING;
	}
	else 
	{
		return endDiameter > startDiameter ? EnumBorderStatus::GROWING : EnumBorderStatus::STATIONARY;
	}
}

double WorldBorder::minX()
{
	double d0 = getCenterX() - getDiameter() / 2.0;
	if (d0 < (double)(-worldSize)) 
	{
		d0 = (double)(-worldSize);
	}

	return d0;
}

double WorldBorder::minZ()
{
	double d0 = getCenterZ() - getDiameter() / 2.0;
	if (d0 < (double)(-worldSize)) 
	{
		d0 = (double)(-worldSize);
	}

	return d0;
}

double WorldBorder::maxX()
{
	double d0 = getCenterX() + getDiameter() / 2.0;
	if (d0 > (double)worldSize) 
	{
		d0 = (double)worldSize;
	}

	return d0;
}

double WorldBorder::maxZ()
{
	double d0 = getCenterZ() + getDiameter() / 2.0;
	if (d0 > (double)worldSize) 
	{
		d0 = (double)worldSize;
	}

	return d0;
}

double WorldBorder::getCenterX() const
{
	return centerX;
}

double WorldBorder::getCenterZ() const
{
	return centerZ;
}

void WorldBorder::setCenter(double x, double z)
{
	centerX = x;
	centerZ = z;

	for(auto iborderlistener : getListeners())
	{
		iborderlistener->onCenterChanged(*this, x, z);
	}
}

double WorldBorder::getDiameter()
{
	if (getStatus() != EnumBorderStatus::STATIONARY) 
	{
		double d0 = (double)((float)(Util::currentTimeMillis() - startTime) / (float)(endTime - startTime));
		if (d0 < 1.0) 
		{
			return startDiameter + (endDiameter - startDiameter) * d0;
		}

		setTransition(endDiameter);
	}

	return startDiameter;
}

double WorldBorder::getTargetSize() const
{
	return endDiameter;
}

void WorldBorder::setTransition(double newSize)
{
	startDiameter = newSize;
	endDiameter = newSize;
	endTime = Util::currentTimeMillis();
	startTime = endTime;

	for (auto iborderlistener : getListeners())
	{
		iborderlistener->onSizeChanged(*this, newSize);
	}
}

void WorldBorder::setTransition(double oldSize, double newSize, int64_t time)
{
	startDiameter = oldSize;
	endDiameter = newSize;
	startTime = Util::currentTimeMillis();
	endTime = startTime + time;

	for (auto iborderlistener : getListeners())
	{
		iborderlistener->onTransitionStarted(*this, oldSize, newSize, time);
	}
}

void WorldBorder::addListener(IBorderListener* listener)
{
	listeners.emplace_back(listener);
}

void WorldBorder::setSize(int32_t size)
{
	worldSize = size;
}

int32_t WorldBorder::getSize() const
{
	return worldSize;
}

double WorldBorder::getDamageBuffer() const
{
	return damageBuffer;
}

void WorldBorder::setDamageBuffer(double bufferSize)
{
	damageBuffer = bufferSize;

	for (auto iborderlistener : getListeners())
	{
		iborderlistener->onDamageBufferChanged(*this, bufferSize);
	}
}

double WorldBorder::getDamageAmount() const
{
	return damageAmount;
}

void WorldBorder::setDamageAmount(double newAmount)
{
	damageAmount = newAmount;

	for (auto iborderlistener : getListeners())
	{
		iborderlistener->onDamageAmountChanged(*this, newAmount);
	}
}

double WorldBorder::getResizeSpeed() const
{
	return endTime == startTime ? 0.0 : MathHelper::abs(startDiameter - endDiameter) / (double)(endTime - startTime);
}

int32_t WorldBorder::getWarningTime() const
{
	return warningTime;
}

void WorldBorder::setWarningTime(int32_t warningTime)
{
	warningTime = warningTime;

	for (auto iborderlistener : getListeners())
	{
		iborderlistener->onWarningTimeChanged(*this, warningTime);
	}
}

int32_t WorldBorder::getWarningDistance() const
{
	return warningDistance;
}

void WorldBorder::setWarningDistance(int32_t warningDistance)
{
	warningDistance = warningDistance;
	for (auto iborderlistener : getListeners())
	{
		iborderlistener->onWarningDistanceChanged(*this, warningDistance);
	}
}

std::vector<IBorderListener*> WorldBorder::getListeners() const
{
	return listeners;
}

int64_t WorldBorder::getTimeUntilTarget() const
{
	return getStatus() == EnumBorderStatus::STATIONARY ? 0L : endTime - Util::currentTimeMillis();
}



