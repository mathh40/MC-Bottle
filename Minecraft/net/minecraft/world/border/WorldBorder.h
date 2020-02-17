#pragma once
#include <vector>
#include "EnumBorderStatus.h"
#include "IBorderListener.h"

class Entity;
class AxisAlignedBB;
class ChunkPos;
class BlockPos;

class WorldBorder
{
public:
	WorldBorder();
	bool contains(BlockPos& pos);
	bool contains(ChunkPos& range);
	bool contains(AxisAlignedBB& bb);
	double getClosestDistance(Entity* entityIn);
	double getClosestDistance(double x, double z);
	EnumBorderStatus getStatus() const;
	double minX();
	double minZ();
	double maxX();
	double maxZ();
	double getCenterX() const;
	double getCenterZ() const;
	void setCenter(double x, double z);
	double getDiameter();
	int64_t getTimeUntilTarget() const;
	double getTargetSize() const;
	void setTransition(double newSize);
	void setTransition(double oldSize, double newSize, int64_t time);
	void addListener(IBorderListener* listener);
	void setSize(int32_t size);
	int32_t getSize() const;
	double getDamageBuffer() const;
	void setDamageBuffer(double bufferSize);
	double getDamageAmount() const;
	void setDamageAmount(double newAmount);
	double getResizeSpeed() const;
	int32_t getWarningTime() const;
	void setWarningTime(int32_t warningTime);
	int32_t getWarningDistance() const;
	void setWarningDistance(int32_t warningDistance);

protected:
	std::vector<IBorderListener*> getListeners() const;
private:
	std::vector<IBorderListener*> listeners;
	double centerX;
	double centerZ;
	double startDiameter = 6.0E7;
	double endDiameter;
	int64_t endTime;
	int64_t startTime;
	int32_t worldSize;
	double damageAmount;
	double damageBuffer;
	int32_t warningTime;
	int32_t warningDistance;
};
