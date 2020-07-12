#pragma once
#include <cstdint>

class IJumpingMount
{
    virtual void setJumpPower(int32_t var1) = 0;

    virtual bool canJump() = 0;

    virtual void handleStartJump(int32_t var1) = 0;

    virtual void handleStopJump() = 0;
};
