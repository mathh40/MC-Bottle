#include "StructureEndCityPieces.h"
#include "gen/structure/MapGenStructureIO.h"
#include "ResourceLocation.h"
#include "World.h"
#include "state/BlockPistonStructureHelper.h"
#include <utility>


const PlacementSettings StructureEndCityPieces::INSERT = (PlacementSettings()).setIgnoreEntities(true).setReplacedBlock(Blocks::AIR);

StructureEndCityPieces::CityTemplate::CityTemplate(TemplateManager& p_i47214_1_, std::string_view p_i47214_2_,
	BlockPos& p_i47214_3_, Rotation p_i47214_4_, bool overwriteIn)
	:StructureComponentTemplate(0),pieceName(p_i47214_2_),templatePosition(p_i47214_3_),rotation(std::move(p_i47214_4_)),overwrite(overwriteIn),loadTemplate(p_i47214_1_)
{
	
}

void StructureEndCityPieces::registerPieces()
{
	MapGenStructureIO::registerStructureComponent(CityTemplate.class, "ECP");
}

StructureEndCityPieces::CityTemplate StructureEndCityPieces::addPiece(TemplateManager& p_191090_0_, StructureEndCityPieces::CityTemplate& p_191090_1_, BlockPos& p_191090_2_, std::string_view p_191090_3_, Rotation p_191090_4_, bool owerwrite)
{
	StructureEndCityPieces::CityTemplate structureendcitypieces$citytemplate = CityTemplate(p_191090_0_, p_191090_3_, p_191090_1_.templatePosition, p_191090_4_, owerwrite);
	MutableBlockPos blockpos = p_191090_1_.templates.calculateConnectedPos(p_191090_1_.placeSettings, p_191090_2_,
	                                                                structureendcitypieces$citytemplate.placeSettings,
	                                                                BlockPos::ORIGIN);
	structureendcitypieces$citytemplate.offset(blockpos.getx(), blockpos.gety(), blockpos.getz());
	return structureendcitypieces$citytemplate;
}

void StructureEndCityPieces::startHouseTower(TemplateManager& p_191087_0_, BlockPos& p_191087_1_, Rotation p_191087_2_, std::vector<StructureEndCityPieces::CityTemplate>& p_191087_3_, pcg32& p_191087_4_)
{
	FAT_TOWER_GENERATOR->init();
	HOUSE_TOWER_GENERATOR->init();
	TOWER_BRIDGE_GENERATOR->init();
	TOWER_GENERATOR->init();
	CityTemplate structureendcitypieces$citytemplate = addHelper(p_191087_3_, StructureEndCityPieces::CityTemplate(p_191087_0_, "base_floor", p_191087_1_, p_191087_2_, true));
	structureendcitypieces$citytemplate = addHelper(p_191087_3_, addPiece(p_191087_0_, structureendcitypieces$citytemplate, BlockPos(-1, 0, -1), "second_floor", p_191087_2_, false));
	structureendcitypieces$citytemplate = addHelper(p_191087_3_, addPiece(p_191087_0_, structureendcitypieces$citytemplate, BlockPos(-1, 4, -1), "third_floor", p_191087_2_, false));
	structureendcitypieces$citytemplate = addHelper(p_191087_3_, addPiece(p_191087_0_, structureendcitypieces$citytemplate, BlockPos(-1, 8, -1), "third_roof", p_191087_2_, true));
	recursiveChildren(p_191087_0_, TOWER_GENERATOR, 1, structureendcitypieces$citytemplate, (BlockPos)nullptr, p_191087_3_, p_191087_4_);
}

CityTemplate StructureEndCityPieces::addHelper(std::vector<CityTemplate>& p_189935_0_, CityTemplate p_189935_1_)
{
	p_189935_0_.emplace_back(p_189935_1_);
	return p_189935_1_;
}

bool StructureEndCityPieces::recursiveChildren(TemplateManager& p_191088_0_, StructureEndCityPieces::IGenerator& p_191088_1_, int32_t p_191088_2_, StructureEndCityPieces::CityTemplate p_191088_3_, BlockPos& p_191088_4_, std::vector<> p_191088_5_, pcg32& p_191088_6_)
{
	if (p_191088_2_ > 8) 
	{
		return false;
	}
	else 
	{
		std::vector<StructureComponent*> list;
		if (p_191088_1_.generate(p_191088_0_, p_191088_2_, p_191088_3_, p_191088_4_, list, p_191088_6_)) 
		{
			bool flag = false;
			auto i = p_191088_6_();
#
			for(auto& structurecomponent: list)
			{
				structurecomponent->componentType = i;
				auto structurecomponent1 = StructureComponent.findIntersecting(p_191088_5_, structurecomponent->getBoundingBox());
				if (structurecomponent1 != nullptr && structurecomponent1->componentType != p_191088_3_->componentType) 
				{
					flag = true;
					break;
				}
			}

			if (!flag) 
			{
				p_191088_5_.addAll(list);
				return true;
			}
		}

		return false;
	}
}


void StructureEndCityPieces::CityTemplate::loadTemplate(const TemplateManager& p_191085_1_)
{
	Template templates = p_191085_1_.getTemplate((MinecraftServer)nullptr, ResourceLocation("endcity/" + pieceName));
	PlacementSettings placementsettings = (overwrite ? StructureEndCityPieces::OVERWRITE : StructureEndCityPieces::INSERT).copy().setRotation(rotation);
	setup(templates, templatePosition, placementsettings);
}

void StructureEndCityPieces::CityTemplate::writeStructureToNBT(NBTTagCompound* tagCompound)
{
	StructureComponentTemplate::writeStructureToNBT(tagCompound);
	tagCompound->setString("Template", pieceName);
	tagCompound->setString("Rot", rotation.getname());
	tagCompound->setBoolean("OW", overwrite);
}

void StructureEndCityPieces::CityTemplate::readStructureFromNBT(NBTTagCompound* tagCompound,
	const TemplateManager& p_143011_2_)
{
	StructureComponentTemplate::readStructureFromNBT(tagCompound, p_143011_2_);
	pieceName = tagCompound->getString("Template");
	rotation = Rotation::valueOf(tagCompound->getString("Rot"));
	overwrite = tagCompound->getBoolean("OW");
	loadTemplate(p_143011_2_);
}

void StructureEndCityPieces::CityTemplate::handleDataMarker(std::string_view function, BlockPos& pos, World* worldIn,
	pcg32 rand, StructureBoundingBox sbb)
{
	if(Util::startwith("Chest",function))
	{
		BlockPos blockpos = pos.down();
		if (sbb.isVecInside(blockpos)) 
		{
			TileEntity* tileentity = worldIn->getTileEntity(blockpos);
			if (Util::instanceof<TileEntityChest>(tileentity)) 
			{
				((TileEntityChest)tileentity).setLootTable(LootTableList::CHESTS_END_CITY_TREASURE, MathHelper::nextLong(rand));
			}
		}
	}
	else if(Util::startwith("Sentry",function))
	{
		EntityShulker entityshulker = EntityShulker(worldIn);
		entityshulker.setPosition(pos.getx() + 0.5, pos.gety() + 0.5, pos.getz() + 0.5);
		entityshulker.setAttachmentPos(pos);
		worldIn->spawnEntity(entityshulker);
	}
	else if(Util::startwith("Elytra",function))
	{
		EntityItemFrame entityitemframe = EntityItemFrame(worldIn, pos, rotation.rotate(EnumFacing::SOUTH));
		entityitemframe.setDisplayedItem(ItemStack(Items::ELYTRA));
		worldIn->spawnEntity(entityitemframe);
	}
}
