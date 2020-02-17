#pragma once
#include <filesystem>
#include <optional>

namespace ScreenShotHelper 
{
	ITextComponent saveScreenshot(std::filesystem::path gameDirectory, int width, int height, Framebuffer buffer) {
		return saveScreenshot(gameDirectory, std::nullopt, width, height, buffer);
	}

	ITextComponent saveScreenshot(std::filesystem::path gameDirector,std::optional<std::string> screenshotName, int width, int height, Framebuffer buffer);

	std::vector<unsigned char> createScreenshot(int width, int height, Framebuffer framebufferIn);
};
