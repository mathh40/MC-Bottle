#include "EntityAreaEffectCloud.h"

#include "../potion/PotionUtils.h"
#include "WorldServer.h"

RADIUS           = EntityDataManager.createKey(EntityAreaEffectCloud.class, DataSerializers::FLOAT);
COLOR            = EntityDataManager.createKey(EntityAreaEffectCloud.class, DataSerializers::VARINT);
IGNORE_RADIUS    = EntityDataManager.createKey(EntityAreaEffectCloud.class, DataSerializers::BOOLEAN);
PARTICLE         = EntityDataManager.createKey(EntityAreaEffectCloud.class, DataSerializers::VARINT);
PARTICLE_PARAM_1 = EntityDataManager.createKey(EntityAreaEffectCloud.class, DataSerializers::VARINT);
PARTICLE_PARAM_2 = EntityDataManager.createKey(EntityAreaEffectCloud.class, DataSerializers::VARINT);

EntityAreaEffectCloud::EntityAreaEffectCloud(World *worldIn)
    : Entity(worldIn), potion(PotionTypes::EMPTY), duration(600), waitTime(20), reapplicationDelay(20)
{
    noClip          = true;
    bisImmuneToFire = true;
    setRadius(3.0f);
}

EntityAreaEffectCloud::EntityAreaEffectCloud(World *worldIn, double x, double y, double z)
    : EntityAreaEffectCloud(worldIn)
{
    setPosition(x, y, z);
}

void EntityAreaEffectCloud::setRadius(float radiusIn)
{
    double d0 = posX;
    double d1 = posY;
    double d2 = posZ;
    setSize(radiusIn * 2.0F, 0.5F);
    setPosition(d0, d1, d2);
    if (!world->isRemote)
    {
        getDataManager().set(RADIUS, radiusIn);
    }
}

float EntityAreaEffectCloud::getRadius()
{
    return getDataManager().get(RADIUS);
}

void EntityAreaEffectCloud::setPotion(const PotionType &potionIn)
{
    potion = potionIn;
    if (!colorSet)
    {
        updateFixedColor();
    }
}

void EntityAreaEffectCloud::addEffect(const PotionEffect &effect)
{
    effects.add(effect);
    if (!colorSet)
    {
        updateFixedColor();
    }
}

int32_t EntityAreaEffectCloud::getColor()
{
    return getDataManager().get(COLOR);
}

void EntityAreaEffectCloud::setColor(int32_t colorIn)
{
    colorSet = true;
    getDataManager().set(COLOR, colorIn);
}

EnumParticleTypes EntityAreaEffectCloud::getParticle()
{
    return EnumParticleTypes::getParticleFromId(getDataManager().get(PARTICLE));
}

void EntityAreaEffectCloud::setParticle(const EnumParticleTypes &particleIn)
{
    getDataManager().set(PARTICLE, particleIn.getParticleID());
}

int32_t EntityAreaEffectCloud::getParticleParam1()
{
    return getDataManager().get(PARTICLE_PARAM_1);
}

void EntityAreaEffectCloud::setParticleParam1(int32_t particleParam)
{
    getDataManager().set(PARTICLE_PARAM_1, particleParam);
}

int32_t EntityAreaEffectCloud::getParticleParam2()
{
    return getDataManager().get(PARTICLE_PARAM_2);
}

void EntityAreaEffectCloud::setParticleParam2(int32_t particleParam)
{
    getDataManager().set(PARTICLE_PARAM_2, particleParam);
}

bool EntityAreaEffectCloud::shouldIgnoreRadius()
{
    return getDataManager().get(IGNORE_RADIUS);
}

int32_t EntityAreaEffectCloud::getDuration() const
{
    return duration;
}

void EntityAreaEffectCloud::setDuration(int32_t durationIn)
{
    duration = durationIn;
}

void EntityAreaEffectCloud::onUpdate()
{
    Entity::onUpdate();
    bool flag = shouldIgnoreRadius();
    float f   = getRadius();
    if (world->isRemote)
    {
        EnumParticleTypes enumparticletypes = getParticle();
        std::vector<int32_t> aint(enumparticletypes.getArgumentCount());
        if (aint.size() > 0)
        {
            aint[0] = getParticleParam1();
        }

        if (aint.size() > 1)
        {
            aint[1] = getParticleParam2();
        }

        float f2;
        float f3;
        float f4;
        int k;
        int l;
        int i1;
        if (flag)
        {
            if (randombool(rand))
            {
                for (auto i = 0; i < 2; ++i)
                {
                    float f1 = MathHelper::nextFloat(rand) * 6.2831855F;
                    f2       = MathHelper::sqrt(MathHelper::nextFloat(rand)) * 0.2F;
                    f3       = MathHelper::cos(f1) * f2;
                    f4       = MathHelper::sin(f1) * f2;
                    if (enumparticletypes == EnumParticleTypes::SPELL_MOB)
                    {
                        auto j = randombool(rand) ? 16777215 : getColor();
                        k      = j >> 16 & 255;
                        l      = j >> 8 & 255;
                        i1     = j & 255;
                        world->spawnAlwaysVisibleParticle(
                            EnumParticleTypes::SPELL_MOB.getParticleID(), posX + (double)f3, posY, posZ + (double)f4,
                            (double)((float)k / 255.0F), (double)((float)l / 255.0F), (double)((float)i1 / 255.0F));
                    }
                    else
                    {
                        world->spawnAlwaysVisibleParticle(enumparticletypes.getParticleID(), posX + (double)f3, posY,
                                                          posZ + (double)f4, 0.0, 0.0, 0.0, aint);
                    }
                }
            }
        }
        else
        {
            float f5 = 3.1415927F * f * f;

            for (auto k1 = 0; (float)k1 < f5; ++k1)
            {
                f2       = MathHelper::nextFloat(rand) * 6.2831855F;
                f3       = MathHelper::sqrt(MathHelper::nextFloat(rand)) * f;
                f4       = MathHelper::cos(f2) * f3;
                float f9 = MathHelper::sin(f2) * f3;
                if (enumparticletypes == EnumParticleTypes::SPELL_MOB)
                {
                    k       = getColor();
                    l       = k >> 16 & 255;
                    i1      = k >> 8 & 255;
                    auto j1 = k & 255;
                    world->spawnAlwaysVisibleParticle(EnumParticleTypes::SPELL_MOB.getParticleID(), posX + (double)f4,
                                                      posY, posZ + (double)f9, (double)((float)l / 255.0F),
                                                      (double)((float)i1 / 255.0F), (double)((float)j1 / 255.0F));
                }
                else
                {
                    world->spawnAlwaysVisibleParticle(enumparticletypes.getParticleID(), posX + (double)f4, posY,
                                                      posZ + (double)f9, (0.5 - MathHelper::nextDouble(rand)) * 0.15,
                                                      0.009999999776482582, (0.5 - MathHelper::nextDouble(rand)) * 0.15,
                                                      aint);
                }
            }
        }
    }
    else
    {
        if (ticksExisted >= waitTime + duration)
        {
            setDead();
            return;
        }

        bool flag1 = ticksExisted < waitTime;
        if (flag != flag1)
        {
            setIgnoreRadius(flag1);
        }

        if (flag1)
        {
            return;
        }

        if (radiusPerTick != 0.0F)
        {
            f += radiusPerTick;
            if (f < 0.5F)
            {
                setDead();
                return;
            }

            setRadius(f);
        }

        if (ticksExisted % 5 == 0)
        {
            Iterator iterator = reapplicationDelayMap.entrySet().iterator();

            while (iterator.hasNext())
            {
                Entry entry = (Entry)iterator.next();
                if (ticksExisted >= (Integer)entry.getValue())
                {
                    iterator.remove();
                }
            }

            List lstPotions = Lists.newArrayList();
            Iterator var23  = potion.getEffects().iterator();

            while (var23.hasNext())
            {
                PotionEffect potioneffect1 = (PotionEffect)var23.next();
                lstPotions.add(new PotionEffect(potioneffect1.getPotion(), potioneffect1.getDuration() / 4,
                                                potioneffect1.getAmplifier(), potioneffect1.getIsAmbient(),
                                                potioneffect1.doesShowParticles()));
            }

            lstPotions.addAll(effects);
            if (lstPotions.isEmpty())
            {
                reapplicationDelayMap.clear();
            }
            else
            {
                List list = world->getEntitiesWithinAABB(EntityLivingBase.class, getEntityBoundingBox());
                if (!list.isEmpty())
                {
                    Iterator var26 = list.iterator();

                    while (true)
                    {
                        EntityLivingBase *entitylivingbase;
                        double d2;
                        do
                        {
                            do
                            {
                                do
                                {
                                    if (!var26.hasNext())
                                    {
                                        return;
                                    }

                                    entitylivingbase = (EntityLivingBase)var26.next();
                                } while (reapplicationDelayMap.containsKey(entitylivingbase));
                            } while (!entitylivingbase->canBeHitWithPotion());

                            double d0 = entitylivingbase->posX - posX;
                            double d1 = entitylivingbase->posZ - posZ;
                            d2        = d0 * d0 + d1 * d1;
                        } while (d2 > (double)(f * f));

                        reapplicationDelayMap.put(entitylivingbase, ticksExisted + reapplicationDelay);
                        Iterator var15 = lstPotions.iterator();

                        while (var15.hasNext())
                        {
                            PotionEffect potioneffect = (PotionEffect)var15.next();
                            if (potioneffect.getPotion().isInstant())
                            {
                                potioneffect.getPotion().affectEntity(this, getOwner(), entitylivingbase,
                                                                      potioneffect.getAmplifier(), 0.5);
                            }
                            else
                            {
                                entitylivingbase->addPotionEffect(new PotionEffect(potioneffect));
                            }
                        }

                        if (radiusOnUse != 0.0F)
                        {
                            f += radiusOnUse;
                            if (f < 0.5F)
                            {
                                setDead();
                                return;
                            }

                            setRadius(f);
                        }

                        if (durationOnUse != 0)
                        {
                            duration += durationOnUse;
                            if (duration <= 0)
                            {
                                setDead();
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void EntityAreaEffectCloud::setRadiusOnUse(float radiusOnUseIn)
{
    radiusOnUse = radiusOnUseIn;
}

void EntityAreaEffectCloud::setRadiusPerTick(float radiusPerTickIn)
{
    radiusPerTick = radiusPerTickIn;
}

void EntityAreaEffectCloud::setWaitTime(int32_t waitTimeIn)
{
    waitTime = waitTimeIn;
}

void EntityAreaEffectCloud::setOwner(EntityLivingBase *ownerIn)
{
    owner         = ownerIn;
    ownerUniqueId = ownerIn == nullptr ? nullptr : ownerIn->getUniqueID();
}

EntityLivingBase *EntityAreaEffectCloud::getOwner()
{
    if (owner == nullptr && ownerUniqueId != nullptr && Util:: instanceof <WorldServer>(world))
    {
        Entity *entity = ((WorldServer *)world)->getEntityFromUuid(ownerUniqueId);
        if (Util:: instanceof <EntityLivingBase>(entity))
        {
            owner = (EntityLivingBase *)entity;
        }
    }

    return owner;
}

void EntityAreaEffectCloud::entityInit()
{
    getDataManager().register(COLOR, 0);
    getDataManager().register(RADIUS, 0.5F);
    getDataManager().register(IGNORE_RADIUS, false);
    getDataManager().register(PARTICLE, EnumParticleTypes::SPELL_MOB.getParticleID());
    getDataManager().register(PARTICLE_PARAM_1, 0);
    getDataManager().register(PARTICLE_PARAM_2, 0);
}

void EntityAreaEffectCloud::setIgnoreRadius(bool ignoreRadius)
{
    getDataManager().set(IGNORE_RADIUS, ignoreRadius);
}

void EntityAreaEffectCloud::readEntityFromNBT(NBTTagCompound *compound)
{
    ticksExisted       = compound->getInteger("Age");
    duration           = compound->getInteger("Duration");
    waitTime           = compound->getInteger("WaitTime");
    reapplicationDelay = compound->getInteger("ReapplicationDelay");
    durationOnUse      = compound->getInteger("DurationOnUse");
    radiusOnUse        = compound->getFloat("RadiusOnUse");
    radiusPerTick      = compound->getFloat("RadiusPerTick");
    setRadius(compound->getFloat("Radius"));
    ownerUniqueId = compound->getUniqueId("OwnerUUID");
    if (compound->hasKey("Particle", 8))
    {
        auto enumparticletypes = EnumParticleTypes::getByName(compound->getString("Particle"));
        if (enumparticletypes)
        {
            setParticle(enumparticletypes.value());
            setParticleParam1(compound->getInteger("ParticleParam1"));
            setParticleParam2(compound->getInteger("ParticleParam2"));
        }
    }

    if (compound->hasKey("Color", 99))
    {
        setColor(compound->getInteger("Color"));
    }

    if (compound->hasKey("Potion", 8))
    {
        setPotion(PotionUtils::getPotionTypeFromNBT(compound));
    }

    if (compound->hasKey("Effects", 9))
    {
        NBTTagList nbttaglist = compound->getTagList("Effects", 10);
        effects.clear();

        for (int i = 0; i < nbttaglist.tagCount(); ++i)
        {
            PotionEffect potioneffect = PotionEffect::readCustomPotionEffectFromNBT(nbttaglist.getCompoundTagAt(i));
            if (potioneffect != nullptr)
            {
                addEffect(potioneffect);
            }
        }
    }
}

void EntityAreaEffectCloud::writeEntityToNBT(NBTTagCompound *compound)
{
    compound->setInteger("Age", ticksExisted);
    compound->setInteger("Duration", duration);
    compound->setInteger("WaitTime", waitTime);
    compound->setInteger("ReapplicationDelay", reapplicationDelay);
    compound->setInteger("DurationOnUse", durationOnUse);
    compound->setFloat("RadiusOnUse", radiusOnUse);
    compound->setFloat("RadiusPerTick", radiusPerTick);
    compound->setFloat("Radius", getRadius());
    compound->setString("Particle", getParticle().getParticleName());
    compound->setInteger("ParticleParam1", getParticleParam1());
    compound->setInteger("ParticleParam2", getParticleParam2());
    if (ownerUniqueId != nullptr)
    {
        compound->setUniqueId("OwnerUUID", ownerUniqueId);
    }

    if (colorSet)
    {
        compound->setInteger("Color", getColor());
    }

    if (potion != PotionTypes::EMPTY && potion != nullptr)
    {
        compound->setString("Potion", ((ResourceLocation)PotionType::REGISTRY.getNameForObject(potion)).to_string());
    }

    if (!effects.empty())
    {
        NBTTagList nbttaglist = new NBTTagList();
        Iterator var3         = effects.iterator();

        while (var3.hasNext())
        {
            PotionEffect potioneffect = (PotionEffect)var3.next();
            nbttaglist.appendTag(potioneffect.writeCustomPotionEffectToNBT(new NBTTagCompound()));
        }

        compound->setTag("Effects", nbttaglist);
    }
}

void EntityAreaEffectCloud::notifyDataManagerChange(const DataParameter &key)
{
    if (RADIUS.equals(key))
    {
        setRadius(getRadius());
    }

    Entity::notifyDataManagerChange(key);
}

EnumPushReaction EntityAreaEffectCloud::getPushReaction()
{
    return EnumPushReaction::IGNORE;
}

void EntityAreaEffectCloud::updateFixedColor()
{
    if (potion == PotionTypes::EMPTY && effects.empty())
    {
        getDataManager().set(COLOR, 0);
    }
    else
    {
        getDataManager().set(COLOR,
                             PotionUtils::getPotionColorFromEffectList(PotionUtils::mergeEffects(potion, effects)));
    }
}
