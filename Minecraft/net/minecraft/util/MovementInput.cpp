#include "MovementInput.h"

void MovementInput::updatePlayerMoveState()
{

}

Vec2f MovementInput::getMoveVector()
{
	return Vec2f(moveStrafe, moveForward);
}
