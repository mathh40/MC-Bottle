#include "EntityJumpHelper.h"

EntityJumpHelper::EntityJumpHelper(EntityLiving *entityIn) : entity(entityIn)
{
}

void EntityJumpHelper::setJumping()
{
    isJumping = true;
}

void EntityJumpHelper::doJump()
{
    entity->setJumping(isJumping);
    isJumping = false;
}
