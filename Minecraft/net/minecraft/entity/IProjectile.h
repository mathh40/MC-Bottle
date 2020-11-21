#pragma once
class IProjectile
{
public:
    virtual ~IProjectile() = default;
    virtual void shoot(double var1, double var3, double var5, float var7, float var8) = 0;
};