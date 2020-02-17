#pragma once
#include "BossInfo.h"
#include <unordered_set>

class EntityPlayerMP;

class BossInfoServer : public BossInfo
{
public:
	BossInfoServer(std::unique_ptr<ITextComponent> nameIn, Color colorIn, Overlay overlayIn);
	void setPercent(float percentIn) override;
	void setColor(Color colorIn) override;
	void setOverlay(Overlay overlayIn) override;
	BossInfo& setDarkenSky(bool darkenSkyIn) override;
	BossInfo& setPlayEndBossMusic(bool playEndBossMusicIn) override;
	BossInfo& setCreateFog(bool createFogIn) override;
	void setName(std::unique_ptr<ITextComponent> nameIn) override;
	void addPlayer(EntityPlayerMP* player);
	void removePlayer(EntityPlayerMP* player);
	void setVisible(bool visibleIn);
	std::vector<EntityPlayerMP*> getPlayers();
private:
	std::unordered_set<EntityPlayerMP*> players;
	std::unordered_set<EntityPlayerMP*> readOnlyPlayers;
	bool visible;
	void sendUpdate(SPacketUpdateBossInfo.Operation operationIn);

};
