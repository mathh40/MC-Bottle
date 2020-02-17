#pragma once
#include "../../../../crossguid/crossguid/guid.hpp"
#include "../util/text/ITextComponent.h"

class BossInfo
{
public:
	enum class Overlay
	{
		PROGRESS,
		NOTCHED_6,
		NOTCHED_10,
		NOTCHED_12,
		NOTCHED_20
	};

	enum class Color
	{
		PINK,
		BLUE,
		RED,
		GREEN,
		YELLOW,
		PURPLE,
		WHITE
	};

	BossInfo(xg::Guid uniqueIdIn, std::unique_ptr<ITextComponent> nameIn, Color colorIn, Overlay overlayIn);
	xg::Guid getUniqueId() const;
	ITextComponent* getName() const;
	virtual void setName(std::unique_ptr<ITextComponent> nameIn);
	float getPercent() const;
	virtual void setPercent(float percentIn);
	Color getColor() const;
	virtual void setColor(Color colorIn);
	Overlay getOverlay() const;
	virtual void setOverlay(Overlay overlayIn);
	bool shouldDarkenSky() const;
	virtual BossInfo& setDarkenSky(bool darkenSkyIn);
	bool shouldPlayEndBossMusic() const;
	virtual BossInfo& setPlayEndBossMusic(bool playEndBossMusicIn);
	virtual BossInfo& setCreateFog(bool createFogIn);
	bool shouldCreateFog() const;

protected:
	std::unique_ptr<ITextComponent> name;
	float percent;
	Color color;
	Overlay overlay;
	bool darkenSky;
	bool playEndBossMusic;
	bool createFog;
private:
	xg::Guid uniqueId;
};


