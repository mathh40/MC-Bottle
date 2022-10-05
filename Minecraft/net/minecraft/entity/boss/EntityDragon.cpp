#include "EntityDragon.h"

#include "../../world/WorldProviderEnd.h"
#include "../../world/gen/feature/WorldGenEndPodium.h"
#include "EntityDamageSource.h"
#include "SharedMonsterAttributes.h"
#include "dragon/phase/IPhase.h"
#include "item/EntityEnderCrystal.h"
#include "item/EntityXPOrb.h"

std::shared_ptr<spdlog::logger> EntityDragon::LOGGER = spdlog::get("Minecraft")->clone("EntityDragon");
DataParameter EntityDragon::PHASE = EntityDataManager::createKey(EntityDragon.class, DataSerializers::VARINT);

EntityDragon::EntityDragon(World *worldIn) : EntityLiving(worldIn), growlTime(100), phaseManager(this)
{
    setHealth(getMaxHealth());
    setSize(16.0F, 8.0F);
    noClip             = true;
    bisImmuneToFire    = true;
    ignoreFrustumCheck = true;
    if (!worldIn->isRemote && Util:: instanceof <WorldProviderEnd>(worldIn->provider))
    {
        fightManager = ((WorldProviderEnd *)worldIn->provider)->getDragonFightManager();
    }
    else
    {
        fightManager = std::nullopt;
    }
}

std::array<double, 3> EntityDragon::getMovementOffsets(int32_t p_70974_1_, float p_70974_2_)
{
    if (getHealth() <= 0.0F)
    {
        p_70974_2_ = 0.0F;
    }

    p_70974_2_ = 1.0F - p_70974_2_;
    int i      = ringBufferIndex - p_70974_1_ & 63;
    int j      = ringBufferIndex - p_70974_1_ - 1 & 63;
    std::array<double, 3> adouble;
    double d0  = ringBuffer[i][0];
    double d1  = MathHelper::wrapDegrees(ringBuffer[j][0] - d0);
    adouble[0] = d0 + d1 * (double)p_70974_2_;
    d0         = ringBuffer[i][1];
    d1         = ringBuffer[j][1] - d0;
    adouble[1] = d0 + d1 * (double)p_70974_2_;
    adouble[2] = ringBuffer[i][2] + (ringBuffer[j][2] - ringBuffer[i][2]) * (double)p_70974_2_;
    return adouble;
}

void EntityDragon::onLivingUpdate()
{
    float f11;
    float f1;
    if (world->isRemote)
    {
        setHealth(getHealth());
        if (!isSilent())
        {
            f11 = MathHelper::cos(animTime * 6.2831855F);
            f1  = MathHelper::cos(prevAnimTime * 6.2831855F);
            if (f1 <= -0.3F && f11 >= -0.3F)
            {
                world->playSound(posX, posY, posZ, SoundEvents::ENTITY_ENDERDRAGON_FLAP, getSoundCategory(), 5.0F,
                                 0.8F + MathHelper::nextFloat(rand) * 0.3F, false);
            }

            if (!phaseManager.getCurrentPhase().getIsStationary() && --growlTime < 0)
            {
                world->playSound(posX, posY, posZ, SoundEvents::ENTITY_ENDERDRAGON_GROWL, getSoundCategory(), 2.5F,
                                 0.8F + MathHelper::nextFloat(rand) * 0.3F, false);
                growlTime = 200 + rand(200);
            }
        }
    }

    prevAnimTime = animTime;
    float f14;
    if (getHealth() <= 0.0F)
    {
        f11 = (MathHelper::nextFloat(rand) - 0.5F) * 8.0F;
        f1  = (MathHelper::nextFloat(rand) - 0.5F) * 4.0F;
        f14 = (MathHelper::nextFloat(rand) - 0.5F) * 8.0F;
        world->spawnParticle(EnumParticleTypes::EXPLOSION_LARGE, posX + (double)f11, posY + 2.0 + (double)f1,
                             posZ + (double)f14, 0.0, 0.0, 0.0, {});
    }
    else
    {
        updateDragonEnderCrystal();
        f11 = 0.2F / (MathHelper::sqrt(motionX * motionX + motionZ * motionZ) * 10.0F + 1.0F);
        f11 *= (float)MathHelper::pow(2.0, motionY);
        if (phaseManager.getCurrentPhase().getIsStationary())
        {
            animTime += 0.1F;
        }
        else if (slowed)
        {
            animTime += f11 * 0.5F;
        }
        else
        {
            animTime += f11;
        }

        rotationYaw = MathHelper::wrapDegrees(rotationYaw);
        if (isAIDisabled())
        {
            animTime = 0.5F;
        }
        else
        {
            if (ringBufferIndex < 0)
            {
                for (int i = 0; i < ringBuffer.size(); ++i)
                {
                    ringBuffer[i][0] = (double)rotationYaw;
                    ringBuffer[i][1] = posY;
                }
            }

            if (++ringBufferIndex == ringBuffer.size())
            {
                ringBufferIndex = 0;
            }

            ringBuffer[ringBufferIndex][0] = (double)rotationYaw;
            ringBuffer[ringBufferIndex][1] = posY;
            double d6;
            double d7;
            double d8;
            float f20;
            float f6;
            float f22;
            float f23;
            if (world->isRemote)
            {
                if (newPosRotationIncrements > 0)
                {
                    double d5     = posX + (interpTargetX - posX) / (double)newPosRotationIncrements;
                    d6            = posY + (interpTargetY - posY) / (double)newPosRotationIncrements;
                    d7            = posZ + (interpTargetZ - posZ) / (double)newPosRotationIncrements;
                    d8            = MathHelper::wrapDegrees(interpTargetYaw - (double)rotationYaw);
                    rotationYaw   = (float)((double)rotationYaw + d8 / (double)newPosRotationIncrements);
                    rotationPitch = (float)((double)rotationPitch + (interpTargetPitch - (double)rotationPitch) /
                                                                        (double)newPosRotationIncrements);
                    --newPosRotationIncrements;
                    setPosition(d5, d6, d7);
                    setRotation(rotationYaw, rotationPitch);
                }

                phaseManager.getCurrentPhase().doClientRenderEffects();
            }
            else
            {
                auto iphase = phaseManager.getCurrentPhase();
                iphase->doLocalUpdate();
                if (phaseManager.getCurrentPhase() != iphase)
                {
                    iphase = phaseManager.getCurrentPhase();
                    iphase->doLocalUpdate();
                }

                auto vec3d = iphase->getTargetLocation();
                if (vec3d != nullptr)
                {
                    d6        = vec3d.getx() - posX;
                    d7        = vec3d.gety() - posY;
                    d8        = vec3d.getz() - posZ;
                    double d3 = d6 * d6 + d7 * d7 + d8 * d8;
                    f20       = iphase->getMaxRiseOrFall();
                    d7        = MathHelper::clamp(d7 / (double)MathHelper::sqrt(d6 * d6 + d8 * d8), (double)(-f20),
                                                  (double)f20);
                    motionY += d7 * 0.10000000149011612;
                    rotationYaw = MathHelper::wrapDegrees(rotationYaw);
                    double d4   = MathHelper::clamp(
                          MathHelper::wrapDegrees(180.0 - MathHelper::atan2(d6, d8) * 57.29577951308232 -
                                                  (double)rotationYaw),
                          -50.0, 50.0);
                    Vec3d vec3d1 = (Vec3d(vec3d.getx() - posX, vec3d.gety() - posY, vec3d.getz() - posZ)).normalize();
                    Vec3d vec3d2 = (Vec3d((double)MathHelper::sin(rotationYaw * 0.017453292F), motionY,
                                          (double)(-MathHelper::cos(rotationYaw * 0.017453292F))))
                                       .normalize();
                    f6 = MathHelper::max(((float)vec3d2.dotProduct(vec3d1) + 0.5F) / 1.5F, 0.0F);
                    randomYawVelocity *= 0.8F;
                    randomYawVelocity = (float)((double)randomYawVelocity + d4 * (double)iphase->getYawFactor());
                    rotationYaw += randomYawVelocity * 0.1F;
                    f22 = (float)(2.0 / (d3 + 1.0));
                    f23 = 0.06F;
                    moveRelative(0.0F, 0.0F, -1.0F, 0.06F * (f6 * f22 + (1.0F - f22)));
                    if (slowed)
                    {
                        move(MoverType::SELF, motionX * 0.800000011920929, motionY * 0.800000011920929,
                             motionZ * 0.800000011920929);
                    }
                    else
                    {
                        move(MoverType::SELF, motionX, motionY, motionZ);
                    }

                    Vec3d vec3d3 = (Vec3d(motionX, motionY, motionZ)).normalize();
                    float f10    = ((float)vec3d3.dotProduct(vec3d2) + 1.0F) / 2.0F;
                    f10          = 0.8F + 0.15F * f10;
                    motionX *= (double)f10;
                    motionZ *= (double)f10;
                    motionY *= 0.9100000262260437;
                }
            }

            renderYawOffset        = rotationYaw;
            dragonPartHead.width   = 1.0F;
            dragonPartHead.height  = 1.0F;
            dragonPartNeck.width   = 3.0F;
            dragonPartNeck.height  = 3.0F;
            dragonPartTail1.width  = 2.0F;
            dragonPartTail1.height = 2.0F;
            dragonPartTail2.width  = 2.0F;
            dragonPartTail2.height = 2.0F;
            dragonPartTail3.width  = 2.0F;
            dragonPartTail3.height = 2.0F;
            dragonPartBody.height  = 3.0F;
            dragonPartBody.width   = 5.0F;
            dragonPartWing1.height = 2.0F;
            dragonPartWing1.width  = 4.0F;
            dragonPartWing2.height = 3.0F;
            dragonPartWing2.width  = 4.0F;
            std::array<Vec3d, 8> avec3d;

            for (auto j = 0; j < dragonPartArray.size(); ++j)
            {
                avec3d[j] = Vec3d(dragonPartArray[j].posX, dragonPartArray[j].posY, dragonPartArray[j].posZ);
            }

            f14 = (float)(getMovementOffsets(5, 1.0F)[1] - getMovementOffsets(10, 1.0F)[1]) * 10.0F * 0.017453292F;
            float f16 = MathHelper::cos(f14);
            float f2  = MathHelper::sin(f14);
            float f17 = rotationYaw * 0.017453292F;
            float f3  = MathHelper::sin(f17);
            float f18 = MathHelper::cos(f17);
            dragonPartBody.onUpdate();
            dragonPartBody.setLocationAndAngles(posX + (double)(f3 * 0.5F), posY, posZ - (double)(f18 * 0.5F), 0.0F,
                                                0.0F);
            dragonPartWing1.onUpdate();
            dragonPartWing1.setLocationAndAngles(posX + (double)(f18 * 4.5F), posY + 2.0, posZ + (double)(f3 * 4.5F),
                                                 0.0F, 0.0F);
            dragonPartWing2.onUpdate();
            dragonPartWing2.setLocationAndAngles(posX - (double)(f18 * 4.5F), posY + 2.0, posZ - (double)(f3 * 4.5F),
                                                 0.0F, 0.0F);
            if (!world->isRemote && hurtTime == 0)
            {
                collideWithEntities(world->getEntitiesWithinAABBExcludingEntity(
                    this, dragonPartWing1.getEntityBoundingBox().grow(4.0, 2.0, 4.0).offset(0.0, -2.0, 0.0)));
                collideWithEntities(world->getEntitiesWithinAABBExcludingEntity(
                    this, dragonPartWing2.getEntityBoundingBox().grow(4.0, 2.0, 4.0).offset(0.0, -2.0, 0.0)));
                attackEntitiesInList(
                    world->getEntitiesWithinAABBExcludingEntity(this, dragonPartHead.getEntityBoundingBox().grow(1.0)));
                attackEntitiesInList(
                    world->getEntitiesWithinAABBExcludingEntity(this, dragonPartNeck.getEntityBoundingBox().grow(1.0)));
            }

            auto adouble = getMovementOffsets(5, 1.0F);
            float f19    = MathHelper::sin(rotationYaw * 0.017453292F - randomYawVelocity * 0.01F);
            float f4     = MathHelper::cos(rotationYaw * 0.017453292F - randomYawVelocity * 0.01F);
            dragonPartHead.onUpdate();
            dragonPartNeck.onUpdate();
            f20 = getHeadYOffset(1.0F);
            dragonPartHead.setLocationAndAngles(posX + (double)(f19 * 6.5F * f16),
                                                posY + (double)f20 + (double)(f2 * 6.5F),
                                                posZ - (double)(f4 * 6.5F * f16), 0.0F, 0.0F);
            dragonPartNeck.setLocationAndAngles(posX + (double)(f19 * 5.5F * f16),
                                                posY + (double)f20 + (double)(f2 * 5.5F),
                                                posZ - (double)(f4 * 5.5F * f16), 0.0F, 0.0F);

            int k;
            for (k = 0; k < 3; ++k)
            {
                MultiPartEntityPart *multipartentitypart;
                if (k == 0)
                {
                    multipartentitypart = &dragonPartTail1;
                }

                if (k == 1)
                {
                    multipartentitypart = &dragonPartTail2;
                }

                if (k == 2)
                {
                    multipartentitypart = &dragonPartTail3;
                }

                auto adouble1 = getMovementOffsets(12 + k * 2, 1.0F);
                float f21     = rotationYaw * 0.017453292F + simplifyAngle(adouble1[0] - adouble[0]) * 0.017453292F;
                f6            = MathHelper::sin(f21);
                f22           = MathHelper::cos(f21);
                f23           = 1.5F;
                float f24     = (float)(k + 1) * 2.0F;
                multipartentitypart->onUpdate();
                multipartentitypart->setLocationAndAngles(posX - (double)((f3 * 1.5F + f6 * f24) * f16),
                                                          posY + (adouble1[1] - adouble[1]) -
                                                              (double)((f24 + 1.5F) * f2) + 1.5,
                                                          posZ + (double)((f18 * 1.5F + f22 * f24) * f16), 0.0F, 0.0F);
            }

            if (!world->isRemote)
            {
                slowed = destroyBlocksInAABB(dragonPartHead.getEntityBoundingBox()) |
                         destroyBlocksInAABB(dragonPartNeck.getEntityBoundingBox()) |
                         destroyBlocksInAABB(dragonPartBody.getEntityBoundingBox());
                if (fightManager.has_value())
                {
                    fightManager->dragonUpdate(this);
                }
            }

            for (k = 0; k < dragonPartArray.size(); ++k)
            {
                dragonPartArray[k].prevPosX = avec3d[k].getx();
                dragonPartArray[k].prevPosY = avec3d[k].gety();
                dragonPartArray[k].prevPosZ = avec3d[k].getz();
            }
        }
    }
}

bool EntityDragon::attackEntityFromPart(MultiPartEntityPart *part, DamageSource::DamageSource source, float damage)
{
    damage = phaseManager.getCurrentPhase().getAdjustedDamage(part, source, damage);
    if (*part != dragonPartHead)
    {
        damage = damage / 4.0F + MathHelper::min(damage, 1.0F);
    }

    if (damage < 0.01F)
    {
        return false;
    }
    else
    {
        if (Util:: instanceof <EntityPlayer>(source.getTrueSource()) || source.isExplosion())
        {
            float f = getHealth();
            attackDragonFrom(source, damage);
            if (getHealth() <= 0.0F && !phaseManager.getCurrentPhase().getIsStationary())
            {
                setHealth(1.0F);
                phaseManager.setPhase(PhaseList::DYING);
            }

            if (phaseManager.getCurrentPhase().getIsStationary())
            {
                sittingDamageReceived = (int)((float)sittingDamageReceived + (f - getHealth()));
                if ((float)sittingDamageReceived > 0.25F * getMaxHealth())
                {
                    sittingDamageReceived = 0;
                    phaseManager.setPhase(PhaseList::TAKEOFF);
                }
            }
        }

        return true;
    }
}

bool EntityDragon::attackEntityFrom(DamageSource::DamageSource source, float amount)
{
    if (Util:: instanceof <DamageSource::EntityDamageSource>(source) &&
                              ((DamageSource::EntityDamageSource)source).getIsThornsDamage())
    {
        attackEntityFromPart(dragonPartBody, source, amount);
    }

    return false;
}

void EntityDragon::onKillCommand()
{
    setDead();
    if (fightManager.has_value())
    {
        fightManager->dragonUpdate(this);
        fightManager->processDragonDeath(this);
    }
}

int32_t EntityDragon::initPathPoints()
{
    if (pathPoints[0] == std::nullopt)
    {
        for (int i = 0; i < 24; ++i)
        {
            int j = 5;
            int l;
            int i1;
            int lvt_3_1_;
            if (i < 12)
            {
                l  = (int)(60.0F * MathHelper::cos(2.0F * (-3.1415927F + 0.2617994F * (float)i)));
                i1 = (int)(60.0F * MathHelper::sin(2.0F * (-3.1415927F + 0.2617994F * (float)i)));
            }
            else if (i < 20)
            {
                lvt_3_1_ = i - 12;
                l        = (int)(40.0F * MathHelper::cos(2.0F * (-3.1415927F + 0.3926991F * (float)lvt_3_1_)));
                i1       = (int)(40.0F * MathHelper::sin(2.0F * (-3.1415927F + 0.3926991F * (float)lvt_3_1_)));
                j += 10;
            }
            else
            {
                lvt_3_1_ = i - 20;
                l        = (int)(20.0F * MathHelper::cos(2.0F * (-3.1415927F + 0.7853982F * (float)lvt_3_1_)));
                i1       = (int)(20.0F * MathHelper::sin(2.0F * (-3.1415927F + 0.7853982F * (float)lvt_3_1_)));
            }

            lvt_3_1_      = MathHelper::max(world->getSeaLevel() + 10,
                                            world->getTopSolidOrLiquidBlock(BlockPos(l, 0, i1)).gety() + j);
            pathPoints[i] = PathPoint(l, lvt_3_1_, i1);
        }

        neighbors[0]  = 6146;
        neighbors[1]  = 8197;
        neighbors[2]  = 8202;
        neighbors[3]  = 16404;
        neighbors[4]  = 32808;
        neighbors[5]  = 32848;
        neighbors[6]  = 65696;
        neighbors[7]  = 131392;
        neighbors[8]  = 131712;
        neighbors[9]  = 263424;
        neighbors[10] = 526848;
        neighbors[11] = 525313;
        neighbors[12] = 1581057;
        neighbors[13] = 3166214;
        neighbors[14] = 2138120;
        neighbors[15] = 6373424;
        neighbors[16] = 4358208;
        neighbors[17] = 12910976;
        neighbors[18] = 9044480;
        neighbors[19] = 9706496;
        neighbors[20] = 15216640;
        neighbors[21] = 13688832;
        neighbors[22] = 11763712;
        neighbors[23] = 8257536;
    }

    return getNearestPpIdx(posX, posY, posZ);
}

int32_t EntityDragon::getNearestPpIdx(double x, double y, double z)
{
    float f             = 10000.0F;
    auto i              = 0;
    PathPoint pathpoint = PathPoint(MathHelper::floor(x), MathHelper::floor(y), MathHelper::floor(z));
    auto j              = 0;
    if (fightManager == std::nullopt || fightManager->getNumAliveCrystals() == 0)
    {
        j = 12;
    }

    for (auto k = j; k < 24; ++k)
    {
        if (pathPoints[k].has_value())
        {
            float f1 = pathPoints[k]->distanceToSquared(pathpoint);
            if (f1 < f)
            {
                f = f1;
                i = k;
            }
        }
    }

    return i;
}

std::optional<Path> EntityDragon::findPath(int32_t startIdx, int32_t finishIdx, std::optional<PathPoint> andThen)
{
    std::optional<PathPoint> pathpoint5;
    for (auto i = 0; i < 24; ++i)
    {
        pathpoint5                    = pathPoints[i];
        pathpoint5->visited           = false;
        pathpoint5->distanceToTarget  = 0.0F;
        pathpoint5->totalPathDistance = 0.0F;
        pathpoint5->distanceToNext    = 0.0F;
        pathpoint5->previous          = nullptr;
        pathpoint5->index             = -1;
    }

    std::optional<PathPoint> pathpoint4 = pathPoints[startIdx];
    pathpoint5                          = pathPoints[finishIdx];
    pathpoint4->totalPathDistance       = 0.0F;
    pathpoint4->distanceToNext          = pathpoint4->distanceTo(pathpoint5);
    pathpoint4->distanceToTarget        = pathpoint4->distanceToNext;
    pathFindQueue.clearPath();
    pathFindQueue.addPoint(*pathpoint4);
    std::optional<PathPoint> pathpoint1 = pathpoint4;
    int j                               = 0;
    if (fightManager == std::nullopt || fightManager->getNumAliveCrystals() == 0)
    {
        j = 12;
    }

    while (!pathFindQueue.isPathEmpty())
    {
        std::optional<PathPoint> pathpoint2 = pathFindQueue.dequeue();
        if (pathpoint2 == pathpoint5)
        {
            if (andThen.has_value())
            {
                andThen->previous = &*pathpoint5;
                pathpoint5        = andThen;
            }

            return makePath(pathpoint4, pathpoint5);
        }

        if (pathpoint2->distanceTo(*pathpoint5) < pathpoint1->distanceTo(*pathpoint5))
        {
            pathpoint1 = pathpoint2;
        }

        pathpoint2->visited = true;
        int k               = 0;

        int i1;
        for (i1 = 0; i1 < 24; ++i1)
        {
            if (pathPoints[i1] == pathpoint2)
            {
                k = i1;
                break;
            }
        }

        for (i1 = j; i1 < 24; ++i1)
        {
            if ((neighbors[k] & 1 << i1) > 0)
            {
                std::optional<PathPoint> pathpoint3 = pathPoints[i1];
                if (!pathpoint3->visited)
                {
                    float f = pathpoint2->totalPathDistance + pathpoint2->distanceTo(*pathpoint3);
                    if (!pathpoint3->isAssigned() || f < pathpoint3->totalPathDistance)
                    {
                        pathpoint3->previous          = &*pathpoint2;
                        pathpoint3->totalPathDistance = f;
                        pathpoint3->distanceToNext    = pathpoint3->distanceTo(*pathpoint5);
                        if (pathpoint3->isAssigned())
                        {
                            pathFindQueue.changeDistance(*pathpoint3,
                                                         pathpoint3->totalPathDistance + pathpoint3->distanceToNext);
                        }
                        else
                        {
                            pathpoint3->distanceToTarget = pathpoint3->totalPathDistance + pathpoint3->distanceToNext;
                            pathFindQueue.addPoint(*pathpoint3);
                        }
                    }
                }
            }
        }
    }

    if (pathpoint1 == pathpoint4)
    {
        return std::nullopt;
    }
    else
    {
        LOGGER->debug("Failed to find path from {} to {}", startIdx, finishIdx);
        if (andThen.has_value())
        {
            andThen->previous = &*pathpoint1;
            pathpoint1        = andThen;
        }

        return makePath(pathpoint4, pathpoint1);
    }
}

void EntityDragon::registerFixesDragon(DataFixer fixer)
{
    EntityLiving::registerFixesMob(fixer, EntityDragon.class);
}

void EntityDragon::writeEntityToNBT(NBTTagCompound *compound)
{
    EntityLiving::writeEntityToNBT(compound);
    compound->setInteger("DragonPhase", phaseManager.getCurrentPhase().->getType().getId());
}

void EntityDragon::readEntityFromNBT(NBTTagCompound *compound)
{
    EntityLiving::readEntityFromNBT(compound);
    if (compound->hasKey("DragonPhase"))
    {
        phaseManager.setPhase(PhaseList::getById(compound->getInteger("DragonPhase")));
    }
}

std::vector<Entity *> EntityDragon::getParts()
{
    std::vector<Entity *> parts(dragonPartArray.begin(), dragonPartArray.end());
    return dragonPartArray;
}

bool EntityDragon::canBeCollidedWith()
{
    return false;
}

World *EntityDragon::getWorld()
{
    return world;
}

SoundCategory EntityDragon::getSoundCategory()
{
    return SoundCategory::HOSTILE;
}

float EntityDragon::getHeadPartYOffset(int32_t p_184667_1_, double p_184667_2_[], double p_184667_3_[])
{
    IPhase *iphase      = phaseManager.getCurrentPhase();
    PhaseList phaselist = iphase->getType();
    double d0;
    if (phaselist != PhaseList::LANDING && phaselist != PhaseList::TAKEOFF)
    {
        if (iphase->getIsStationary())
        {
            d0 = (double)p_184667_1_;
        }
        else if (p_184667_1_ == 6)
        {
            d0 = 0.0;
        }
        else
        {
            d0 = p_184667_3_[1] - p_184667_2_[1];
        }
    }
    else
    {
        BlockPos blockpos = world->getTopSolidOrLiquidBlock(WorldGenEndPodium::END_PODIUM_LOCATION);
        float f           = MathHelper::max(MathHelper::sqrt(getDistanceSqToCenter(blockpos)) / 4.0, 1.0);
        d0                = (double)((float)p_184667_1_ / f);
    }

    return (float)d0;
}

Vec3d EntityDragon::getHeadLookVec(float p_184665_1_)
{
    IPhase *iphase      = phaseManager.getCurrentPhase();
    PhaseList phaselist = iphase->getType();
    Vec3d vec3d;
    float f5;
    if (phaselist != PhaseList::LANDING && phaselist != PhaseList::TAKEOFF)
    {
        if (iphase->getIsStationary())
        {
            float f4      = rotationPitch;
            f5            = 1.5F;
            rotationPitch = -45.0F;
            vec3d         = getLook(p_184665_1_);
            rotationPitch = f4;
        }
        else
        {
            vec3d = getLook(p_184665_1_);
        }
    }
    else
    {
        BlockPos blockpos = world->getTopSolidOrLiquidBlock(WorldGenEndPodium::END_PODIUM_LOCATION);
        f5                = MathHelper::max(MathHelper::sqrt(getDistanceSqToCenter(blockpos)) / 4.0, 1.0);
        float f1          = 6.0F / f5;
        float f2          = rotationPitch;
        float f3          = 1.5F;
        rotationPitch     = -f1 * 1.5F * 5.0F;
        vec3d             = getLook(p_184665_1_);
        rotationPitch     = f2;
    }

    return vec3d;
}

void EntityDragon::onCrystalDestroyed(EntityEnderCrystal *crystal, const BlockPos &pos,
                                      DamageSource::DamageSource dmgSrc)
{
    EntityPlayer *entityplayer;
    if (Util:: instanceof <EntityPlayer>(dmgSrc.getTrueSource()))
    {
        entityplayer = (EntityPlayer *)dmgSrc.getTrueSource();
    }
    else
    {
        entityplayer = world->getNearestAttackablePlayer(pos, 64.0, 64.0);
    }

    if (crystal == healingEnderCrystal)
    {
        attackEntityFromPart(dragonPartHead, DamageSource::causeExplosionDamage((EntityLivingBase *)entityplayer),
                             10.0F);
    }

    phaseManager.getCurrentPhase().onCrystalDestroyed(crystal, pos, dmgSrc, entityplayer);
}

void EntityDragon::notifyDataManagerChange(DataParameter key)
{
    if (PHASE::equals(key) && world->isRemote)
    {
        phaseManager.setPhase(PhaseList::getById(getDataManager().get(PHASE)));
    }

    EntityLiving::notifyDataManagerChange(key);
}

const std::optional<DragonFightManager> EntityDragon::getFightManager() const
{
    return fightManager;
}

void EntityDragon::addPotionEffect(PotionEffect potioneffectIn)
{
}

bool EntityDragon::isNonBoss()
{
    return false;
}

Path EntityDragon::makePath(PathPoint start, PathPoint finish)
{
    int i = 1;

    for (PathPoint pathpoint = finish; pathpoint.previous != nullptr; pathpoint = pathpoint.previous)
    {
        ++i;
    }

    std::vector<PathPoint> apathpoint = std::vector<PathPoint>(i);
    PathPoint pathpoint1              = finish;
    --i;

    for (apathpoint[i] = finish; pathpoint1.previous != nullptr; apathpoint[i] = pathpoint1)
    {
        pathpoint1 = pathpoint1.previous;
        --i;
    }

    return Path(apathpoint);
}

void EntityDragon::applyEntityAttributes()
{
    EntityLiving::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(200.0);
}

void EntityDragon::entityInit()
{
    EntityLiving::entityInit();
    getDataManager().registe(PHASE, PhaseList::HOVER.getId());
}

bool EntityDragon::attackDragonFrom(DamageSource::DamageSource source, float amount)
{
    return EntityLiving::attackEntityFrom(source, amount);
}

void EntityDragon::onDeathUpdate()
{
    if (fightManager.has_value())
    {
        fightManager->dragonUpdate(this);
    }

    ++deathTicks;
    if (deathTicks >= 180 && deathTicks <= 200)
    {
        float f  = (MathHelper::nextFloat(rand) - 0.5F) * 8.0F;
        float f1 = (MathHelper::nextFloat(rand) - 0.5F) * 4.0F;
        float f2 = (MathHelper::nextFloat(rand) - 0.5F) * 8.0F;
        world->spawnParticle(EnumParticleTypes::EXPLOSION_HUGE, posX + (double)f, posY + 2.0 + (double)f1,
                             posZ + (double)f2, 0.0, 0.0, 0.0, {});
    }

    bool flag = world->getGameRules().getBoolean("doMobLoot");
    int i     = 500;
    if (fightManager.has_value() && !fightManager->hasPreviouslyKilledDragon())
    {
        i = 12000;
    }

    if (!world->isRemote)
    {
        if (deathTicks > 150 && deathTicks % 5 == 0 && flag)
        {
            dropExperience(MathHelper::floor((float)i * 0.08F));
        }

        if (deathTicks == 1)
        {
            world->playBroadcastSound(1028, BlockPos(this), 0);
        }
    }

    move(MoverType::SELF, 0.0, 0.10000000149011612, 0.0);
    rotationYaw += 20.0F;
    renderYawOffset = rotationYaw;
    if (deathTicks == 200 && !world->isRemote)
    {
        if (flag)
        {
            dropExperience(MathHelper::floor((float)i * 0.2F));
        }

        if (fightManager.has_value())
        {
            fightManager->processDragonDeath(this);
        }

        setDead();
    }
}

void EntityDragon::despawnEntity()
{
}

SoundEvent EntityDragon::getAmbientSound()
{
    return SoundEvents::ENTITY_ENDERDRAGON_AMBIENT;
}

SoundEvent EntityDragon::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    return SoundEvents::ENTITY_ENDERDRAGON_HURT;
}

float EntityDragon::getSoundVolume()
{
    return 5.0F;
}

ResourceLocation EntityDragon::getLootTable()
{
    return LootTableList::ENTITIES_ENDER_DRAGON;
}

bool EntityDragon::canBeRidden(Entity *entityIn)
{
    return false;
}

float EntityDragon::getHeadYOffset(float p_184662_1_)
{
    double d0;
    if (phaseManager.getCurrentPhase().getIsStationary())
    {
        d0 = -1.0;
    }
    else
    {
        auto adouble  = getMovementOffsets(5, 1.0F);
        auto adouble1 = getMovementOffsets(0, 1.0F);
        d0            = adouble[1] - adouble1[1];
    }

    return (float)d0;
}

void EntityDragon::updateDragonEnderCrystal()
{
    if (healingEnderCrystal != nullptr)
    {
        if (healingEnderCrystal->isDead)
        {
            healingEnderCrystal = nullptr;
        }
        else if (ticksExisted % 10 == 0 && getHealth() < getMaxHealth())
        {
            setHealth(getHealth() + 1.0F);
        }
    }

    if (rand(10) == 0)
    {
        auto list = world->getEntitiesWithinAABB<EntityEnderCrystal>(getEntityBoundingBox().grow(32.0));
        EntityEnderCrystal *entityendercrystal = nullptr;
        double d0                              = std::numeric_limits<double>::max();

        for (auto entityendercrystal1 : list)
        {
            double d1 = entityendercrystal1->getDistanceSq(this);
            if (d1 < d0)
            {
                d0                 = d1;
                entityendercrystal = entityendercrystal1;
            }
        }

        healingEnderCrystal = entityendercrystal;
    }
}

void EntityDragon::collideWithEntities(const std::vector<Entity *> &p_70970_1_)
{
    double d0 =
        (dragonPartBody.getEntityBoundingBox().getminX() + dragonPartBody.getEntityBoundingBox().getmaxX()) / 2.0;
    double d1 =
        (dragonPartBody.getEntityBoundingBox().getminZ() + dragonPartBody.getEntityBoundingBox().getmaxZ()) / 2.0;

    for (auto entity : p_70970_1_)
    {
        if (Util:: instanceof <EntityLivingBase>(entity))
        {
            double d2 = entity->posX - d0;
            double d3 = entity->posZ - d1;
            double d4 = d2 * d2 + d3 * d3;
            entity->addVelocity(d2 / d4 * 4.0, 0.20000000298023224, d3 / d4 * 4.0);
            if (!phaseManager.getCurrentPhase().getIsStationary() &&
                ((EntityLivingBase *)entity)->getRevengeTimer() < entity->ticksExisted - 2)
            {
                entity->attackEntityFrom(DamageSource::causeMobDamage(this), 5.0F);
                applyEnchantments(this, entity);
            }
        }
    }
}

void EntityDragon::attackEntitiesInList(const std::vector<Entity *> &p_70971_1_)
{
    for (auto entity : p_70971_1_)
    {
        if (Util:: instanceof <EntityLivingBase>(entity))
        {
            entity->attackEntityFrom(DamageSource::causeMobDamage(this), 10.0F);
            applyEnchantments(this, entity);
        }
    }
}

float EntityDragon::simplifyAngle(double p_70973_1_)
{
    return MathHelper::wrapDegrees(p_70973_1_);
}

bool EntityDragon::destroyBlocksInAABB(const AxisAlignedBB &p_70972_1_)
{
    auto i     = MathHelper::floor(p_70972_1_.getminX());
    auto j     = MathHelper::floor(p_70972_1_.getminY());
    auto k     = MathHelper::floor(p_70972_1_.getminZ());
    auto l     = MathHelper::floor(p_70972_1_.getmaxX());
    auto i1    = MathHelper::floor(p_70972_1_.getmaxY());
    auto j1    = MathHelper::floor(p_70972_1_.getmaxZ());
    auto flag  = false;
    auto flag1 = false;

    for (auto k1 = i; k1 <= l; ++k1)
    {
        for (auto l1 = j; l1 <= i1; ++l1)
        {
            for (auto i2 = k; i2 <= j1; ++i2)
            {
                BlockPos blockpos(k1, l1, i2);
                IBlockState *iblockstate = world->getBlockState(blockpos);
                Block *block             = iblockstate->getBlock();
                if (iblockstate->getMaterial() != Material::AIR && iblockstate->getMaterial() != Material::FIRE)
                {
                    if (!world->getGameRules().getBoolean("mobGriefing"))
                    {
                        flag = true;
                    }
                    else if (block != Blocks::BARRIER && block != Blocks::OBSIDIAN && block != Blocks::END_STONE &&
                             block != Blocks::BEDROCK && block != Blocks::END_PORTAL &&
                             block != Blocks::END_PORTAL_FRAME)
                    {
                        if (block != Blocks::COMMAND_BLOCK && block != Blocks::REPEATING_COMMAND_BLOCK &&
                            block != Blocks::CHAIN_COMMAND_BLOCK && block != Blocks::IRON_BARS &&
                            block != Blocks::END_GATEWAY)
                        {
                            flag1 = world->setBlockToAir(blockpos) || flag1;
                        }
                        else
                        {
                            flag = true;
                        }
                    }
                    else
                    {
                        flag = true;
                    }
                }
            }
        }
    }

    if (flag1)
    {
        double d0 = p_70972_1_.getminX() + (p_70972_1_.getmaxX() - p_70972_1_.getminX()) * MathHelper::nextDouble(rand);
        double d1 = p_70972_1_.getminY() + (p_70972_1_.getmaxY() - p_70972_1_.getminY()) * MathHelper::nextDouble(rand);
        double d2 = p_70972_1_.getminZ() + (p_70972_1_.getmaxZ() - p_70972_1_.getminZ()) * MathHelper::nextDouble(rand);
        world->spawnParticle(EnumParticleTypes::EXPLOSION_LARGE, d0, d1, d2, 0.0, 0.0, 0.0, {});
    }

    return flag;
}

void EntityDragon::dropExperience(int32_t p_184668_1_) const
{
    while (p_184668_1_ > 0)
    {
        auto i = EntityXPOrb::getXPSplit(p_184668_1_);
        p_184668_1_ -= i;
        world->spawnEntity(new EntityXPOrb(world, posX, posY, posZ, i));
    }
}
