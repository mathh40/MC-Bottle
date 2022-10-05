class EntityLiving;
class EntityJumpHelper
{
  public:
    explicit EntityJumpHelper(EntityLiving *entityIn);
    void setJumping();
    void doJump();

  protected:
    bool isJumping;

  private:
    EntityLiving *entity;
};
