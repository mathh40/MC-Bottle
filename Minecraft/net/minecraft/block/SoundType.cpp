#include "SoundType.h"

SoundType SoundType::WOOD(1.0F, 1.0F, SoundEvents::BLOCK_WOOD_BREAK, SoundEvents::BLOCK_WOOD_STEP, SoundEvents::BLOCK_WOOD_PLACE, SoundEvents::BLOCK_WOOD_HIT, SoundEvents::BLOCK_WOOD_FALL);
SoundType SoundType::GROUND(1.0F, 1.0F, SoundEvents::BLOCK_GRAVEL_BREAK, SoundEvents::BLOCK_GRAVEL_STEP, SoundEvents::BLOCK_GRAVEL_PLACE, SoundEvents::BLOCK_GRAVEL_HIT, SoundEvents::BLOCK_GRAVEL_FALL);
SoundType SoundType::PLANT();
SoundType SoundType::STONE();
SoundType SoundType::METAL();
SoundType SoundType::GLASS();
SoundType SoundType::CLOTH();
SoundType SoundType::SAND();
SoundType SoundType::SNOW();
SoundType SoundType::LADDER();
SoundType SoundType::ANVIL();
SoundType SoundType::SLIME();


SoundType::SoundType(float volumeIn, float pitchIn, SoundEvent& breakSoundIn, SoundEvent& stepSoundIn,
	SoundEvent& placeSoundIn, SoundEvent& hitSoundIn, SoundEvent& fallSoundIn)
	:volume(volumeIn), pitch(pitchIn), breakSound(breakSoundIn), stepSound(stepSoundIn)
	, placeSound(placeSoundIn), hitSound(hitSoundIn), fallSound(fallSoundIn)
{

}

float SoundType::getVolume() const
{
	return volume;
}

float SoundType::getPitch() const
{
	return pitch;
}

SoundEvent& SoundType::getBreakSound()
{
	return breakSound;
}

SoundEvent& SoundType::getStepSound()
{
	return stepSound;
}

SoundEvent& SoundType::getPlaceSound()
{
	return placeSound;
}

SoundEvent& SoundType::getHitSound()
{
	return hitSound;
}

SoundEvent& SoundType::getFallSound()
{
	return fallSound;
}
