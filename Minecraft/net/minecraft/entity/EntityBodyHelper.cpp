#include "EntityBodyHelper.h"


#include "EntityLiving.h"
#include "EntityLivingBase.h"

EntityBodyHelper::EntityBodyHelper(EntityLivingBase *livingIn) {
    living = livingIn;
}

void EntityBodyHelper::updateRenderAngles() {
    double d0 = living->posX - living->prevPosX;
    double d1 = living->posZ - living->prevPosZ;
    if (d0 * d0 + d1 * d1 > 2.500000277905201E-7) {
        living->renderYawOffset = living->rotationYaw;
        living->rotationYawHead = computeAngleWithBound(living->renderYawOffset, living->rotationYawHead, 75.0F);
        prevRenderYawHead = living->rotationYawHead;
        rotationTickCounter = 0;
    } else if (living->getPassengers().empty() || !(Util::instanceof<EntityLiving>(living->getPassengers()[0]))) {
        float f = 75.0F;
        if (MathHelper::abs(living->rotationYawHead - prevRenderYawHead) > 15.0F) {
            rotationTickCounter = 0;
            prevRenderYawHead = living->rotationYawHead;
        } else {
            ++rotationTickCounter;
            int i = true;
            if (rotationTickCounter > 10) {
                f = MathHelper::max(1.0F - (float)(rotationTickCounter - 10) / 10.0F, 0.0F) * 75.0F;
            }
        }

        living->renderYawOffset = computeAngleWithBound(living->rotationYawHead, living->renderYawOffset, f);
    }
}

float EntityBodyHelper::computeAngleWithBound(float p_75665_1_, float p_75665_2_, float p_75665_3_) const {
    float f = MathHelper::wrapDegrees(p_75665_1_ - p_75665_2_);
    if (f < -p_75665_3_) {
        f = -p_75665_3_;
    }

    if (f >= p_75665_3_) {
        f = p_75665_3_;
    }

    return p_75665_1_ - f;
}
