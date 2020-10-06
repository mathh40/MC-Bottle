#pragma once
#include "../Entity.h"

class DataParameter;
class EntityEnderCrystal :public Entity
{
  public:
      EntityEnderCrystal(World* worldIn);
      EntityEnderCrystal(World* worldIn, double x, double y, double z);
      void onUpdate() override;
      bool canBeCollidedWith() override;
      bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
      void onKillCommand() override;
      void setBeamTarget(std::optional<BlockPos> beamTarget);
      std::optional<BlockPos> getBeamTarget();
      void setShowBottom(bool showBottom);
      bool shouldShowBottom();
      bool isInRangeToRenderDist(double distance) override;

      int32_t innerRotation;
protected:
    bool canTriggerWalking() override;
    void entityInit() override;
    void writeEntityToNBT(NBTTagCompound* compound) override;
    void readEntityFromNBT(NBTTagCompound* compound) override;


  private:

    void onCrystalDestroyed(DamageSource::DamageSource source);

    static const DataParameter BEAM_TARGET;
    static const DataParameter SHOW_BOTTOM;
};