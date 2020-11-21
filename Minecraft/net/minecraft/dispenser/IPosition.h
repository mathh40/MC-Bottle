#pragma once
class IPosition {
public:
    virtual ~IPosition() = default;
    virtual double getx() = 0;
   virtual double gety() = 0;
   virtual double getz() = 0;
};