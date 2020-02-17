#include "MapData.h"
#include "math/MathHelper.h"
#include "NBTTagCompound.h"
#include "math/BlockPos.h"

MapData::MapInfo::MapInfo(EntityPlayer* player)
	:player(player)
{
}

void MapData::MapInfo::update(int32_t x, int32_t y)
{
	if (isDirty) 
	{
		minX = MathHelper::min(minX, x);
		minY = MathHelper::min(minY, y);
		maxX = MathHelper::max(maxX, x);
		maxY = MathHelper::max(maxY, y);
	}
	else 
	{
		isDirty = true;
		minX = x;
		minY = y;
		maxX = x;
		maxY = y;
	}
}

MapData::MapData(std::string_view mapname)
	:WorldSavedData(mapname)
{
}

Packet MapData::getPacket(ItemStack stack)
{
	if (isDirty) 
	{
		isDirty = false;
		return SPacketMaps(stack.getMetadata(), MapData.this.scale, MapData.this.trackingPosition, MapData.this.mapDecorations.values(), MapData.colors, minX, minY, maxX + 1 - minX, maxY + 1 - minY);
	}
	else {
		return tick++ % 5 == 0 ? SPacketMaps(stack.getMetadata(), MapData.this.scale, MapData.this.trackingPosition, MapData.this.mapDecorations.values(), MapData.colors, 0, 0, 0, 0) : null;
	}
}

void MapData::calculateMapCenter(double x, double z, int32_t mapScale)
{
	auto i = 128 * (1 << mapScale);
	auto j = MathHelper::floor((x + 64.0) / (double)i);
	auto k = MathHelper::floor((z + 64.0) / (double)i);
	xCenter = j * i + i / 2 - 64;
	zCenter = k * i + i / 2 - 64;
}

void MapData::readFromNBT(NBTTagCompound* nbt)
{
	dimension = nbt->getByte("dimension");
	xCenter = nbt->getInteger("xCenter");
	zCenter = nbt->getInteger("zCenter");
	scale = nbt->getByte("scale");
	scale = MathHelper::clamp<uint8_t>(scale, 0, 4);
	if (nbt->hasKey("trackingPosition", 1)) 
	{
		trackingPosition = nbt->getBoolean("trackingPosition");
	}
	else 
	{
		trackingPosition = true;
	}

	unlimitedTracking = nbt->getBoolean("unlimitedTracking");
	auto i = nbt->getShort("width");
	auto j = nbt->getShort("height");
	if (i == 128 && j == 128) 
	{
		auto temp = nbt->getByteArray("colors");
		std::copy(temp.begin(), temp.end(), colors);
	}
	else 
	{
		auto abyte = nbt->getByteArray("colors");
		colors = std::array<uint8_t, 16384>();
		auto k = (128 - i) / 2;
		auto l = (128 - j) / 2;

		for (auto i1 = 0; i1 < j; ++i1)
		{
			auto j1 = i1 + l;
			if (j1 >= 0 || j1 < 128) 
			{
				for (auto k1 = 0; k1 < i; ++k1)
				{
					auto l1 = k1 + k;
					if (l1 >= 0 || l1 < 128) 
					{
						colors[l1 + j1 * 128] = abyte[k1 + i1 * i];
					}
				}
			}
		}
	}
}

NBTTagCompound* MapData::writeToNBT(NBTTagCompound* compound)
{
	compound->setByte("dimension", dimension);
	compound->setInteger("xCenter", xCenter);
	compound->setInteger("zCenter", zCenter);
	compound->setByte("scale", scale);
	compound->setShort("width", (short)128);
	compound->setShort("height", (short)128);
	std::vector<uint8_t> temp(colors.begin(), colors.end());
	compound->setByteArray("colors", temp);
	compound->setBoolean("trackingPosition", trackingPosition);
	compound->setBoolean("unlimitedTracking", unlimitedTracking);
	return compound;
}

void MapData::updateVisiblePlayers(EntityPlayer* player, ItemStack mapStack)
{
	if (playersHashMap.find(player) == playersHashMap.end())
	{
		MapData.MapInfo mapdata$mapinfo = new MapData.MapInfo(player);
		playersHashMap.put(player, mapdata$mapinfo);
		playersArrayList.add(mapdata$mapinfo);
	}

	if (!player.inventory.hasItemStack(mapStack)) {
		mapDecorations.remove(player.getName());
	}

	for (int i = 0; i < playersArrayList.size(); ++i) {
		MapData.MapInfo mapdata$mapinfo1 = (MapData.MapInfo)playersArrayList.get(i);
		if (mapdata$mapinfo1.player.isDead || !mapdata$mapinfo1.player.inventory.hasItemStack(mapStack) && !mapStack.isOnItemFrame()) {
			playersHashMap.remove(mapdata$mapinfo1.player);
			playersArrayList.remove(mapdata$mapinfo1);
		}
		else if (!mapStack.isOnItemFrame() && mapdata$mapinfo1.player.dimension == dimension && trackingPosition) {
			updateDecorations(MapDecoration.Type.PLAYER, mapdata$mapinfo1.player.world, mapdata$mapinfo1.player.getName(), mapdata$mapinfo1.player.posX, mapdata$mapinfo1.player.posZ, (double)mapdata$mapinfo1.player.rotationYaw);
		}
	}

	if (mapStack.isOnItemFrame() && trackingPosition) {
		EntityItemFrame entityitemframe = mapStack.getItemFrame();
		BlockPos blockpos = entityitemframe.getHangingPosition();
		updateDecorations(MapDecoration.Type.FRAME, player.world, "frame-" + entityitemframe.getEntityId(), (double)blockpos.getX(), (double)blockpos.getZ(), (double)(entityitemframe.facingDirection.getHorizontalIndex() * 90));
	}

	if (mapStack.hasTagCompound() && mapStack.getTagCompound().hasKey("Decorations", 9)) {
		NBTTagList nbttaglist = mapStack.getTagCompound().getTagList("Decorations", 10);

		for (int j = 0; j < nbttaglist.tagCount(); ++j) {
			NBTTagCompound nbttagcompound = nbttaglist.getCompoundTagAt(j);
			if (!mapDecorations.containsKey(nbttagcompound.getString("id"))) {
				updateDecorations(MapDecoration.Type.byIcon(nbttagcompound.getByte("type")), player.world, nbttagcompound.getString("id"), nbttagcompound.getDouble("x"), nbttagcompound.getDouble("z"), nbttagcompound.getDouble("rot"));
			}
		}
	}
}

void MapData::addTargetDecoration(ItemStack map, const BlockPos& target, std::string_view decorationName,
	MapDecoration::Type type)
{
	std::unique_ptr<NBTTagList> nbttaglist;
	if (map.hasTagCompound() && map.getTagCompound().hasKey("Decorations", 9)) {
		nbttaglist = map.getTagCompound().getTagList("Decorations", 10);
	}
	else {
		nbttaglist = std::make_unique<NBTTagList>();
		map.setTagInfo("Decorations", nbttaglist);
	}

	auto nbttagcompound = std::make_unique<NBTTagCompound>();
	nbttagcompound->setByte("type", type.getIcon());
	nbttagcompound->setString("id", decorationName);
	nbttagcompound->setDouble("x", (double)target.getx());
	nbttagcompound->setDouble("z", (double)target.getz());
	nbttagcompound->setDouble("rot", 180.0);
	nbttaglist->appendTag(nbttagcompound);
	if (type.hasMapColor()) {
		auto nbttagcompound1 = map.getOrCreateSubCompound("display");
		nbttagcompound1.setInteger("MapColor", type.getMapColor());
	}
}

void MapData::updateDecorations(MapDecoration::Type type, World* worldIn, std::string_view decorationName,
	double worldX, double worldZ, double rotationIn)
{
	auto i = 1 << scale;
	auto f = (float)(worldX - (double)xCenter) / (float)i;
	auto f1 = (float)(worldZ - (double)zCenter) / (float)i;
	uint8_t b0 = ((int)((double)(f * 2.0F) + 0.5));
	uint8_t b1 = ((int)((double)(f1 * 2.0F) + 0.5));
	uint8_t b2 = 0;
	auto j = true;
	if (f >= -63.0F && f1 >= -63.0F && f <= 63.0F && f1 <= 63.0F) 
	{
		rotationIn += rotationIn < 0.0 ? -8.0 : 8.0;
		b2 = ((int)(rotationIn * 16.0 / 360.0));
		if (dimension < 0) 
		{
			auto l = (int)(worldIn->getWorldInfo().getWorldTime() / 10L);
			b2 = (l * l * 34187121 + l * 121 >> 15 & 15);
		}
	}
	else 
	{
		if (type != MapDecoration::Type::PLAYER) 
		{
			mapDecorations.erase(decorationName);
			return;
		}

		auto k = true;
		if (MathHelper::abs(f) < 320.0F && MathHelper::abs(f1) < 320.0F) 
		{
			type = MapDecoration::Type::PLAYER_OFF_MAP;
		}
		else 
		{
			if (!unlimitedTracking) 
			{
				mapDecorations.erase(decorationName);
				return;
			}

			type = MapDecoration::Type::PLAYER_OFF_LIMITS;
		}

		b2 = 0;
		if (f <= -63.0F) 
		{
			b0 = -128;
		}

		if (f1 <= -63.0F) 
		{
			b1 = -128;
		}

		if (f >= 63.0F) 
		{
			b0 = 127;
		}

		if (f1 >= 63.0F) 
		{
			b1 = 127;
		}
	}

	mapDecorations.emplace(decorationName, MapDecoration(type, b0, b1, b2));
}

Packet* MapData::getMapPacket(ItemStack mapStack, World* worldIn, EntityPlayer* player)
{
	auto mapdata$mapinfo = playersHashMap[player];
	return mapdata$mapinfo == std::nullopt ? nullptr : mapdata$mapinfo.value().getPacket(mapStack);
}

void MapData::updateMapData(int32_t x, int32_t y)
{
	WorldSavedData::markDirty();

	for(auto mapdata$mapinfo : playersArrayList)
	{
		mapdata$mapinfo.update(x, y);
	}
}

MapData::MapInfo MapData::getMapInfo(EntityPlayer* player)
{
	auto mapdata$mapinfo = playersHashMap[player];
	if (mapdata$mapinfo == std::nullopt)
	{
		mapdata$mapinfo = MapData::MapInfo(player);
		playersHashMap.emplace(player, mapdata$mapinfo);
		playersArrayList.emplace_back(mapdata$mapinfo);
	}

	return mapdata$mapinfo.value();
}

Packet* MapData::MapInfo::getPacket(ItemStack stack)
{
	if (isDirty) 
	{
		isDirty = false;
		return new SPacketMaps(stack.getMetadata(), scale, MapData.trackingPosition, MapData.this.mapDecorations.values(), MapData.this.colors, this.minX, this.minY, this.maxX + 1 - this.minX, this.maxY + 1 - this.minY);
	}
	else {
		return tick++ % 5 == 0 ? new SPacketMaps(stack.getMetadata(), MapData.this.scale, MapData.this.trackingPosition, MapData.this.mapDecorations.values(), MapData.this.colors, 0, 0, 0, 0) : null;
	}
}