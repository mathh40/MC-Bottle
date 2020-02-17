#include "TileEntityStructure.h"
#include "ChatAllowedCharacters.h"
#include "StringUtils.h"
#include "../world/gen/structure/StructureBoundingBox.h"
#include "../world/gen/structure/StructureComponentTemplate.h"
#include "../world/WorldServer.h"
#include "text/TextComponentTranslation.h"

TileEntityStructure::Mode SAVE("save", 0);
TileEntityStructure::Mode LOAD("load", 1);
TileEntityStructure::Mode CORNER("corner", 2);
TileEntityStructure::Mode DATA("data", 3);


std::string TileEntityStructure::Mode::getName() const
{
	return modeName;
}

int32_t TileEntityStructure::Mode::getModeId() const
{
	return modeId;
}

TileEntityStructure::Mode* TileEntityStructure::Mode::getById(int32_t id)
{
	return id >= 0 && id < MODES.size() ? MODES[id] : MODES[0];
}

TileEntityStructure::Mode::Mode(std::string modeNameIn, int32_t modeIdIn)
	:modeName(modeNameIn),modeId(modeIdIn)
{
	MODES.push_back(this);
}

TileEntityStructure::TileEntityStructure()
	: size(BlockPos::ORIGIN),mirror(Mirror::NONE), rotation(Rotation::NONE)
	  ,mode(TileEntityStructure::Mode::DATA), ignoreEntities(true), showBoundingBox(true), integrity(1.0F)
{
}

NBTTagCompound* TileEntityStructure::writeToNBT(NBTTagCompound* compound)
{
	TileEntity::writeToNBT(compound);
	compound->setString("name", name);
	compound->setString("author", author);
	compound->setString("metadata", metadata);
	compound->setInteger("posX", position.getx());
	compound->setInteger("posY", position.gety());
	compound->setInteger("posZ", position.getz());
	compound->setInteger("sizeX", size.getx());
	compound->setInteger("sizeY", size.gety());
	compound->setInteger("sizeZ", size.getz());
	compound->setString("rotation", rotation.toString());
	compound->setString("mirror", mirror.toString());
	compound->setString("mode", mode.toString());
	compound->setBoolean("ignoreEntities", ignoreEntities);
	compound->setBoolean("powered", powered);
	compound->setBoolean("showair", showAir);
	compound->setBoolean("showboundingbox", showBoundingBox);
	compound->setFloat("integrity", integrity);
	compound->setLong("seed", seed);
	return compound;
}

void TileEntityStructure::readFromNBT(NBTTagCompound* compound)
{
	TileEntity::readFromNBT(compound);
	setName(compound->getString("name"));
	author = compound->getString("author");
	metadata = compound->getString("metadata");
	auto i = MathHelper::clamp(compound->getInteger("posX"), -32, 32);
	auto j = MathHelper::clamp(compound->getInteger("posY"), -32, 32);
	auto k = MathHelper::clamp(compound->getInteger("posZ"), -32, 32);
	position = BlockPos(i, j, k);
	auto l = MathHelper::clamp(compound->getInteger("sizeX"), 0, 32);
	auto i1 = MathHelper::clamp(compound->getInteger("sizeY"), 0, 32);
	auto j1 = MathHelper::clamp(compound->getInteger("sizeZ"), 0, 32);
	size = BlockPos(l, i1, j1);

	try 
	{
		rotation = Rotation::valueOf(compound->getString("rotation"));
	}
	catch (IllegalArgumentException var11) 
	{
		rotation = Rotation::NONE;
	}

	try 
	{
		mirror = Mirror::valueOf(compound->getString("mirror"));
	}
	catch (IllegalArgumentException var10) 
	{
		mirror = Mirror::NONE;
	}

	try 
	{
		mode = TileEntityStructure::Mode::valueOf(compound->getString("mode"));
	}
	catch (IllegalArgumentException var9) 
	{
		mode = TileEntityStructure::Mode::DATA;
	}

	ignoreEntities = compound->getBoolean("ignoreEntities");
	powered = compound->getBoolean("powered");
	showAir = compound->getBoolean("showair");
	showBoundingBox = compound->getBoolean("showboundingbox");
	if (compound->hasKey("integrity")) {
		integrity = compound->getFloat("integrity");
	}
	else 
	{
		integrity = 1.0F;
	}

	seed = compound->getLong("seed");
	updateBlockState();
}

std::optional<SPacketUpdateTileEntity> TileEntityStructure::getUpdatePacket()
{
	return SPacketUpdateTileEntity(pos, 7, getUpdateTag());
}

NBTTagCompound* TileEntityStructure::getUpdateTag()
{
	return writeToNBT(new NBTTagCompound());
}

bool TileEntityStructure::usedBy(EntityPlayer* player)
{
	if (!player->canUseCommandBlock()) 
	{
		return false;
	}
	else 
	{
		if (player->getEntityWorld().isRemote) 
		{
			player->openEditStructure(this);
		}

		return true;
	}
}

std::string TileEntityStructure::getName() const
{
	return name;
}

void TileEntityStructure::setName(std::string_view nameIn)
{
	auto s = nameIn;
	auto var3 = ChatAllowedCharacters::ILLEGAL_STRUCTURE_CHARACTERS;
	auto var4 = var3.size();

	for(auto var5 = 0; var5 < var4; ++var5) 
	{
		char c0 = var3[var5];
		std::replace(s.begin(),s.end(),c0, '_');
	}

	name = s;
}

void TileEntityStructure::createdBy(EntityLivingBase* p_189720_1_)
{
	if (!StringUtils::isNullOrEmpty(p_189720_1_->getName())) 
	{
		author = p_189720_1_->getName();
	}
}

BlockPos TileEntityStructure::getPosition() const
{
	return position;
}

void TileEntityStructure::setPosition(BlockPos posIn)
{
	position = posIn;
}

BlockPos TileEntityStructure::getStructureSize() const
{
	return size;
}

void TileEntityStructure::setSize(BlockPos sizeIn)
{
	size = sizeIn;
}

Mirror TileEntityStructure::getMirror() const
{
	return mirror;
}

void TileEntityStructure::setMirror(Mirror mirrorIn)
{
	mirror = mirrorIn;
}

Rotation TileEntityStructure::getRotation() const
{
	return rotation;
}

void TileEntityStructure::setRotation(Rotation rotationIn)
{
	rotation = rotationIn;
}

std::string TileEntityStructure::getMetadata() const
{
	return metadata;
}

void TileEntityStructure::setMetadata(std::string metadataIn)
{
	metadata = metadataIn;
}

TileEntityStructure::Mode TileEntityStructure::getMode()
{
	return mode;
}

void TileEntityStructure::setMode(TileEntityStructure::Mode modeIn)
{
	mode = modeIn;
	IBlockState* iblockstate = world->getBlockState(getPos());
	if (iblockstate->getBlock() == Blocks::STRUCTURE_BLOCK) 
	{
		world->setBlockState(getPos(), iblockstate->withProperty(BlockStructure::MODE, modeIn), 2);
	}
}

void TileEntityStructure::nextMode() const
{
	if(getMode() == Mode::SAVE)
	{
		setMode(TileEntityStructure::Mode::LOAD);
	}
	if(getMode() == Mode::LOAD)
	{
		setMode(TileEntityStructure::Mode::CORNER);
	}
	if(getMode() == Mode::CORNER)
	{
		setMode(TileEntityStructure::Mode::DATA);
	}
	if(getMode() == Mode::DATA)
	{
		setMode(TileEntityStructure::Mode::SAVE);
	}
}

bool TileEntityStructure::ignoresEntities() const
{
	return ignoreEntities;
}

void TileEntityStructure::setIgnoresEntities(bool ignoreEntitiesIn)
{
	ignoreEntities = ignoreEntitiesIn;
}

float TileEntityStructure::getIntegrity() const
{
	return integrity;
}

void TileEntityStructure::setIntegrity(float integrityIn)
{
	integrity = integrityIn;
}

int64_t TileEntityStructure::getSeed() const
{
	return seed;
}

void TileEntityStructure::setSeed(int64_t seedIn)
{
	seed = seedIn;
}

bool TileEntityStructure::detectSize()
{
	if (mode != TileEntityStructure::Mode::SAVE) 
	{
		return false;
	}
	else 
	{
		BlockPos blockpos = getPos();
		BlockPos blockpos1 = BlockPos(blockpos.getx() - 80, 0, blockpos.getz() - 80);
		BlockPos blockpos2 = BlockPos(blockpos.getx() + 80, 255, blockpos.getz() + 80);
		auto list = getNearbyCornerBlocks(blockpos1, blockpos2);
		auto list1 = filterRelatedCornerBlocks(list);
		if (list1.size() < 1) 
		{
			return false;
		}
		else 
		{
			StructureBoundingBox structureboundingbox = calculateEnclosingBoundingBox(blockpos, list1);
			if (structureboundingbox.maxX - structureboundingbox.minX > 1 && structureboundingbox.maxY - structureboundingbox.minY > 1 && structureboundingbox.maxZ - structureboundingbox.minZ > 1) 
			{
				position = BlockPos(structureboundingbox.minX - blockpos.getx() + 1, structureboundingbox.minY - blockpos.gety() + 1, structureboundingbox.minZ - blockpos.getz() + 1);
				size = BlockPos(structureboundingbox.maxX() - structureboundingbox.minX - 1, structureboundingbox.maxY - structureboundingbox.minY - 1, structureboundingbox.maxZ - structureboundingbox.minZ - 1);
				markDirty();
				IBlockState* iblockstate = world->getBlockState(blockpos);
				world->notifyBlockUpdate(blockpos, iblockstate, iblockstate, 3);
				return true;
			}
			else 
			{
				return false;
			}
		}
	}
}

void TileEntityStructure::updateBlockState()
{
	if (world != nullptr) 
	{
		BlockPos blockpos = getPos();
		IBlockState* iblockstate = world->getBlockState(blockpos);
		if (iblockstate->getBlock() == Blocks::STRUCTURE_BLOCK) {
			world->setBlockState(blockpos, iblockstate->withProperty(BlockStructure::MODE, mode), 2);
		}
	}
}

std::vector<TileEntityStructure*> TileEntityStructure::filterRelatedCornerBlocks(std::vector<TileEntityStructure*> p_184415_1_)
{
	std::vector<TileEntityStructure*> temp;
	std::copy_if(p_184415_1_.begin(),p_184415_1_.end(),std::back_inserter(temp),[=](TileEntityStructure* p_apply_1_){return p_apply_1_->mode == TileEntityStructure::Mode::CORNER && name ==p_apply_1_->name;});
	return temp;
}

std::vector<TileEntityStructure*> TileEntityStructure::getNearbyCornerBlocks(BlockPos p_184418_1_, BlockPos p_184418_2_)
{
	std::vector<TileEntityStructure*> list;
	Iterator var4 = BlockPos::getAllInBoxMutable(p_184418_1_, p_184418_2_).iterator();

	while(var4.hasNext()) 
	{
		MutableBlockPos blockpos$mutableblockpos = (MutableBlockPos)var4.next();
		IBlockState* iblockstate = world->getBlockState(blockpos$mutableblockpos);
		if (iblockstate->getBlock() == Blocks::STRUCTURE_BLOCK) 
		{
			TileEntity* tileentity = world->getTileEntity(blockpos$mutableblockpos);
			if (tileentity != nullptr && Util::instanceof< TileEntityStructure>(tileentity)) 
			{
				list.push_back((TileEntityStructure*)tileentity);
			}
		}
	}

	return list;
}

StructureBoundingBox TileEntityStructure::calculateEnclosingBoundingBox(BlockPos p_184416_1_,
	std::vector<TileEntityStructure*> p_184416_2_)
{
	StructureBoundingBox structureboundingbox;
	if (p_184416_2_.size() > 1) 
	{
		BlockPos blockpos = p_184416_2_[0]->getPos();
		structureboundingbox = StructureBoundingBox(blockpos, blockpos);
	}
	else 
	{
		structureboundingbox = StructureBoundingBox(p_184416_1_, p_184416_1_);
	}

	for(auto tileentitystructure: p_184416_2_)
	{
		BlockPos blockpos1 = tileentitystructure->getPos();
		if (blockpos1.getx() < structureboundingbox.minX) 
		{
			structureboundingbox.minX = blockpos1.getx();
		}
		else if (blockpos1.getx() > structureboundingbox.maxX) 
		{
			structureboundingbox.maxX = blockpos1.getx();
		}

		if (blockpos1.gety() < structureboundingbox.minY) 
		{
			structureboundingbox.minY = blockpos1.gety();
		}
		else if (blockpos1.gety() > structureboundingbox.maxY) 
		{
			structureboundingbox.maxY = blockpos1.gety();
		}

		if (blockpos1.getz() < structureboundingbox.minZ) 
		{
			structureboundingbox.minZ = blockpos1.getz();
		}
		else if (blockpos1.getz() > structureboundingbox.maxZ) 
		{
			structureboundingbox.maxZ = blockpos1.getz();
		}
	}

	return structureboundingbox;
}

bool TileEntityStructure::save(boolean writeToDisk)
{
	if (mode == TileEntityStructure::Mode::SAVE && !world->isRemote && !StringUtils::isNullOrEmpty(name)) 
	{
		BlockPos blockpos = getPos().add(position);
		WorldServer* worldserver = (WorldServer*)world;
		MinecraftServer* minecraftserver = world->getMinecraftServer();
		TemplateManager templatemanager = worldserver->getStructureTemplateManager();
		Template template = templatemanager.getTemplate(minecraftserver, ResourceLocation(name));
		template.takeBlocksFromWorld(world, blockpos, size, !ignoreEntities, Blocks::STRUCTURE_VOID);
		template.setAuthor(author);
		return !writeToDisk || templatemanager.writeTemplate(minecraftserver, ResourceLocation(name));
	}
	else 
	{
		return false;
	}
}

bool TileEntityStructure::load(bool requireMatchingSize)
{
	if (mode == TileEntityStructure::Mode::LOAD && !world->isRemote && !StringUtils::isNullOrEmpty(name)) 
	{
		BlockPos blockpos = getPos();
		BlockPos blockpos1 = blockpos.add(position);
		WorldServer* worldserver = (WorldServer*)world;
		MinecraftServer minecraftserver = world->getMinecraftServer();
		TemplateManager templatemanager = worldserver->getStructureTemplateManager();
		Template template = templatemanager.get(minecraftserver, ResourceLocation(name));
		if (template == nullptr) 
		{
			return false;
		}
		else 
		{
			if (!StringUtils::isNullOrEmpty(template.getAuthor())) 
			{
				author = template.getAuthor();
			}

			BlockPos blockpos2 = template.getSize();
			boolean flag = size.equals(blockpos2);
			if (!flag) 
			{
				size = blockpos2;
				markDirty();
				IBlockState* iblockstate = world->getBlockState(blockpos);
				world->notifyBlockUpdate(blockpos, iblockstate, iblockstate, 3);
			}

			if (requireMatchingSize && !flag) {
				return false;
			}
			else 
			{
				PlacementSettings placementsettings = (PlacementSettings()).setMirror(mirror).setRotation(rotation).setIgnoreEntities(ignoreEntities).setChunk((ChunkPos)nullptr).setReplacedBlock((Block)nullptr).setIgnoreStructureBlock(false);
				if (integrity < 1.0F) 
				{
					placementsettings.setIntegrity(MathHelper::clamp(integrity, 0.0F, 1.0F)).setSeed(seed);
				}

				template.addBlocksToWorldChunk(world, blockpos1, placementsettings);
				return true;
			}
		}
	}
	else 
	{
		return false;
	}
}

void TileEntityStructure::writeCoordinates(ByteBuf buf)
{
	buf.writeInt(pos.getx());
	buf.writeInt(pos.gety());
	buf.writeInt(pos.getz());
}

bool TileEntityStructure::save()
{
	return save(true);
}

bool TileEntityStructure::load()
{
	return load(true);
}

void TileEntityStructure::unloadStructure()
{
	WorldServer* worldserver = (WorldServer*)world;
	TemplateManager templatemanager = worldserver->getStructureTemplateManager();
	templatemanager.remove(ResourceLocation(name));
}

bool TileEntityStructure::isStructureLoadable()
{
	if (mode == TileEntityStructure::Mode::LOAD && !world->isRemote) 
	{
		WorldServer* worldserver = (WorldServer*)world;
		MinecraftServer minecraftserver = world->getMinecraftServer();
		TemplateManager templatemanager = worldserver.getStructureTemplateManager();
		return templatemanager.get(minecraftserver, ResourceLocation(name)) != nullptr;
	}
	else 
	{
		return false;
	}
}

bool TileEntityStructure::isPowered() const
{
	return powered;
}

void TileEntityStructure::setPowered(bool poweredIn)
{
	powered = poweredIn;
}

bool TileEntityStructure::showsAir() const
{
	return showAir;
}

void TileEntityStructure::setShowAir(bool showAirIn)
{
	showAir = showAirIn;
}

bool TileEntityStructure::showsBoundingBox() const
{
	return showBoundingBox;
}

void TileEntityStructure::setShowBoundingBox(bool showBoundingBoxIn)
{
	showBoundingBox = showBoundingBoxIn;
}

ITextComponent* TileEntityStructure::getDisplayName()
{
	return TextComponentTranslation("structure_block.hover." + mode->modeName, new Object[]{mode == TileEntityStructure::Mode::DATA ? metadata : name});
}
