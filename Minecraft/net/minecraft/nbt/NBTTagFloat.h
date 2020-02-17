#pragma once
#include "NBTPrimitive.h"

class NBTTagFloat :public NBTPrimitive
{
public:
	NBTTagFloat() = default;
	explicit NBTTagFloat(float data);
	void write(std::ostream &output)const override;
	void read(std::istream &input, int depth, NBTSizeTracker* sizeTracker) override;
	uint8_t getId() const override;
	std::string to_string() const override;
	friend bool operator==(const NBTTagFloat &a, const NBTTagFloat &b);

	int64_t getLong()const override;

	int32_t getInt()const override;

	int16_t getShort()const override;

	uint8_t getByte()const override;

	double getDouble()const override;

	float getFloat()const override;
private:
	float data{};
};

namespace std {
	template <> struct hash<NBTTagFloat>
	{
		size_t operator()(const NBTTagFloat & x) const noexcept
		{
			auto i = static_cast<int32_t>(x.getFloat());
			return std::hash<NBTBase>{}(x) ^ i;
		}
	};
}
