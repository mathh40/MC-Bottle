#pragma once
#include "IPosition.h"

class PositionImpl :public IPosition {
   public:
    PositionImpl(double xCoord, double yCoord, double zCoord);
    double getx() override;
    double gety() override;
    double getz() override;

    protected:
   double x;
   double y;
   double z;
};
