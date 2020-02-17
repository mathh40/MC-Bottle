#pragma once
#include <cstdint>

class DifficultyInstance
{
public:
	DifficultyInstance(EnumDifficulty worldDifficulty, int64_t worldTime, int64_t chunkInhabitedTime, float moonPhaseFactor);
	float getAdditionalDifficulty() const;
	bool isHarderThan(float p_193845_1_) const;
	float getClampedAdditionalDifficulty() const;
private:
	EnumDifficulty worldDifficulty;
	float additionalDifficulty;

	float calculateAdditionalDifficulty(EnumDifficulty difficulty, int64_t worldTime, int64_t chunkInhabitedTime, float moonPhaseFactor);
};
