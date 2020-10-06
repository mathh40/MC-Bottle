#pragma once
#include "../EntityLivingBase.h"
#include "math/Rotations.h"

class DataParameter;
class EntityArmorStand :public EntityLivingBase {
public:
    EntityArmorStand(World* worldIn);
    EntityArmorStand(World* worldIn, double posX, double posY, double posZ);
    bool isServerWorld();
    std::vector<ItemStack>& getHeldEquipment() override;
    std::vector<ItemStack>& getArmorInventoryList() override;
    ItemStack getItemStackFromSlot(EntityEquipmentSlot slotIn) override;
    void setItemStackToSlot(EntityEquipmentSlot slotIn, ItemStack stack) override;
    bool replaceItemInInventory(int32_t inventorySlot, ItemStack itemStackIn) override;
    void writeEntityToNBT(NBTTagCompound* compound) override;
    void readEntityFromNBT(NBTTagCompound* compound) override;
    bool canBePushed() override;
    EnumActionResult applyPlayerInteraction(EntityPlayer* player, Vec3d vec, EnumHand hand) override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    void handleStatusUpdate(std::byte id) override;
    bool isInRangeToRenderDist(double distance) override;
    float getEyeHeight();
    double getYOffset() override;
    void travel(float strafe, float vertical, float forward) override;
    void setRenderYawOffset(float offset) override;
    void setRotationYawHead(float rotation) override;
    void onUpdate() override;
    void setInvisible(bool invisible) override;
    bool isChild() override;
    void onKillCommand() override;
    bool isImmuneToExplosions() override;
    EnumPushReaction getPushReaction() override;
    bool isSmall();
    bool getShowArms();
    bool hasNoBasePlate();
    bool hasMarker();
    void setHeadRotation(Rotations vec);
    void setBodyRotation(Rotations vec);
    void setLeftArmRotation(Rotations vec);
    void setRightArmRotation(Rotations vec);
    void setLeftLegRotation(Rotations vec);
    void setRightLegRotation(Rotations vec);
    Rotations getHeadRotation() const;
    Rotations getBodyRotation() const;
    Rotations getLeftArmRotation() const;
    Rotations getRightArmRotation() const;
    Rotations getLeftLegRotation() const;
    Rotations getRightLegRotation() const;
    bool canBeCollidedWith() override;
    EnumHandSide getPrimaryHand() override;
    void onStruckByLightning(EntityLightningBolt* lightningBolt) override;
    bool canBeHitWithPotion() override;
    void notifyDataManagerChange(DataParameter key) override;
    bool attackable() override;


    static void registerFixesArmorStand(DataFixer fixer);

    static const DataParameter STATUS;
    static const DataParameter HEAD_ROTATION;
    static const DataParameter BODY_ROTATION;
    static const DataParameter LEFT_ARM_ROTATION;
    static const DataParameter RIGHT_ARM_ROTATION;
    static const DataParameter LEFT_LEG_ROTATION;
    static const DataParameter RIGHT_LEG_ROTATION;
    int64_t punchCooldown;
protected:
    void setSize(float width, float height) override;
    void entityInit() override;
    void collideWithEntity(Entity* entityIn) override;
    void collideWithNearbyEntities() override;
    EntityEquipmentSlot getClickedSlot(Vec3d p_190772_1_);
    float updateDistance(float p_110146_1_, float p_110146_2_) override;
    void updatePotionMetadata() override;
    SoundEvent getFallSound(int32_t heightIn) override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;



private:
    void writePoseToNBT(NBTTagCompound* tagCompound);
    std::unique_ptr<NBTTagCompound> readPoseFromNBT() const;
    bool isDisabled(EntityEquipmentSlot slotIn) const;
    void swapItem(EntityPlayer* player, EntityEquipmentSlot p_184795_2_, ItemStack p_184795_3_, EnumHand hand);
    void playParticles();
    void damageArmorStand(float damage);
    void dropBlock();
    void dropContents();
    void playBrokenSound();
    void updateBoundingBox(bool p_181550_1_);
    void setSmall(bool small);
    void setShowArms(bool showArms);
    void setNoBasePlate(bool noBasePlate);
    void setMarker(bool marker);
    static std::byte setBit(std::byte p_184797_1_, uint8_t p_184797_2_, bool p_184797_3_);




    std::vector<ItemStack> handItems;
    std::vector<ItemStack> armorItems;
    bool canInteract;
    int32_t disabledSlots;
    bool wasMarker;
    Rotations headRotation;
    Rotations bodyRotation;
    Rotations leftArmRotation;
    Rotations rightArmRotation;
    Rotations leftLegRotation;
    Rotations rightLegRotation;
};
