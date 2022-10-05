#pragma once
class WorldBorder;

class IBorderListener
{
  public:
    virtual ~IBorderListener()                                 = default;
    virtual void onSizeChanged(WorldBorder &var1, double var2) = 0;

    virtual void onTransitionStarted(WorldBorder &var1, double var2, double var4, long var6) = 0;

    virtual void onCenterChanged(WorldBorder &var1, double var2, double var4) = 0;

    virtual void onWarningTimeChanged(WorldBorder &var1, int var2) = 0;

    virtual void onWarningDistanceChanged(WorldBorder &var1, int var2) = 0;

    virtual void onDamageAmountChanged(WorldBorder &var1, double var2 = 0);

    virtual void onDamageBufferChanged(WorldBorder &var1, double var2) = 0;
}
