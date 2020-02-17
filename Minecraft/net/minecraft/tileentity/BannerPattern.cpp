#include "BannerPattern.h"
#include "../world/gen/feature/WorldGenFlowers.h"

BannerPattern::BASE("base", "b"),
BannerPattern::SQUARE_BOTTOM_LEFT("square_bottom_left", "bl", "   ", "   ", "#  "),
BannerPattern::SQUARE_BOTTOM_RIGHT("square_bottom_right", "br", "   ", "   ", "  #"),
BannerPattern::SQUARE_TOP_LEFT("square_top_left", "tl", "#  ", "   ", "   "),
BannerPattern::SQUARE_TOP_RIGHT("square_top_right", "tr", "  #", "   ", "   "),
BannerPattern::STRIPE_BOTTOM("stripe_bottom", "bs", "   ", "   ", "###"),
BannerPattern::STRIPE_TOP("stripe_top", "ts", "###", "   ", "   "),
BannerPattern::STRIPE_LEFT("stripe_left", "ls", "#  ", "#  ", "#  "),
BannerPattern::STRIPE_RIGHT("stripe_right", "rs", "  #", "  #", "  #"),
BannerPattern::STRIPE_CENTER("stripe_center", "cs", " # ", " # ", " # "),
BannerPattern::STRIPE_MIDDLE("stripe_middle", "ms", "   ", "###", "   "),
BannerPattern::STRIPE_DOWNRIGHT("stripe_downright", "drs", "#  ", " # ", "  #"),
BannerPattern::STRIPE_DOWNLEFT("stripe_downleft", "dls", "  #", " # ", "#  "),
BannerPattern::STRIPE_SMALL("small_stripes", "ss", "# #", "# #", "   "),
BannerPattern::CROSS("cross", "cr", "# #", " # ", "# #"),
BannerPattern::STRAIGHT_CROSS("straight_cross", "sc", " # ", "###", " # "),
BannerPattern::TRIANGLE_BOTTOM("triangle_bottom", "bt", "   ", " # ", "# #"),
BannerPattern::TRIANGLE_TOP("triangle_top", "tt", "# #", " # ", "   "),
BannerPattern::TRIANGLES_BOTTOM("triangles_bottom", "bts", "   ", "# #", " # "),
BannerPattern::TRIANGLES_TOP("triangles_top", "tts", " # ", "# #", "   "),
BannerPattern::DIAGONAL_LEFT("diagonal_left", "ld", "## ", "#  ", "   "),
BannerPattern::DIAGONAL_RIGHT("diagonal_up_right", "rd", "   ", "  #", " ##"),
BannerPattern::DIAGONAL_LEFT_MIRROR("diagonal_up_left", "lud", "   ", "#  ", "## "),
BannerPattern::DIAGONAL_RIGHT_MIRROR("diagonal_right", "rud", " ##", "  #", "   "),
BannerPattern::CIRCLE_MIDDLE("circle", "mc", "   ", " # ", "   "),
BannerPattern::RHOMBUS_MIDDLE("rhombus", "mr", " # ", "# #", " # "),
BannerPattern::HALF_VERTICAL("half_vertical", "vh", "## ", "## ", "## "),
BannerPattern::HALF_HORIZONTAL("half_horizontal", "hh", "###", "###", "   "),
BannerPattern::HALF_VERTICAL_MIRROR("half_vertical_right", "vhr", " ##", " ##", " ##"),
BannerPattern::HALF_HORIZONTAL_MIRROR("half_horizontal_bottom", "hhb", "   ", "###", "###"),
BannerPattern::BORDER("border", "bo", "###", "# #", "###"),
BannerPattern::CURLY_BORDER("curly_border", "cbo", ItemStack(Blocks::VINE)),
BannerPattern::CREEPER("creeper", "cre", ItemStack(Items::SKULL, 1, 4)),
BannerPattern::GRADIENT("gradient", "gra", "# #", " # ", " # "),
BannerPattern::GRADIENT_UP("gradient_up", "gru", " # ", " # ", "# #"),
BannerPattern::BRICKS("bricks", "bri", ItemStack(Blocks::BRICK_BLOCK)),
BannerPattern::SKULL("skull", "sku", ItemStack(Items::SKULL, 1, 1)),
BannerPattern::FLOWER("flower", "flo", ItemStack(Blocks::RED_FLOWER, 1, BlockFlower::EnumFlowerType::OXEYE_DAISY.getMeta())),
BannerPattern::MOJANG("mojang", "moj", ItemStack(Items::GOLDEN_APPLE, 1, 1));


std::string BannerPattern::getFileName() const
{
	return fileName;
}

std::string BannerPattern::getHashname() const
{
	return hashname;
}

std::array<std::string, 3> BannerPattern::getPatterns() const
{
	return patterns;
}

bool BannerPattern::hasPattern()
{
	return !patternItem.isEmpty() || !patterns[0].empty();
}

bool BannerPattern::hasPatternItem()
{
	return !patternItem.isEmpty();
}

ItemStack BannerPattern::getPatternItem() const
{
	return patternItem;
}

std::optional<BannerPattern> BannerPattern::byHash(std::string_view hash)
{
	BannerPattern[] var1 = values();
	int var2 = var1.length;

	for(int var3 = 0; var3 < var2; ++var3) {
		BannerPattern bannerpattern = var1[var3];
		if (bannerpattern.hashname == hash) 
		{
			return bannerpattern;
		}
	}
	return std::nullopt;
}

BannerPattern::BannerPattern(std::string_view p_i47245_3_, std::string_view p_i47245_4_)
	:patternItem(ItemStack::EMPTY),fileName(p_i47245_3_),hashname(p_i47245_4_)
{
	
}

BannerPattern::BannerPattern(std::string_view p_i47246_3_, std::string_view p_i47246_4_, ItemStack p_i47246_5_)
	:BannerPattern(p_i47246_3_, p_i47246_4_),patternItem(p_i47246_5_)
{
}

BannerPattern::BannerPattern(std::string_view p_i47247_3_, std::string_view p_i47247_4_, std::string_view p_i47247_5_,
	std::string_view p_i47247_6_, std::string_view p_i47247_7_)
	:BannerPattern(p_i47247_3_, p_i47247_4_)
{
	patterns[0] = p_i47247_5_;
	patterns[1] = p_i47247_6_;
	patterns[2] = p_i47247_7_;
}
