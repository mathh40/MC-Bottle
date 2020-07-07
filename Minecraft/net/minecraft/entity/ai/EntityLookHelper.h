class Entity;
class EntityLiving;
class EntityLookHelper
{
public:
    explicit EntityLookHelper(EntityLiving* entitylivingIn);
    void setLookPositionWithEntity(Entity* entityIn, float deltaYaw, float deltaPitch);
    void setLookPosition(double x, double y, double z, float deltaYaw, float deltaPitch);
    void onUpdateLook();
    bool getIsLooking() const;
    double getLookPosX() const;
    double getLookPosY() const;
    double getLookPosZ() const;

private:

    float updateRotation(float p_75652_1_, float p_75652_2_, float p_75652_3_) const;

    EntityLiving* entity;
    float deltaLookYaw;
    float deltaLookPitch;
    bool isLooking;
    double posX;
    double posY;
    double posZ;
};
