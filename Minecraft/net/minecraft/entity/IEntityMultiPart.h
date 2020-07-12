namespace DamageSource {
    class DamageSource;
}

class World;

class IEntityMultiPart
{
    virtual World* getWorld() = 0;

    virtual bool attackEntityFromPart(MultiPartEntityPart var1, DamageSource::DamageSource var2, float var3) = 0;
};
