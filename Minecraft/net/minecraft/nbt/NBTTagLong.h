#pragma once
#include "NBTPrimitive.h"

class NBTTagLong :public NBTPrimitive
{
public:
	NBTTagLong() = default;
	explicit NBTTagLong(int64_t data);
	void write(std::ostream &output) const override;
	void read(std::istream &input, int depth, NBTSizeTracker* sizeTracker) override;
	uint8_t getId() const override;
	std::string to_string() const override;
	friend bool operator==(const NBTTagLong &a, const NBTTagLong &b);

	int64_t getLong()const override;

	int32_t getInt()const override;

	int16_t getShort()const override;

	uint8_t getByte()const override;

	double getDouble()const override;

	float getFloat()const override;
private:
	int64_t data{};
};

namespace std {
	template <> struct hash<NBTTagLong>
	{
		size_t operator()(const NBTTagLong & x) const noexcept
		{
			return std::hash<NBTBase>{}(x) ^ (x.getLong() ^ x.getLong() >> 32);
		}
	};
}