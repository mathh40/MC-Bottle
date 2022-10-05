#pragma once
#include "Potion.h"

class PotionHealth : public Potion
{
  public:
    PotionHealth(bool isBadEffectIn, int32_t liquidColorIn);
    bool isInstant() const;
    bool isReady(int32_t duration, int32_t amplifier) const;

  private:
};
