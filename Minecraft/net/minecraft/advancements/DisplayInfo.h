#pragma once

class ITextComponent;
class ItemStack;
class ByteBuffer;

class DisplayInfo
{
  public:
    DisplayInfo(const ItemStack &icon, ITextComponent *title, ITextComponent *description,
                std::optional<ResourceLocation> background, FrameType frame, bool showToast, bool announceToChat,
                bool hidden);
    void setPosition(float x, float y);
    ITextComponent *getTitle() const;
    ITextComponent *getDescription() const;
    const ItemStack &getIcon() const;
    std::optional<ResourceLocation> getBackground() const;
    FrameType &getFrame() const;
    float getX() const;
    float getY() const;
    bool shouldShowToast() const;
    bool shouldAnnounceToChat() const;
    bool isHidden() const;
    static DisplayInfo deserialize(json &object);
    void write(ByteBuffer &buf);
    static DisplayInfo read(const ByteBuffer &buf);

  private:
    static ItemStack deserializeIcon(json &object);

    ITextComponent *title;
    ITextComponent *description;
    ItemStack *icon;
    ResourceLocation background;
    FrameType frame;
    bool showToast;
    bool announceToChat;
    bool hidden;
    float x;
    float y;
};
