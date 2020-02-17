#pragma once
#include <cstdint>
#include <vector>

class MapDecoration
{
public:
	class Type
	{
	public:
		static MapDecoration::Type PLAYER;
		static MapDecoration::Type FRAME;
		static MapDecoration::Type RED_MARKER;
		static MapDecoration::Type BLUE_MARKER;
		static MapDecoration::Type TARGET_X;
		static MapDecoration::Type TARGET_POINT;
		static MapDecoration::Type PLAYER_OFF_MAP;
		static MapDecoration::Type PLAYER_OFF_LIMITS;
		static MapDecoration::Type MANSION;
		static MapDecoration::Type MONUMENT;

		uint8_t getIcon() const;
		bool isRenderedOnFrame() const;
		bool hasMapColor() const;
		uint32_t getMapColor() const;
		MapDecoration::Type* byIcon(uint8_t p_191159_0_) const;
		friend bool operator==(const Type& lhs, const Type& rhs);
		friend bool operator!=(const Type& lhs, const Type& rhs);
	private:
		uint8_t ordinal;
		uint8_t icon;
		bool renderedOnFrame;
		uint32_t mapColor;
		static std::vector<Type*> values;
		static uint8_t count;
		explicit Type(bool p_i47343_3_);
		Type(bool p_i47344_3_, uint32_t p_i47344_4_);
	};

	MapDecoration(MapDecoration::Type typeIn, uint8_t xIn, uint8_t yIn, uint8_t rotationIn);
	uint8_t getImage() const;
	MapDecoration::Type getType() const;
	uint8_t getX() const;
	uint8_t getY() const;
	uint8_t getRotation() const;
	bool renderOnFrame() const;
	friend bool operator==(const MapDecoration& lhs, const MapDecoration& rhs);

private:
	MapDecoration::Type type;
	uint8_t x;
	uint8_t y;
	uint8_t rotation;

};

namespace std
{
	template<> 
	struct hash<MapDecoration>
	{
		typedef MapDecoration argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const noexcept
		{
			auto i = s.getType().getIcon();
			i = 31 * i + s.getX();
			i = 31 * i + s.getY();
			i = 31 * i + s.getRotation();
			return i;
		}
	};
}
