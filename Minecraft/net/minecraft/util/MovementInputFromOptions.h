#pragma once
#include "MovementInput.h"

class MovementInputFromOptions : public MovementInput
{
  public:
    MovementInputFromOptions(GameSettings gameSettingsIn);
    void updatePlayerMoveState() override;

  private:
    GameSettings gameSettings;
};
