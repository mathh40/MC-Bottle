#pragma once
#include "ResourceLocation.h"
#include "registry/RegistryNamespaced.h"

class SoundEvent
{
public:
	static RegistryNamespaced< ResourceLocation, SoundEvent> REGISTRY;

	SoundEvent(ResourceLocation soundNameIn);
	ResourceLocation getSoundName() const;

	void registerSounds();

private:
	static int soundEventId;
	ResourceLocation soundName;

	void registerSound(std::string soundNameIn);
};
