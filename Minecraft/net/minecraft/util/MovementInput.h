#pragma once

struct MovementInput
{
    float moveStrafe;
    float moveForward;
    bool forwardKeyDown;
    bool backKeyDown;
    bool leftKeyDown;
    bool rightKeyDown;
    bool jump;
    bool sneak;

    virtual void updatePlayerMoveState();
    Vec2f getMoveVector();
};
