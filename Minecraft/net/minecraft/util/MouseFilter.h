#pragma once

class MouseFilter
{
  public:
    float smooth(float p_76333_1_, float p_76333_2_);
    void reset();

  private:
    float targetValue    = 0.0F;
    float remainingValue = 0.0F;
    float lastAmount     = 0.0F;
};
