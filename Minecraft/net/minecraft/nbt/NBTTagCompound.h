#pragma once
#include "NBTBase.h"
#include "NBTTagByte.h"
#include <regex>
#include <unordered_map>
#include <spdlog/logger.h>
#include "NBTTagList.h"
#include <memory>

namespace xg {
	class Guid;
}

class NBTTagCompound :public NBTBase
{
public:
	void write(std::ostream &output) const override;
	void read(std::istream &input, int depth, NBTSizeTracker sizeTracker);
	uint8_t getId() const override;
	int32_t getSize() const;
	std::string to_string()const override;
	bool isEmpty() const override;

	friend bool operator==(const NBTTagCompound &a, const NBTTagCompound &b);
	void merge(NBTTagCompound* other);


	void setTag(std::string key, std::unique_ptr<NBTBase> value);
	void setByte(std::string key, uint8_t value);
	void setShort(std::string key, int16_t value);
	void setInteger(std::string key, int32_t value);
	void setLong(std::string key, int64_t value);
	void setUniqueId(std::string key, xg::Guid &value);
	xg::Guid getUniqueId(std::string key);
	bool hasUniqueId(std::string key) const;
	void setFloat(std::string key, float value);
	void setDouble(std::string key, double value);
	void setString(std::string key, std::string value);
	void setByteArray(std::string key, std::vector<uint8_t> &value);
	void setIntArray(std::string key, std::vector<int32_t> &value);
	void setBoolean(std::string key, bool value);
	NBTBase* getTag(std::string key) const;
	uint8_t getTagId(std::string key) const;
	bool hasKey(std::string key) const;
	bool hasKey(std::string key, int type) const;
	uint8_t getByte(std::string key) const;
	int16_t getShort(std::string key) const;
	int32_t getInteger(std::string key) const;
	int64_t getLong(std::string key) const;
	float getFloat(std::string key) const;
	double getDouble(std::string key) const;
	std::string getString(std::string key) const;
	std::vector<uint8_t> getByteArray(std::string key) const;
	std::vector<int32_t> getIntArray(std::string key) const;
	NBTTagCompound* getCompoundTag(std::string key) const;
	NBTTagList* getTagList(std::string key, int type) const;
	bool getBoolean(std::string key) const;
	void removeTag(std::string key);
	const std::unordered_map<const std::string, std::unique_ptr<NBTBase>>& getCompoundMap()const;
protected:

	std::string handleEscape(std::string p_193582_0_) const;
private:
	static std::regex SIMPLE_VALUE;
	static std::shared_ptr<spdlog::logger> LOGGER;
	std::unordered_map<const std::string, std::unique_ptr<NBTBase>> tagMap;

	CrashReport createCrashReport(std::string key, int expectedType, ClassCastException ex);

	void writeEntry(std::string name, NBTBase *data, std::ostream &output) const;
	uint8_t readType(std::istream &input, NBTSizeTracker sizeTracker) const;
	std::string readKey(std::istream &input, NBTSizeTracker sizeTracker) const;
	std::unique_ptr<NBTBase> readNBT(uint8_t id, std::string key, std::istream &input, int depth, NBTSizeTracker sizeTracker);

};

namespace std {
	template <> struct hash<NBTTagCompound>
	{
		size_t operator()(const NBTTagCompound & x) const noexcept
		{
			auto hash = 0;

			for (auto byte : x.getCompoundMap()) {
				hash = hash * 31 + std::hash <NBTBase>{}(*byte.second);
			}

			return std::hash<NBTBase>{}(x) ^ hash;
		}
	};
}
