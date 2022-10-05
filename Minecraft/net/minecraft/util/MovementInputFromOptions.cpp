#include "MovementInputFromOptions.h"

MovementInputFromOptions::MovementInputFromOptions(GameSettings gameSettingsIn) : gameSettings(gameSettingsIn)
{
}

void MovementInputFromOptions::updatePlayerMoveState()
{
    moveStrafe  = 0.0F;
    moveForward = 0.0F;
    if (gameSettings.keyBindForward.isKeyDown())
    {
        ++moveForward;
        forwardKeyDown = true;
    }
    else
    {
        forwardKeyDown = false;
    }

    if (gameSettings.keyBindBack.isKeyDown())
    {
        --moveForward;
        backKeyDown = true;
    }
    else
    {
        backKeyDown = false;
    }

    if (gameSettings.keyBindLeft.isKeyDown())
    {
        ++moveStrafe;
        leftKeyDown = true;
    }
    else
    {
        leftKeyDown = false;
    }

    if (gameSettings.keyBindRight.isKeyDown())
    {
        --moveStrafe;
        rightKeyDown = true;
    }
    else
    {
        rightKeyDown = false;
    }

    jump  = gameSettings.keyBindJump.isKeyDown();
    sneak = gameSettings.keyBindSneak.isKeyDown();
    if (sneak)
    {
        moveStrafe  = (float)((double)moveStrafe * 0.3);
        moveForward = (float)((double)moveForward * 0.3);
    }
}
