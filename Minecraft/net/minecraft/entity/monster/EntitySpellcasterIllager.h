#pragma once
#include "AbstractIllager.h"
#include "../ai/EntityAIBase.h"

class EntitySpellcasterIllager : public AbstractIllager {
public:
    class SpellType {
    public:
        static SpellType NONE;
        static SpellType SUMMON_VEX;
        static SpellType FANGS;
        static SpellType WOLOLO;
        static SpellType DISAPPEAR;
        static SpellType BLINDNESS;

        static EntitySpellcasterIllager::SpellType *getFromId(int32_t idIn);
        static std::vector<SpellType *> values();
        SpellType(int32_t idIn, double xParticleSpeed, double yParticleSpeed, double zParticleSpeed);
        int32_t getID() const;
        std::array<double, 3> getParticleSpeed() const;

    private:
        int32_t id;
        std::array<double, 3> particleSpeed;
        static std::vector<SpellType *> valu;
    };

    class AIUseSpell : public EntityAIBase {
    public:

        AIUseSpell(EntitySpellcasterIllager *entitySpellcasterIllagerIn);

        bool shouldExecute() override;

        bool shouldContinueExecuting() override;

        void startExecuting() override;

        void updateTask() override;

    protected:
        int32_t spellWarmup;
        int32_t spellCooldown;

        virtual void castSpell() = 0;

        virtual int32_t getCastWarmupTime();

        virtual int getCastingTime() = 0;

        virtual int getCastingInterval() = 0;

        virtual std::optional<SoundEvent> getSpellPrepareSound() = 0;

        virtual EntitySpellcasterIllager::SpellType getSpellType() = 0;
    public:
        EntitySpellcasterIllager *entitySpellcasterIllager;
    };

    class AICastingApell : public EntityAIBase {
    public:
        AICastingApell(EntitySpellcasterIllager *entitySpellcasterIllagerIn);

        bool shouldExecute() override;

        void startExecuting() override;

        void resetTask() override;

        void updateTask() override;
    private:
        EntitySpellcasterIllager *entitySpellcasterIllager;
    };

    EntitySpellcasterIllager(World* p_i47506_1_);
    void readEntityFromNBT(NBTTagCompound* compound) override;
    void writeEntityToNBT(NBTTagCompound* compound) override;
    AbstractIllager::IllagerArmPose getArmPose() override;
    bool isSpellcasting() const;
    void setSpellType(EntitySpellcasterIllager::SpellType spellType);
    void onUpdate() override;

protected:
    void entityInit() override;  
    EntitySpellcasterIllager::SpellType getSpellType();
    void updateAITasks() override;
    int32_t getSpellTicks() const;
    virtual  SoundEvent getSpellSound() = 0;

private:
    static DataParameter SPELL;
    int32_t spellTicks;
    EntitySpellcasterIllager::SpellType activeSpell;
};
