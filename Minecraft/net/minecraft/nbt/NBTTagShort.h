#pragma once
#include "NBTPrimitive.h"

class NBTTagShort :public NBTPrimitive
{
public:
	NBTTagShort() = default;
	explicit NBTTagShort(int16_t data);
	void write(std::ostream &output) const override;
	void read(std::istream &input, int depth, NBTSizeTracker* sizeTracker) override;
	uint8_t getId() const override;
	std::string to_string() const override;
	friend bool operator==(const NBTTagShort &a, const NBTTagShort &b);

	int64_t getLong()const override;

	int32_t getInt()const override;

	int16_t getShort()const override;

	uint8_t getByte()const override;

	double getDouble()const override;

	float getFloat()const override;
private:
	int16_t data{};
};

namespace std {
	template <> struct hash<NBTTagShort>
	{
		size_t operator()(const NBTTagShort & x) const noexcept
		{
			return std::hash<NBTBase>{}(x) ^ x.getShort();
		}
	};
}
