#include "DifficultyInstance.h"

#include "../util/math/MathHelper.h"

DifficultyInstance::DifficultyInstance(EnumDifficulty worldDifficulty, int64_t worldTime, int64_t chunkInhabitedTime,
                                       float moonPhaseFactor)
    : worldDifficulty(worldDifficulty), additionalDifficulty(calculateAdditionalDifficulty(
                                            worldDifficulty, worldTime, chunkInhabitedTime, moonPhaseFactor))
{
}

float DifficultyInstance::getAdditionalDifficulty() const
{
    return additionalDifficulty;
}

bool DifficultyInstance::isHarderThan(float p_193845_1_) const
{
    return additionalDifficulty > p_193845_1_;
}

float DifficultyInstance::getClampedAdditionalDifficulty() const
{
    if (additionalDifficulty < 2.0F)
    {
        return 0.0F;
    }
    else
    {
        return additionalDifficulty > 4.0F ? 1.0F : (additionalDifficulty - 2.0F) / 2.0F;
    }
}

float DifficultyInstance::calculateAdditionalDifficulty(EnumDifficulty difficulty, int64_t worldTime,
                                                        int64_t chunkInhabitedTime, float moonPhaseFactor)
{
    if (difficulty == EnumDifficulty::PEACEFUL)
    {
        return 0.0F;
    }
    else
    {
        bool flag = difficulty == EnumDifficulty::HARD;
        float f   = 0.75F;
        float f1  = MathHelper::clamp((worldTime + -72000.0F) / 1440000.0F, 0.0F, 1.0F) * 0.25F;
        f += f1;
        float f2 = 0.0F;
        f2 += MathHelper::clamp(chunkInhabitedTime / 3600000.0F, 0.0F, 1.0F) * (flag ? 1.0F : 0.75F);
        f2 += MathHelper::clamp(moonPhaseFactor * 0.25F, 0.0F, f1);
        if (difficulty == EnumDifficulty::EASY)
        {
            f2 *= 0.5F;
        }

        f += f2;
        return (float)difficulty.getId() * f;
    }
}
