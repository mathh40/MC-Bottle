#include "BossInfo.h"

BossInfo::BossInfo(xg::Guid uniqueIdIn, std::unique_ptr<ITextComponent> nameIn, Color colorIn, Overlay overlayIn)
	:uniqueId(uniqueIdIn), name(std::move(nameIn)), color(colorIn), overlay(overlayIn), percent(1.0f)
{

}

xg::Guid BossInfo::getUniqueId() const
{
	return uniqueId;
}

ITextComponent* BossInfo::getName() const
{
	return name.get();
}

void BossInfo::setName(std::unique_ptr<ITextComponent> nameIn)
{
	name = std::move(nameIn);
}

float BossInfo::getPercent() const
{
	return percent;
}

void BossInfo::setPercent(float percentIn)
{
	percent = percentIn;
}

Color BossInfo::getColor() const
{
	return color;
}

void BossInfo::setColor(Color colorIn)
{
	color = colorIn;
}

Overlay BossInfo::getOverlay() const
{
	return overlay;
}

void BossInfo::setOverlay(Overlay overlayIn)
{
	overlay = overlayIn;
}

bool BossInfo::shouldDarkenSky() const
{
	return darkenSky;
}

BossInfo& BossInfo::setDarkenSky(bool darkenSkyIn)
{
	darkenSky = darkenSkyIn;
	return *this;
}

bool BossInfo::shouldPlayEndBossMusic() const
{
	return playEndBossMusic;
}

BossInfo& BossInfo::setPlayEndBossMusic(bool playEndBossMusicIn)
{
	playEndBossMusic = playEndBossMusicIn;
	return *this;
}

BossInfo& BossInfo::setCreateFog(bool createFogIn)
{
	createFog = createFogIn;
	return *this;
}

bool BossInfo::shouldCreateFog() const
{
	return createFog;
}
