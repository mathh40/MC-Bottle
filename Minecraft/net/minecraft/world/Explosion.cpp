#include "Explosion.h"

#include "../entity/Entity.h"
#include "../entity/item/EntityTNTPrimed.h"
#include "../util/DamageSource.h"

#include <random>
#include <unordered_set>

Explosion::Explosion(World *worldIn, Entity *entityIn, double x, double y, double z, float size, bool causesFire,
                     bool damagesTerrain)
    : world(worldIn), exploder(entityIn), size(size), x(x), y(y), z(z), causesFire(causesFire),
      damagesTerrain(damagesTerrain)
{
}

void Explosion::doExplosionA()
{
    std::unordered_set<BlockPos> set;
    // int i = true;

    for (auto j = 0; j < 16; ++j)
    {
        for (auto k = 0; k < 16; ++k)
        {
            for (auto l = 0; l < 16; ++l)
            {
                if (j == 0 || j == 15 || k == 0 || k == 15 || l == 0 || l == 15)
                {
                    double d0     = j / 15.0F * 2.0F - 1.0F;
                    double d1     = k / 15.0F * 2.0F - 1.0F;
                    double d2     = l / 15.0F * 2.0F - 1.0F;
                    const auto d3 = MathHelper::sqrt(d0 * d0 + d1 * d1 + d2 * d2);
                    d0 /= d3;
                    d1 /= d3;
                    d2 /= d3;
                    std::uniform_real_distribution<float> e;
                    float f   = size * (0.7F + e(world->rand) * 0.6F);
                    double d4 = x;
                    double d6 = y;
                    double d8 = z;

                    for (float var21 = 0.3F; f > 0.0F; f -= 0.22500001F)
                    {
                        BlockPos blockpos(d4, d6, d8);
                        auto iblockstate = world->getBlockState(blockpos);
                        if (iblockstate->getMaterial() != Material::AIR)
                        {
                            float f2 = exploder != nullptr
                                           ? exploder->getExplosionResistance(this, world, blockpos, iblockstate)
                                           : iblockstate->getBlock()->getExplosionResistance(std::nullopt);
                            f -= (f2 + 0.3F) * 0.3F;
                        }

                        if (f > 0.0F && (exploder == nullptr ||
                                         exploder->canExplosionDestroyBlock(this, world, blockpos, iblockstate, f)))
                        {
                            set.emplace(blockpos);
                        }

                        d4 += d0 * 0.30000001192092896;
                        d6 += d1 * 0.30000001192092896;
                        d8 += d2 * 0.30000001192092896;
                    }
                }
            }
        }
    }

    affectedBlockPositions.insert(affectedBlockPositions.begin(), set.begin(), set.end());
    float f3 = size * 2.0F;
    auto k   = MathHelper::floor(x - f3 - 1.0);
    auto l   = MathHelper::floor(x + f3 + 1.0);
    int i2   = MathHelper::floor(y - f3 - 1.0);
    int i1   = MathHelper::floor(y + f3 + 1.0);
    int j2   = MathHelper::floor(z - f3 - 1.0);
    int j1   = MathHelper::floor(z + f3 + 1.0);

    AxisAlignedBB aabb(k, i2, j2, l, i1, j1);
    auto list = world->getEntitiesWithinAABBExcludingEntity(exploder, aabb);
    const Vec3d vec3d(x, y, z);

    for (auto entity : list)
    {
        if (!entity->isImmuneToExplosions())
        {
            double d12 = entity->getDistance(x, y, z) / f3;
            if (d12 <= 1.0)
            {
                double d5  = entity->posX - x;
                double d7  = entity->posY + entity->getEyeHeight() - y;
                double d9  = entity->posZ - z;
                double d13 = MathHelper::sqrt(d5 * d5 + d7 * d7 + d9 * d9);
                if (d13 != 0.0)
                {
                    d5 /= d13;
                    d7 /= d13;
                    d9 /= d13;
                    double d14 = world->getBlockDensity(vec3d, entity->getEntityBoundingBox());
                    double d10 = (1.0 - d12) * d14;
                    entity->attackEntityFrom(DamageSource::DamageSource.causeExplosionDamage(this),
                                             (d10 * d10 + d10) / 2.0 * 7.0 * f3 + 1.0);
                    double d11 = d10;
                    if (Util:: instanceof <EntityLivingBase>(entity))
                    {
                        d11 = EnchantmentProtection::getBlastDamageReduction((EntityLivingBase *)entity, d10);
                    }

                    entity->motionX += d5 * d11;
                    entity->motionY += d7 * d11;
                    entity->motionZ += d9 * d11;
                    if (Util:: instanceof <EntityPlayer>(entity))
                    {
                        EntityPlayer *entityplayer = reinterpret_cast<EntityPlayer *>(entity);
                        if (!entityplayer->isSpectator() &&
                            (!entityplayer->isCreative() || !entityplayer->capabilities.isFlying))
                        {
                            playerKnockbackMap.emplace(entityplayer, Vec3d(d5 * d10, d7 * d10, d9 * d10));
                        }
                    }
                }
            }
        }
    }
}

void Explosion::doExplosionB(bool spawnParticles)
{
    std::uniform_real_distribution<float> e;
    world->playSound(nullptr, x, y, z, SoundEvents::ENTITY_GENERIC_EXPLODE, SoundCategory::BLOCKS, 4.0F,
                     (1.0F + (e(world->rand) - e(world->rand)) * 0.2F) * 0.7F);
    if (size >= 2.0F && damagesTerrain)
    {
        world->spawnParticle(EnumParticleTypes::EXPLOSION_HUGE, x, y, z, 1.0, 0.0, 0.0);
    }
    else
    {
        world->spawnParticle(EnumParticleTypes::EXPLOSION_LARGE, x, y, z, 1.0, 0.0, 0.0);
    }

    if (damagesTerrain)
    {
        for (BlockPos blockpos : affectedBlockPositions)
        {
            auto iblockstate = world->getBlockState(blockpos);
            auto block       = iblockstate->getBlock();
            if (spawnParticles)
            {
                double d0 = blockpos.getx() + e(world->rand);
                double d1 = blockpos.gety() + e(world->rand);
                double d2 = blockpos.getz() + e(world->rand);
                double d3 = d0 - x;
                double d4 = d1 - y;
                double d5 = d2 - z;
                double d6 = MathHelper::sqrt(d3 * d3 + d4 * d4 + d5 * d5);
                d3 /= d6;
                d4 /= d6;
                d5 /= d6;
                double d7 = 0.5 / (d6 / size + 0.1);
                d7 *= (e(world->rand) * e(world->rand) + 0.3F);
                d3 *= d7;
                d4 *= d7;
                d5 *= d7;
                world->spawnParticle(EnumParticleTypes::EXPLOSION_NORMAL, (d0 + x) / 2.0, (d1 + y) / 2.0,
                                     (d2 + z) / 2.0, d3, d4, d5);
                world->spawnParticle(EnumParticleTypes::SMOKE_NORMAL, d0, d1, d2, d3, d4, d5);
            }

            if (iblockstate->getMaterial() != Material::AIR)
            {
                if (block->canDropFromExplosion(this))
                {
                    block->dropBlockAsItemWithChance(world, blockpos, world->getBlockState(blockpos), 1.0F / size, 0);
                }

                world->setBlockState(blockpos, Blocks::AIR->getDefaultState(), 3);
                block->onExplosionDestroy(world, blockpos, this);
            }
        }
    }

    if (causesFire)
    {
        for (BlockPos blockpos : affectedBlockPositions)
        {
            if (world->getBlockState(blockpos)->getMaterial() == Material::AIR &&
                world->getBlockState(blockpos.down())->isFullBlock() && world->rand(3) == 0)
            {
                world->setBlockState(blockpos, Blocks::FIRE->getDefaultState());
            }
        }
    }
}

std::unordered_map<EntityPlayer *, Vec3d> &Explosion::getPlayerKnockbackMap()
{
    return playerKnockbackMap;
}

EntityLivingBase *Explosion::getExplosivePlacedBy()
{
    if (exploder == nullptr)
    {
        return nullptr;
    }
    else if (Util:: instanceof <EntityTNTPrimed>(exploder))
    {
        return ((EntityTNTPrimed *)exploder)->getTntPlacedBy();
    }
    else
    {
        return Util:: instanceof
            <EntityLivingBase>(exploder) ? reinterpret_cast<EntityLivingBase *>(exploder) : nullptr;
    }
}

void Explosion::clearAffectedBlockPositions()
{
    affectedBlockPositions.clear();
}

std::vector<BlockPos> Explosion::getAffectedBlockPositions() const
{
    return affectedBlockPositions;
}
