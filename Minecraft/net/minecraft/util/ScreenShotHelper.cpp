#include "ScreenShotHelper.h"

#include "text/TextComponentString.h"
#include "text/TextComponentTranslation.h"

#include <lodepng.h>
#include <spdlog\spdlog.h>

namespace ScreenShotHelper
{
std::filesystem::path getTimestampedPNGFileForDirectory(std::filesystem::path gameDirectory)
{
    std::time_t t = std::time(nullptr);
    std::tm tm    = *std::localtime(&t);

    int i  = 1;
    auto s = std::put_time(&tm, "%Y-%m-%d_%H.%M.%S");
    while (true)
    {
        std::filesystem::path file1 =
            std::filesystem::path(gameDirectory.string() + s._Fmtfirst + (i == 1 ? "" : "_" + i) + ".png");
        if (!std::filesystem::exists(file1))
        {
            return file1;
        }

        ++i;
    }
}

ITextComponent *saveScreenshot(std::filesystem::path gameDirector, std::optional<std::string> screenshotName, int width,
                               int height, Framebuffer buffer)
{

    auto LOGGER = spdlog::get("Minecraft")->clone("ScreenShot");
    ;
    auto file1 = gameDirector.append("screenshots");
    std::filesystem::create_directory(file1);
    auto bufferedimage = createScreenshot(width, height, buffer);

    std::filesystem::path file2;

    if (!screenshotName.has_value())
    {
        file2 = getTimestampedPNGFileForDirectory(file1);
    }
    else
    {
        file2 = file1.append("screenshotName");
    }

    lodepng::encode(file2.string, bufferedimage, width, height);

    ITextComponent *itextcomponent = new TextComponentString(file2.getName());
    itextcomponent->getStyle().setClickEvent(new ClickEvent(ClickEvent::Action::OPEN_FILE, file2));
    itextcomponent->getStyle().setUnderlined(true);

    return new TextComponentTranslation("screenshot.success", new Object[]{itextcomponent});
}
std::vector<unsigned char> createScreenshot(int width, int height, Framebuffer framebufferIn)
{
    std::vector<uint32_t> pixelBuffer;

    if (OpenGlHelper.isFramebufferEnabled())
    {
        width  = framebufferIn.framebufferTextureWidth;
        height = framebufferIn.framebufferTextureHeight;
    }

    int i = width * height;
    if (pixelBuffer.empty() || pixelBuffer.capacity() < i)
    {
        pixelBuffer.resize(i);
        ;
    }

    GlStateManager.glPixelStorei(3333, 1);
    GlStateManager.glPixelStorei(3317, 1);
    pixelBuffer.clear();
    if (OpenGlHelper.isFramebufferEnabled())
    {
        GlStateManager.bindTexture(framebufferIn.framebufferTexture);
        GlStateManager.glGetTexImage(3553, 0, 32993, 33639, pixelBuffer);
    }
    else
    {
        GlStateManager.glReadPixels(0, 0, width, height, 32993, 33639, pixelBuffer);
    }

    TextureUtil.processPixelValues(pixelBuffer, width, height);
    auto bufferedimage = std::vector<uint32_t>(width * height);
    return bufferedimage;
}
} // namespace ScreenShotHelper
