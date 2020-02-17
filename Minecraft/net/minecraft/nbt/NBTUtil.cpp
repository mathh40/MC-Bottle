#include "NBTUtil.h"
#include <memory>
#include "spdlog/spdlog.h"
#include "crossguid/guid.hpp"
#include "../util/ResourceLocation.h"
#include "../block/Block.h"
#include "../block/state/BlockStateContainer.h"

std::shared_ptr<spdlog::logger> LOGGER = spdlog::get("Minecraft")->clone("NBTUtil");

std::optional<GameProfile> NBTUtil::readGameProfileFromNBT(NBTTagCompound compound)
{
	std::string s;
	std::string s1;
	if (compound.hasKey("Name", 8)) {
		s = compound.getString("Name");
	}

	if (compound.hasKey("Id", 8)) {
		s1 = compound.getString("Id");
	}
	xg::Guid uuid(s1);

	GameProfile gameprofile = GameProfile(uuid, s);
	if (compound.hasKey("Properties", 10)) {
		auto nbttagcompound = compound.getCompoundTag("Properties");

		for(auto s2 : nbttagcompound->getCompoundMap()){
			auto nbttaglist = nbttagcompound->getTagList(s2.first, 10);

			for (int i = 0; i < nbttaglist->tagCount(); ++i) {
				auto nbttagcompound1 = nbttaglist->getCompoundTagAt(i);
				auto s3 = nbttagcompound1->getString("Value");
				if (nbttagcompound1->hasKey("Signature", 8)) {
					gameprofile.getProperties().put(s2, new Property(s2, s3, nbttagcompound1->getString("Signature")));
				}
				else {
					gameprofile.getProperties().put(s2, new Property(s2, s3));
				}
			}
		}
	}

	return gameprofile;
}
std::shared_ptr<NBTTagCompound> NBTUtil::writeGameProfile(NBTTagCompound tagCompound, GameProfile profile)
{
	if (!profile.getName().empty()) {
		tagCompound.setString("Name", profile.getName());
	}

	if (profile.getId() != 0) {
		tagCompound.setString("Id", profile.getId().toString());
	}

	if (!profile.getProperties().isEmpty()) {
		auto nbttagcompound = std::make_shared<NBTTagCompound>();
		Iterator var3 = profile.getProperties().keySet().iterator();

		while (var3.hasNext()) {
			String s = (String)var3.next();
			auto nbttaglist = std::make_shared <NBTTagList>();

			NBTTagCompound nbttagcompound1;
			for (Iterator var6 = profile.getProperties().get(s).iterator(); var6.hasNext(); nbttaglist.appendTag(nbttagcompound1)) {
				Property property = (Property)var6.next();
				nbttagcompound1 = std::make_shared<NBTTagCompound>();
				nbttagcompound1.setString("Value", property.getValue());
				if (property.hasSignature()) {
					nbttagcompound1.setString("Signature", property.getSignature());
				}
			}

			nbttagcompound.setTag(s, nbttaglist);
		}

		tagCompound.setTag("Properties", nbttagcompound);
	}

	return tagCompound;
}

std::shared_ptr<NBTTagCompound> NBTUtil::createUUIDTag(xg::Guid uuid)
{
	auto nbttagcompound = std::make_shared<NBTTagCompound>();

	auto bytes = uuid.bytes();
	const char most[] = { bytes[0],bytes[1],bytes[2],bytes[3],bytes[4],bytes[5],bytes[6],bytes[7] };
	const char least[] = { bytes[8],bytes[9],bytes[10],bytes[11],bytes[12],bytes[13],bytes[14],bytes[15] };

	nbttagcompound->setLong("M", reinterpret_cast<int64_t>(&most));
	nbttagcompound->setLong("L", reinterpret_cast<int64_t>(&least));
	return nbttagcompound;
}

xg::Guid NBTUtil::getUUIDFromTag(std::shared_ptr<NBTTagCompound> tag)
{
	auto m = tag->getLong("M");
	auto l = tag->getLong("L");
	const auto most = reinterpret_cast<char*>(m);
	const auto least = reinterpret_cast<char*>(l);
	const char value[] = { most[0],most[1],most[2],most[3],most[4],most[5],most[6],most[7] ,least[0],least[1],least[2],least[3],least[4],least[5],least[6],least[7] };
	return xg::Guid(value);
}

BlockPos NBTUtil::getPosFromTag(std::shared_ptr<NBTTagCompound> tag)
{
	return BlockPos(tag->getInteger("X"), tag->getInteger("Y"), tag->getInteger("Z"));
}

std::shared_ptr<NBTTagCompound> NBTUtil::createPosTag(const BlockPos &pos)
{
	auto nbttagcompound = std::make_shared<NBTTagCompound>();
	nbttagcompound->setInteger("X", pos.getx());
	nbttagcompound->setInteger("Y", pos.gety());
	nbttagcompound->setInteger("Z", pos.getz());
	return nbttagcompound;
}

IBlockState readBlockState(std::shared_ptr<NBTTagCompound> tag)
{
	if (!tag->hasKey("Name", 8)) {
		return Blocks.AIR.getDefaultState();
	}
	else {
		Block block = Block.REGISTRY.getObject(ResourceLocation(tag->getString("Name")));
		IBlockState iblockstate = block.getDefaultState();
		if (tag->hasKey("Properties", 10)) {
			auto nbttagcompound = tag->getCompoundTag("Properties");
			BlockStateContainer blockstatecontainer = block.getBlockState();
			for(auto s : nbttagcompound->getCompoundMap()){
				IProperty iproperty = blockstatecontainer.getProperty(s);
				iblockstate = NBTUtil::setValueHelper(iblockstate, iproperty, s, nbttagcompound, tag);
			}
		}

		return iblockstate;
	}
}

IBlockState NBTUtil::setValueHelper(IBlockState p_193590_0_, IProperty p_193590_1_, std::string p_193590_2_, std::shared_ptr<NBTTagCompound> p_193590_3_, std::shared_ptr<NBTTagCompound> p_193590_4_)
{
	Optional optional = p_193590_1_.parseValue(p_193590_3_.getString(p_193590_2_));
	if (optional.isPresent()) {
		return p_193590_0_.withProperty(p_193590_1_, (Comparable)optional.get());
	}
	else {
		LOGGER->warn("Unable to read property: {} with value: {} for blockstate: {}", p_193590_2_, p_193590_3_->getString(p_193590_2_), p_193590_4_->to_string());
		return p_193590_0_;
	}
}

std::shared_ptr<NBTTagCompound> writeBlockState(std::shared_ptr<NBTTagCompound> tag, IBlockState state)
{
	tag->setString("Name", ((ResourceLocation)Block.REGISTRY.getNameForObject(state.getBlock())).toString());
	if (!state.getProperties().isEmpty()) {
		auto nbttagcompound = std::make_shared<NBTTagCompound>();
		UnmodifiableIterator unmodifiableiterator = state.getProperties().entrySet().iterator();

		while (unmodifiableiterator.hasNext()) {
			Entry entry = (Entry)unmodifiableiterator.next();
			IProperty iproperty = (IProperty)entry.getKey();
			nbttagcompound.setString(iproperty.getName(), getName(iproperty, (Comparable)entry.getValue()));
		}

		tag->setTag("Properties", nbttagcompound);
	}

	return tag;
}