#pragma once
#include "NBTTagCompound.h"
#include <optional>
#include "math/BlockPos.h"
#include "../block/state/IBlockState.h"

namespace NBTUtil
{
	std::optional<GameProfile> readGameProfileFromNBT(NBTTagCompound compound);
	std::shared_ptr<NBTTagCompound> writeGameProfile(NBTTagCompound tagCompound, GameProfile profile);

	std::shared_ptr<NBTTagCompound> createUUIDTag(xg::Guid uuid);

	xg::Guid getUUIDFromTag(std::shared_ptr<NBTTagCompound> tag);
		
	BlockPos getPosFromTag(std::shared_ptr<NBTTagCompound> tag);

	std::shared_ptr<NBTTagCompound> createPosTag(const BlockPos &pos);

	IBlockState* readBlockState(std::shared_ptr<NBTTagCompound> tag);

	IBlockState* setValueHelper(IBlockState* p_193590_0_, IProperty p_193590_1_, std::string p_193590_2_, std::shared_ptr<NBTTagCompound> p_193590_3_, std::shared_ptr<NBTTagCompound> p_193590_4_);

	std::shared_ptr<NBTTagCompound> writeBlockState(std::shared_ptr<NBTTagCompound> tag, IBlockState* state);

	std::string getName(IProperty p_190010_0_, Comparable p_190010_1_);
}
