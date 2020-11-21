#include "PositionImpl.h"

PositionImpl::PositionImpl(double xCoord, double yCoord, double zCoord):
    x(xCoord), y(yCoord), z(zCoord) {
}

double PositionImpl::getx() {
    return x;
}

double PositionImpl::gety() {
    return y;
}

double PositionImpl::getz() {
    return z;
}
