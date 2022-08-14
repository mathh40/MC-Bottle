#include "CombatTracker.h"
#include "math/BlockPos.h"
#include "DamageSource.h"
#include "../block/Block.h"
#include "text/TextComponentTranslation.h"
#include "../entity/EntityLivingBase.h"

CombatTracker::CombatTracker(EntityLivingBase* fighterIn)
	:fighter(fighterIn)
{
}

void CombatTracker::calculateFallSuffix()
{
	resetFallSuffix();
	if (fighter->isOnLadder()) {
        auto block =
            fighter->world
                ->getBlockState(BlockPos(fighter->posX, fighter->getEntityBoundingBox().getminY(), fighter->posZ))
                ->getBlock();
		if (block == Blocks::LADDER) {
			fallSuffix = "ladder";
		}
		else if (block == Blocks::VINE) {
			fallSuffix = "vines";
		}
	}
	else if (fighter->isInWater()) {
		fallSuffix = "water";
	}
}

void CombatTracker::trackDamage(const DamageSource::DamageSource & damageSrc, const float healthIn, const float damageAmount)
{
	reset();
	calculateFallSuffix();
    std::unique_ptr<CombatEntry> combatentry = std::make_unique<CombatEntry>(
        damageSrc, fighter->ticksExisted, healthIn, damageAmount, fallSuffix, fighter->fallDistance);
	combatEntries.push_back(combatentry);
	lastDamageTime = fighter->ticksExisted;
	takingDamage = true;
	if (combatentry->isLivingDamageSrc() && !inCombat && fighter->isEntityAlive()) {
		inCombat = true;
		combatStartTime = fighter->ticksExisted;
		combatEndTime = combatStartTime;
		fighter->sendEnterCombat();
	}
}

std::shared_ptr<ITextComponent> CombatTracker::getDeathMessage() {
	if (combatEntries.empty()) {
        constexpr std::string_view temp = "death.attack.generic";
        return std::make_unique<TextComponentTranslation>(temp,fighter->getDisplayName());
	}
	else {
		auto combatentry = getBestCombatEntry();
        auto combatentry1 = combatEntries[combatEntries.size() - 1].get();
		auto itextcomponent1 = combatentry1->getDamageSrcDisplayName();
		auto entity = combatentry1->getDamageSrc().getTrueSource();
        std::shared_ptr<ITextComponent> itextcomponent;
		if (combatentry1->getDamageSrc() == DamageSource::FALL) {
			auto itextcomponent2 = combatentry->getDamageSrcDisplayName();
			if (combatentry->getDamageSrc() != DamageSource::FALL && combatentry->getDamageSrc() != DamageSource::OUT_OF_WORLD) {
				if (itextcomponent2 == nullptr || itextcomponent1 != nullptr && itextcomponent2 == itextcomponent1) {
					if (itextcomponent1 != nullptr) {
                        auto itemstack = Util:: instanceof<EntityLivingBase>(entity) ? reinterpret_cast<EntityLivingBase*>(entity)->getHeldItemMainhand()
                                                       : ItemStack::EMPTY;
						if (!itemstack.isEmpty() && itemstack.hasDisplayName()) {
                            itextcomponent = std::make_unique<TextComponentTranslation>("death.fell.finish.item",
                                                         fighter->getDisplayName(), itextcomponent1,
                                                                      itemstack.getTextComponent());
						}
						else {
                            itextcomponent = std::make_unique<TextComponentTranslation>("death.fell.finish",
                                                         fighter->getDisplayName(), itextcomponent1);
						}
					}
					else {
                        itextcomponent = std::make_unique<TextComponentTranslation>("death.fell.killer", fighter->getDisplayName());
					}
				}
				else {
					auto entity1 = combatentry->getDamageSrc().getTrueSource();
                    ItemStack itemstack1 = Util:: instanceof
                        <EntityLivingBase>(entity1) ? reinterpret_cast<EntityLivingBase*>(entity1)->getHeldItemMainhand()
                                                    : ItemStack::EMPTY;
					if (!itemstack1.isEmpty() && itemstack1.hasDisplayName()) {
                        itextcomponent =
                            std::make_unique <
                            TextComponentTranslation>("death.fell.assist.item",
                                                     fighter->getDisplayName(), itextcomponent2,
                                                                  itemstack1.getTextComponent());
					}
					else {
                        itextcomponent =
                            std::make_unique <
                            TextComponentTranslation>("death.fell.assist",
                                                     fighter->getDisplayName(), itextcomponent2);
					}
				}
			}
			else {
                itextcomponent = std::make_unique <
                                 TextComponentTranslation>("death.fell.accident." + getFallSuffix(combatentry),
                                                          fighter->getDisplayName());
			}
		}
		else {
			itextcomponent = combatentry1->getDamageSrc().getDeathMessage(fighter);
		}

		return itextcomponent;
	}
}

EntityLivingBase* CombatTracker::getBestAttacker()
{
	EntityLivingBase* entitylivingbase = nullptr;
    EntityPlayer *entityplayer = nullptr;
	float f = 0.0F;
	float f1 = 0.0F;
    auto ite = combatEntries.begin();

	while (true) {
        CombatEntry* combatentry = nullptr;
		do {
			do {
                ++ite;
                if (ite == combatEntries.end()) {
					if (entityplayer != nullptr && f1 >= f / 3.0F) {
						return entityplayer;
					}

					return entitylivingbase;
				}					
				combatentry = (*ite).get();
                if (Util:: instanceof <EntityPlayer>(combatentry->getDamageSrc().getTrueSource()) && (entityplayer == nullptr || combatentry->getDamage() > f1)) {
					f1 = combatentry->getDamage();
                    entityplayer = reinterpret_cast < EntityPlayer* > (combatentry->getDamageSrc().getTrueSource());
				}
            } while (!(Util::instanceof<EntityLivingBase>(combatentry->getDamageSrc().getTrueSource())));
		} while (entitylivingbase != nullptr && combatentry->getDamage() <= f);

		f = combatentry->getDamage();
        entitylivingbase = reinterpret_cast < EntityLivingBase*>(combatentry->getDamageSrc().getTrueSource());
	}
}

uint32_t CombatTracker::getCombatDuration() const
{
	return inCombat ? fighter->ticksExisted - combatStartTime : combatEndTime - combatStartTime;
}

void CombatTracker::reset()
{
	uint32_t i = inCombat ? 300 : 100;
	if (takingDamage && (!fighter->isEntityAlive() || fighter->ticksExisted - lastDamageTime > i)) {
		bool flag = inCombat;
		takingDamage = false;
		inCombat = false;
		combatEndTime = fighter->ticksExisted;
		if (flag) {
			fighter->sendEndCombat();
		}

		combatEntries.clear();
	}
}

EntityLivingBase* CombatTracker::getFighter() const
{
	return fighter;
}

std::string CombatTracker::getFallSuffix(CombatEntry* entry)
{
	return entry->getFallSuffix().empty() ? "generic" : entry->getFallSuffix();
}

void CombatTracker::resetFallSuffix()
{
	fallSuffix.clear();
}

CombatEntry* CombatTracker::getBestCombatEntry()
{
    CombatEntry* combatentry = nullptr;
    CombatEntry *combatentry1 = nullptr;
	float f = 0.0F;
	float f1 = 0.0F;

	for (size_t i = 0; i < combatEntries.size(); ++i) {
        auto combatentry2 = combatEntries[i].get();
        auto combatentry3 = i > 0 ? combatEntries[(i - 1)].get() : nullptr;
		if ((combatentry2->getDamageSrc() == DamageSource::FALL || combatentry2->getDamageSrc() == DamageSource::OUT_OF_WORLD) && combatentry2->getDamageAmount() > 0.0F && (combatentry == nullptr || combatentry2->getDamageAmount() > f1)) {
			if (i > 0) {
				combatentry = combatentry3;
			}
			else {
				combatentry = combatentry2;
			}

			f1 = combatentry2->getDamageAmount();
		}

		if (!combatentry2->getFallSuffix().empty() &&
            (combatentry1 == nullptr || combatentry2->getDamage() > f)) {
			combatentry1 = combatentry2;
			f = combatentry2->getDamage();
		}
	}

	if (f1 > 5.0F && combatentry != nullptr) {
        return combatentry;
	}
	else if (f > 5.0F && combatentry1 != nullptr) {
        return combatentry1;
	}
	else {
        return nullptr;
	}
}
