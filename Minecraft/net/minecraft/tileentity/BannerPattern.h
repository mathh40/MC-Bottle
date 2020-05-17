#pragma once
#include "../item/ItemStack.h"
#include "state/BlockWorldState.h"

class BannerPattern
{
public:

static BannerPattern BASE;
static BannerPattern SQUARE_BOTTOM_LEFT;
static BannerPattern SQUARE_BOTTOM_RIGHT;
static BannerPattern SQUARE_TOP_LEFT;
static BannerPattern SQUARE_TOP_RIGHT;
static BannerPattern STRIPE_BOTTOM;
static BannerPattern STRIPE_TOP;
static BannerPattern STRIPE_LEFT;
static BannerPattern STRIPE_RIGHT;
static BannerPattern STRIPE_CENTER;
static BannerPattern STRIPE_MIDDLE;
static BannerPattern STRIPE_DOWNRIGHT;
static BannerPattern STRIPE_DOWNLEFT;
static BannerPattern STRIPE_SMALL;
static BannerPattern CROSS;
static BannerPattern STRAIGHT_CROSS;
static BannerPattern TRIANGLE_BOTTOM;
static BannerPattern TRIANGLE_TOP;
static BannerPattern TRIANGLES_BOTTOM;
static BannerPattern TRIANGLES_TOP;
static BannerPattern DIAGONAL_LEFT;
static BannerPattern DIAGONAL_RIGHT;
static BannerPattern DIAGONAL_LEFT_MIRROR;
static BannerPattern DIAGONAL_RIGHT_MIRROR;
static BannerPattern CIRCLE_MIDDLE;
static BannerPattern RHOMBUS_MIDDLE;
static BannerPattern HALF_VERTICAL;
static BannerPattern HALF_HORIZONTAL;
static BannerPattern HALF_VERTICAL_MIRROR;
static BannerPattern HALF_HORIZONTAL_MIRROR;
static BannerPattern BORDER;
static BannerPattern CURLY_BORDER;
static BannerPattern CREEPER;
static BannerPattern GRADIENT;
static BannerPattern GRADIENT_UP;
static BannerPattern BRICKS;
static BannerPattern SKULL;
static BannerPattern FLOWER;
static BannerPattern MOJANG;

	
	std::string getFileName() const;
	std::string getHashname() const;
	std::array<std::string,3> getPatterns() const;
    bool hasPattern();
	bool hasPatternItem();
	ItemStack getPatternItem() const;
	static std::optional<BannerPattern> byHash(std::string_view hash);
	
private:
    BannerPattern(std::string_view p_i47245_3_, std::string_view p_i47245_4_);
	BannerPattern(std::string_view p_i47246_3_, std::string_view p_i47246_4_, ItemStack p_i47246_5_);
	BannerPattern(std::string_view p_i47247_3_, std::string_view p_i47247_4_, std::string_view p_i47247_5_, std::string_view p_i47247_6_, std::string_view p_i47247_7_);
	
	std::string fileName;
	std::string hashname;
	std::array<std::string,3> patterns;
	ItemStack patternItem;
};
