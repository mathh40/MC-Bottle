#pragma once
#include "CooldownTracker.h"
#include "EnumPlayerModelParts.h"
#include "FoodStats.h"
#include "InventoryPlayer.h"
#include "PlayerCapabilities.h"
#include "../EntityLivingBase.h"
#include "../../../../../authlib/com/mojang/authlib/GameProfile.h"
#include "../../inventory/InventoryEnderChest.h"

class IRecipe;
class EntityMob;
class CommandBlockBaseLogic;
class TileEntityCommandBlock;
class TileEntityStructure;
class IMerchant;
class AbstractHorse;
class TileEntitySign;
class InventoryEnderChest;

class EntityPlayer :public EntityLivingBase {
public:

    enum SleepResult {
      OK,
      NOT_POSSIBLE_HERE,
      NOT_POSSIBLE_NOW,
      TOO_FAR_AWAY,
      OTHER_PROBLEM,
      NOT_SAFE
   };
    class SleepEnemyPredicate {
    public:
        SleepEnemyPredicate(EntityPlayer* playerIn);
        bool operator()(EntityMob* p_apply_1_) const;
    private:
        EntityPlayer* player;
    };

    class EnumChatVisibility
    {
    public:
      static EnumChatVisibility FULL;
      static EnumChatVisibility SYSTEM;
      static EnumChatVisibility HIDDEN;

      EnumChatVisibility(int32_t id, std::string_view resourceKey);
      int32_t getChatVisibility() const;
      static EntityPlayer::EnumChatVisibility *getEnumChatVisibility(int32_t id);
      std::string_view getResourceKey() const;

    private:
        static std::vector<EntityPlayer::EnumChatVisibility*> ID_LOOKUP;
      int32_t chatVisibility;
      std::string_view resourceKey;
   };

    EntityPlayer(World* worldIn, const GameProfile& gameProfileIn);
    void onUpdate() override;
    int32_t getMaxInPortalTime() const;
    int32_t getPortalCooldown() override;
    void playSound(SoundEvent soundIn, float volume, float pitch) override;
    SoundCategory getSoundCategory() override;
    void handleStatusUpdate(std::byte id) override;
    void updateRidden() override;
    void preparePlayerToSpawn() override;
    void onLivingUpdate() override;
    int32_t getScore();
    void setScore(int32_t scoreIn);
    void addScore(int32_t scoreIn);
    void onDeath(DamageSource::DamageSource cause) override;
    EntityItem* dropItem(bool dropAll);
    EntityItem* dropItem(const ItemStack& itemStackIn, bool unused);
    EntityItem* dropItem(ItemStack droppedItem, bool dropAround, bool traceItem);
    float getDigSpeed(IBlockState* state);
    bool canHarvestBlock(IBlockState* state);
    static void registerFixesPlayer(DataFixer fixer);
    void readEntityFromNBT(NBTTagCompound* compound) override;
    void writeEntityToNBT(NBTTagCompound* compound) override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    float getArmorVisibility();
    virtual void openEditSign(TileEntitySign *signTile);
    virtual void displayGuiEditCommandCart(CommandBlockBaseLogic *commandBlock);
    virtual void displayGuiCommandBlock(TileEntityCommandBlock *commandBlock);
    virtual void openEditStructure(TileEntityStructure *structure);
    virtual void displayVillagerTradeGui(IMerchant *villager);
    virtual void displayGUIChest(IInventory *chestInventory);
    virtual void openGuiHorseInventory(AbstractHorse *horse, IInventory *inventoryIn);
    virtual void displayGui(IInteractionObject *guiOwner);
    virtual void openBook(ItemStack stack, EnumHand hand);
    EnumActionResult interactOn(Entity* entityToInteractOn, EnumHand hand);
    double getYOffset() override;
    void dismountRidingEntity() override;
    virtual void attackTargetEntityWithCurrentItem(Entity* targetEntity);
    void disableShield(bool p_190777_1_);
    virtual void onCriticalHit(Entity *entityHit);
    virtual void onEnchantmentCritical(Entity *entityHit);
    void spawnSweepParticles() const;
    virtual void respawnPlayer();
    void setDead() override;
    bool isEntityInsideOpaqueBlock() const override;
    bool isUser();
    const GameProfile& getGameProfile() const;
    virtual EntityPlayer::SleepResult trySleep(const BlockPos& bedLocation);
    virtual void wakeUpPlayer(bool immediately, bool updateWorldFlag, bool setSpawn);
    std::optional<BlockPos> getBedSpawnLocation(World* worldIn, const BlockPos& bedLocation, bool forceSpawn);
    float getBedOrientationInDegrees();
    bool isPlayerSleeping() override;
    bool isPlayerFullyAsleep() const;
    int32_t getSleepTimer() const;
    virtual void sendStatusMessage(ITextComponent* chatComponent, bool actionBar);
    BlockPos getBedLocation();
    bool isSpawnForced() const;
    void setSpawnPoint(std::optional<BlockPos> pos, bool forced);
    void addStat(StatBase* stat);
    virtual void addStat(StatBase *stat, int32_t amount);
    virtual void takeStat(StatBase *stat);
    virtual void unlockRecipes(std::vector<IRecipe*> p_192021_1_);
    virtual void unlockRecipes(std::vector<ResourceLocation> p_193102_1_);
    virtual void resetRecipes(std::vector<IRecipe*> p_192022_1_);
    void jump() override;
    void travel(float strafe, float vertical, float forward) override;
    float getAIMoveSpeed();
    void addMovementStat(double p_71000_1_, double p_71000_3_, double p_71000_5_);
    void fall(float distance, float damageMultiplier) override;
    void onKillEntity(EntityLivingBase* entityLivingIn) override;
    void setInWeb() override;
    void addExperience(int32_t amount);
    int32_t getXPSeed() const;
    virtual void onEnchant(ItemStack enchantedItem, int32_t cost);
    virtual void addExperienceLevel(int32_t levels);
    int32_t xpBarCap() const;
    void addExhaustion(float exhaustion);
    FoodStats& getFoodStats();
    bool canEat(bool ignoreHunger) const;
    bool shouldHeal();
    bool isAllowEdit();
    bool canPlayerEdit(const BlockPos& pos, EnumFacing facing, ItemStack stack);
    bool getAlwaysRenderNameTagForRender() override;
    virtual void sendPlayerAbilities();
    virtual void setGameType(GameType gameType);
    std::string getName() const override;
    InventoryEnderChest& getInventoryEnderChest();
    ItemStack getItemStackFromSlot(EntityEquipmentSlot slotIn) override;
    void setItemStackToSlot(EntityEquipmentSlot slotIn, ItemStack stack) override;
    bool addItemStackToInventory(ItemStack p_191521_1_);
    std::vector<ItemStack> getHeldEquipment() override;
    std::vector<ItemStack>& getArmorInventoryList() override;
    bool addShoulderEntity(NBTTagCompound* p_192027_1_);
    bool isInvisibleToPlayer(EntityPlayer* player) override;
    virtual bool isSpectator() = 0;
    virtual bool isCreative() = 0;
    bool isPushedByWater() override;
    Scoreboard getWorldScoreboard() const;
    Team* getTeam() override;
    ITextComponent* getDisplayName();
    float getEyeHeight();
    void setAbsorptionAmount(float amount) override;
    float getAbsorptionAmount();
    static xg::Guid getUUID(const GameProfile& profile);
    static xg::Guid getOfflineUUID(std::string_view username);
    bool canOpen(LockCode code);
    bool isWearing(const EnumPlayerModelParts& part);
    bool sendCommandFeedback() override;
    bool replaceItemInInventory(int32_t inventorySlot, ItemStack itemStackIn) override;
    bool hasReducedDebug() const;
    EnumHandSide getPrimaryHand() override;
    void setPrimaryHand(EnumHandSide hand);
    NBTTagCompound* getLeftShoulderEntity();
    NBTTagCompound* getRightShoulderEntity();
    float getCooldownPeriod();
    float getCooledAttackStrength(float adjustTicks);
    void resetCooldown();
    CooldownTracker getCooldownTracker() const;
    void applyEntityCollision(Entity* entityIn) override;
    float getLuck();
    bool canUseCommandBlock();


    InventoryPlayer inventory;
    std::unique_ptr<Container> inventoryContainer;
    Container* openContainer;
    float prevCameraYaw;
    float cameraYaw;
    int32_t xpCooldown;
    double prevChasingPosX;
    double prevChasingPosY;
    double prevChasingPosZ;
    double chasingPosX;
    double chasingPosY;
    double chasingPosZ;
    float renderOffsetX;
    float renderOffsetY;
    float renderOffsetZ;
    PlayerCapabilities capabilities;
    int32_t experienceLevel;
    int32_t experienceTotal;
    float experience;
    EntityFishHook* fishEntity;
    std::optional<BlockPos> bedLocation;
    
protected:
    void applyEntityAttributes() override;
    void entityInit() override;
    void updateSize();
    SoundEvent getSwimSound() override;
    SoundEvent getSplashSound() override;
    int32_t getFireImmuneTicks() override;
    bool isMovementBlocked() override;
    virtual void closeScreen();
    void updateEntityActionState() override;
    void destroyVanishingCursedItems();
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    ItemStack dropItemAndGetStack(EntityItem* p_184816_1_) const;
    void blockUsingShield(EntityLivingBase* p_190629_1_) override;
    virtual bool canAttackPlayer(EntityPlayer* other);
    void damageArmor(float damage) override;
    void damageShield(float damage) override;
    void damageEntity(DamageSource::DamageSource damageSrc, float damageAmount) override;
    void doWaterSplashEffect() override;
    SoundEvent getFallSound(int32_t heightIn) override;
    int32_t getExperiencePoints(EntityPlayer* player) override;
    bool isPlayer() override;
    bool canTriggerWalking() override;
    void spawnShoulderEntities();
    void setReducedDebug(bool reducedDebug);
    void setLeftShoulderEntity(NBTTagCompound* tag);
    void setRightShoulderEntity(NBTTagCompound* tag);


    static DataParameter PLAYER_MODEL_FLAG;
    static DataParameter MAIN_HAND;
    static DataParameter LEFT_SHOULDER_ENTITY;
    static DataParameter RIGHT_SHOULDER_ENTITY;
    FoodStats foodStats;
    int32_t flyToggleTimer;
    InventoryEnderChest enderChest;
    bool sleeping;
    int32_t xpSeed;
    float speedInAir = 0.02F;

    static CooldownTracker createCooldownTracker();

private:
    void updateCape();
    void playShoulderEntityAmbientSound(NBTTagCompound* p_192028_1_);
    void collideWithPlayer(Entity* entityIn);
    bool bedInRange(const BlockPos& p_190774_1_, EnumFacing p_190774_2_) const;
    void setRenderOffsetForSleep(EnumFacing bedDirection);
    bool isInBed();
    void addMountedMovementStat(double p_71015_1_, double p_71015_3_, double p_71015_5_);
    void spawnShoulderEntity(NBTTagCompound* p_192026_1_);




    static DataParameter ABSORPTION;
    static DataParameter PLAYER_SCORE;
    
    int32_t sleepTimer;
    std::optional<BlockPos> spawnPos;
    bool spawnForced;   
    int32_t lastXPSound;
    GameProfile gameProfile;
    bool bhasReducedDebug;
    ItemStack itemStackMainHand;
    CooldownTracker cooldownTracker;
};
