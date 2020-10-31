#pragma once
#include "EntityPlayer.h"
#include "../../inventory/IContainerListener.h"
#include "../../stats/RecipeBookServer.h"

class WorldServer;

class EntityPlayerMP :public EntityPlayer ,public IContainerListener
{
public:
    EntityPlayerMP(MinecraftServer* server, WorldServer* worldIn, GameProfile profile, PlayerInteractionManager interactionManagerIn);
    void readEntityFromNBT(NBTTagCompound* compound) override;
    static void registerFixesPlayerMP(DataFixer p_191522_0_);
    void writeEntityToNBT(NBTTagCompound* compound) override;
    void addExperienceLevel(int32_t levels) override;
    void onEnchant(ItemStack enchantedItem, int32_t cost) override;
    void addSelfToInternalCraftingInventory();
    void sendEnterCombat() override;
    void sendEndCombat() override;
    void onUpdate() override;
    void onUpdateEntity();
    void onDeath(DamageSource::DamageSource cause) override;
    void awardKillScore(Entity* p_191956_1_, int32_t p_191956_2_, DamageSource::DamageSource p_191956_3_) override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    bool canAttackPlayer(EntityPlayer* other) override;
    Entity* changeDimension(int32_t dimensionIn) override;
    bool isSpectatedByPlayer(EntityPlayerMP* player) override;
    void onItemPickup(Entity* entityIn, int32_t quantity) override;
    EntityPlayer::SleepResult trySleep(const BlockPos& bedLocation) override;
    void wakeUpPlayer(bool immediately, bool updateWorldFlag, bool setSpawn) override;
    bool startRiding(Entity* entityIn, bool force) override;
    void dismountRidingEntity() override;
    bool isEntityInvulnerable(DamageSource::DamageSource source) override;
    void handleFalling(double y, bool onGroundIn);
    void openEditSign(TileEntitySign* signTile) override;
    void displayGui(IInteractionObject* guiOwner) override;
    void displayGUIChest(IInventory* chestInventory) override;
    void displayVillagerTradeGui(IMerchant* villager) override;
    void openGuiHorseInventory(AbstractHorse* horse, IInventory* inventoryIn) override;
    void openBook(ItemStack stack, EnumHand hand) override;
    void displayGuiCommandBlock(TileEntityCommandBlock* commandBlock) override;
    void sendSlotContents(Container* containerToSend, int32_t slotInd, ItemStack stack);
    void sendContainerToPlayer(Container* containerIn);
    void sendAllContents(Container* containerToSend, std::vector<ItemStack> itemsList);
    void sendWindowProperty(Container* containerIn, int32_t varToUpdate, int32_t newValue);
    void sendAllWindowProperties(Container* containerIn, IInventory* inventory);
    void closeScreen() override;
    void updateHeldItem();
    void closeContainer();
    void setEntityActionState(float strafe, float forward, bool jumping, bool sneaking);
    void addStat(StatBase* stat, int32_t amount) override;
    void takeStat(StatBase* stat) override;
    void unlockRecipes(std::vector<IRecipe*> p_192021_1_) override;
    void unlockRecipes(std::vector<ResourceLocation> p_193102_1_) override;



    NetHandlerPlayServer connection;
    MinecraftServer* server;
    PlayerInteractionManager interactionManager;
    double managedPosX;
    double managedPosZ;
    bool isChangingQuantityOnly;
    int32_t ping;
    bool queuedEndExit;
protected:
    void onInsideBlock(IBlockState* p_191955_1_) override;
    std::unique_ptr<CooldownTracker> createCooldownTracker();
    void updateFallState(double y, bool onGroundIn, IBlockState *state, const BlockPos &pos) override;
    void frostWalk(const BlockPos &pos) override;

private:
    void updateScorePoints(IScoreCriteria* criteria, int32_t points) const;
    std::vector<ScoreObjective> awardTeamKillScores(Entity* p_192038_1_);
    bool canPlayersAttack() const;
    void sendTileEntityUpdate(TileEntity* p_147097_1_);
    void getNextWindowId();


    static std::shared_ptr<spdlog::logger> LOGGER;
    std::string language = "en_US";
    std::vector<int32_t> entityRemoveQueue;
    PlayerAdvancements advancements;
    StatisticsManagerServer statsFile;
    float lastHealthScore = std::numeric_limits<float>::min();
    int32_t lastFoodScore = std::numeric_limits<int32_t>::min();
    int32_t lastAirScore = std::numeric_limits<int32_t>::min();
    int32_t lastArmorScore = std::numeric_limits<int32_t>::min();
    int32_t lastLevelScore = std::numeric_limits<int32_t>::min();
    int32_t lastExperienceScore = std::numeric_limits<int32_t>::min();
    float lastHealth = -1.0E8F;
    int32_t lastFoodLevel = -99999999;
    bool wasHungry = true;
    int32_t lastExperience = -99999999;
    int32_t respawnInvulnerabilityTicks = 60;
    EntityPlayer::EnumChatVisibility chatVisibility;
    bool chatColours = true;
    int64_t playerLastActiveTime = System.currentTimeMillis();
    Entity* spectatingEntity;
    bool invulnerableDimensionChange;
    bool seenCredits;
    RecipeBookServer recipeBook;
    std::optional<Vec3d> levitationStartPos;
    int32_t levitatingSince;
    bool disconnected;
    std::optional<Vec3d> enteredNetherPosition;
    int32_t currentWindowId;

};
