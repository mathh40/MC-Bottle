#pragma once

class PlayerAdvancements
{
  public:
    PlayerAdvancements(MinecraftServer *server, std::filesystem::patch &p_i47422_2_, EntityPlayerMP *player);
    void setPlayer(EntityPlayerMP *player);
    void dispose();
    void reload();
    void save();
    bool grantCriterion(Advancement p_192750_1_, std::string_view p_192750_2_);
    bool revokeCriterion(Advancement p_192744_1_, std::string_view p_192744_2_);
    void flushDirty(EntityPlayerMP *p_192741_1_);
    void setSelectedTab(@Nullable Advancement p_194220_1_);
    AdvancementProgress getProgress(Advancement advancementIn);

  private:
    void registerListeners();
    void ensureAllVisible();
    void checkForAutomaticTriggers();
    void load();
    void registerListeners(Advancement p_193764_1_);
    void unregisterListeners(Advancement p_193765_1_);
    void startProgress(Advancement p_192743_1_, AdvancementProgress p_192743_2_);
    void ensureVisibility(Advancement p_192742_1_);
    bool shouldBeVisible(Advancement p_192738_1_);
    bool hasCompletedChildrenOrSelf(Advancement p_192746_1_);

    static std::shared_ptr<Logger> LOGGER;
    static Gson GSON = (new GsonBuilder())
                           .registerTypeAdapter(AdvancementProgress.class, new AdvancementProgress.Serializer())
                           .registerTypeAdapter(ResourceLocation.class, new ResourceLocation.Serializer())
                           .setPrettyPrinting()
                           .create();
    static TypeToken MAP_TOKEN;
    MinecraftServer *server;
    std::filesystem::patch progressFile;
    Map progress;
    std::set<> visible;
    std::set<> visibilityChanged;
    std::set<> progressChanged;
    EntityPlayerMP *player;
    @Nullable Advancement lastSelectedTab;
    bool isFirstPacket = true;
};
