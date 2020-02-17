#include "NBTTagList.h"
#include "NBTSizeTracker.h"
#include <sstream>
#include "NBTTagInt.h"
#include "NBTTagIntArray.h"
#include "NBTTagDouble.h"
#include "NBTTagFloat.h"
#include "NBTTagEnd.h"

std::shared_ptr<spdlog::logger> LOGGER = spdlog::get("Minecraft")->clone("NBTTagList");

void NBTTagList::write(std::ostream& output) const
{
	if (tagList.empty()) {
		tagType = 0;
	}
	else {
		tagType = tagList[0]->getId();
	}

	output.write(reinterpret_cast<char*>(&tagType),sizeof(tagType));
	uint32_t size = tagList.size();
	output.write(reinterpret_cast<char*>(&size),sizeof(size));

	for (int i = 0; i < tagList.size(); ++i) {
		tagList[i]->write(output);
	}
}

void NBTTagList::read(std::istream& input, int depth, NBTSizeTracker sizeTracker)
{
	sizeTracker.read(296L);
	if (depth > 512) {
		throw std::runtime_error("Tried to read NBT tag with too high complexity, depth > 512");
	}
	else {
		input.read(reinterpret_cast<char*>(&tagType), sizeof(tagType));
		uint32_t size = 0;
		input.read(reinterpret_cast<char*>(&size), sizeof(size));
		if (tagType == 0 && size > 0) {
			throw std::runtime_error("Missing type on ListTag");
		}
		else {
			sizeTracker.read(32L * (long)size);
			tagList.clear();
			tagList.resize(size);

			for (int j = 0; j < size; ++j) {
				auto nbtbase = NBTBase::create(tagType);
				nbtbase->read(input, depth + 1, sizeTracker);
				tagList.emplace_back(nbtbase);
			}

		}
	}
}

uint8_t NBTTagList::getId() const
{
	return 9;
}

std::string NBTTagList::to_string() const
{
	std::stringstream stringbuilder("[");

	for (int i = 0; i < tagList.size(); ++i) {
		if (i != 0) {
			stringbuilder<<',';
		}

		stringbuilder<<tagList[i];
	}
	stringbuilder << ']';
	return stringbuilder.str();
}

void NBTTagList::appendTag(std::shared_ptr<NBTBase> nbt)
{
	if (nbt->getId() == 0) {
		LOGGER->warn("Invalid TagEnd added to ListTag");
	}
	else {
		if (tagType == 0) {
			tagType = nbt->getId();
		}
		else if (tagType != nbt->getId()) {
			LOGGER->warn("Adding mismatching tag types to tag list");
			return;
		}

		tagList.emplace_back(nbt);
	}
}

void NBTTagList::set(int idx, std::shared_ptr<NBTBase> nbt)
{
	if (nbt->getId() == 0) {
		LOGGER->warn("Invalid TagEnd added to ListTag");
	}
	else if (idx >= 0 && idx < tagList.size()) {
		if (tagType == 0) {
			tagType = nbt->getId();
		}
		else if (tagType != nbt->getId()) {
			LOGGER->warn("Adding mismatching tag types to tag list");
			return;
		}

		tagList[idx] = nbt;
	}
	else {
		LOGGER->warn("index out of bounds to set tag in tag list");
	}
}

std::shared_ptr<NBTBase> NBTTagList::removeTag(int i)
{
	auto removeTag = tagList[i];
	tagList.erase(tagList.begin() + i);
	return removeTag;
}

bool NBTTagList::isEmpty() const
{
	return tagList.empty();
}

std::shared_ptr<NBTTagCompound> NBTTagList::getCompoundTagAt(int i)
{
	if (i >= 0 && i < tagList.size()) {
		auto nbtbase = tagList[i];
		if (nbtbase->getId() == 10) {
			return std::shared_ptr<NBTTagCompound>(dynamic_cast<NBTTagCompound*>(nbtbase.get()));
		}
	}

	return std::shared_ptr<NBTTagCompound>();
}

int32_t NBTTagList::getIntAt(int32_t p_186858_1_)
{
	if (p_186858_1_ >= 0 && p_186858_1_ < tagList.size()) {
		auto nbtbase = tagList[p_186858_1_];
		if (nbtbase->getId() == 3) {
			return dynamic_cast<NBTTagInt*>(nbtbase.get())->getInt();
		}
	}

	return 0;
}

std::vector<int32_t> NBTTagList::getIntArrayAt(int32_t i)
{
	if (i >= 0 && i < tagList.size()) {
		auto nbtbase = tagList[i];
		if (nbtbase->getId() == 3) {
			return dynamic_cast<NBTTagIntArray*>(nbtbase.get())->getIntArray();
		}
	}

	return std::vector<int32_t>();
}

double NBTTagList::getDoubleAt(int32_t i)
{
	if (i >= 0 && i < tagList.size()) {
		auto nbtbase = tagList[i];
		if (nbtbase->getId() == 3) {
			return dynamic_cast<NBTTagDouble*>(nbtbase.get())->getDouble();
		}
	}

	return 0.0;
}

float NBTTagList::getFloatAt(int32_t i)
{
	if (i >= 0 && i < tagList.size()) {
		auto nbtbase = tagList[i];
		if (nbtbase->getId() == 3) {
			return dynamic_cast<NBTTagFloat*>(nbtbase.get())->getFloat();
		}
	}

	return 0.0f;
}

std::string NBTTagList::getStringTagAt(int i)
{
	if (i >= 0 && i < tagList.size()) {
		auto nbtbase = tagList[i];
		return nbtbase->getId() == 8 ? nbtbase->getString() : nbtbase->to_string();
	}

	return "";
}

std::shared_ptr<NBTBase> NBTTagList::get(int idx)
{
	return (idx >= 0 && idx < tagList.size() ? tagList[idx] : std::make_shared<NBTTagEnd>());
}

int32_t NBTTagList::tagCount() const
{
	return tagList.size();
}

std::vector<std::shared_ptr<NBTBase>> NBTTagList::getTagList() const
{
	return tagList;
}

int32_t NBTTagList::getTagType() const
{
	return tagType;
}

bool operator==(const NBTTagList& a, const NBTTagList& b)
{
	return a.tagType == b.tagType;
}
