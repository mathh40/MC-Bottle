#pragma once
#include "../util/SoundEvent.h"

class SoundType
{
public: 
	float volume;
	float pitch;
	static SoundType WOOD;
	static SoundType GROUND;
	static SoundType PLANT;
	static SoundType STONE;
	static SoundType METAL;
	static SoundType GLASS;
	static SoundType CLOTH;
	static SoundType SAND;
	static SoundType SNOW;
	static SoundType LADDER;
	static SoundType ANVIL;
	static SoundType SLIME;

	SoundType(float volumeIn, float pitchIn, SoundEvent& breakSoundIn, SoundEvent& stepSoundIn, SoundEvent& placeSoundIn, SoundEvent& hitSoundIn, SoundEvent& fallSoundIn);
	float getVolume() const;
	float getPitch()const;
	SoundEvent& getBreakSound();
	SoundEvent& getStepSound();
	SoundEvent& getPlaceSound();
	SoundEvent& getHitSound();
	SoundEvent& getFallSound();
private: 
	SoundEvent breakSound;
	SoundEvent stepSound;
	SoundEvent placeSound;
	SoundEvent hitSound;
	SoundEvent fallSound;
};
