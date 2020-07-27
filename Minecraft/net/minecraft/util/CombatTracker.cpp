#include "CombatTracker.h"
#include "math/BlockPos.h"
#include "DamageSource.h"
#include "../block/Block.h"
#include "text/TextComponentTranslation.h"

CombatTracker::CombatTracker(EntityLivingBase* fighterIn)
	:fighter(fighterIn)
{
}

void CombatTracker::calculateFallSuffix()
{
	resetFallSuffix();
	if (fighter->isOnLadder()) {
		Block block = fighter->world.getBlockState(BlockPos(fighter->posX, fighter->getEntityBoundingBox().minY, fighter->posZ)).getBlock();
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
	std::shared_ptr<CombatEntry> combatentry = std::make_shared<CombatEntry>(damageSrc, fighter->ticksExisted, healthIn, damageAmount, fallSuffix, fighter->fallDistance);
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

ITextComponent* CombatTracker::getDeathMessage()
{
	if (combatEntries.empty()) {
		return new TextComponentTranslation("death.attack.generic", new Object[]{ this.fighter.getDisplayName() });
	}
	else {
		CombatEntry combatentry = this.getBestCombatEntry();
		CombatEntry combatentry1 = (CombatEntry)this.combatEntries.get(this.combatEntries.size() - 1);
		ITextComponent itextcomponent1 = combatentry1.getDamageSrcDisplayName();
		Entity entity = combatentry1.getDamageSrc().getTrueSource();
		Object itextcomponent;
		if (combatentry != nullptr && combatentry1.getDamageSrc() == DamageSource.FALL) {
			ITextComponent itextcomponent2 = combatentry.getDamageSrcDisplayName();
			if (combatentry.getDamageSrc() != DamageSource.FALL && combatentry.getDamageSrc() != DamageSource.OUT_OF_WORLD) {
				if (itextcomponent2 == null || itextcomponent1 != null && itextcomponent2.equals(itextcomponent1)) {
					if (itextcomponent1 != null) {
						ItemStack itemstack = entity instanceof EntityLivingBase ? ((EntityLivingBase)entity).getHeldItemMainhand() : ItemStack.EMPTY;
						if (!itemstack.isEmpty() && itemstack.hasDisplayName()) {
							itextcomponent = new TextComponentTranslation("death.fell.finish.item", new Object[]{ this.fighter.getDisplayName(), itextcomponent1, itemstack.getTextComponent() });
						}
						else {
							itextcomponent = new TextComponentTranslation("death.fell.finish", new Object[]{ this.fighter.getDisplayName(), itextcomponent1 });
						}
					}
					else {
						itextcomponent = new TextComponentTranslation("death.fell.killer", new Object[]{ this.fighter.getDisplayName() });
					}
				}
				else {
					Entity entity1 = combatentry.getDamageSrc().getTrueSource();
					ItemStack itemstack1 = entity1 instanceof EntityLivingBase ? ((EntityLivingBase)entity1).getHeldItemMainhand() : ItemStack.EMPTY;
					if (!itemstack1.isEmpty() && itemstack1.hasDisplayName()) {
						itextcomponent = new TextComponentTranslation("death.fell.assist.item", new Object[]{ this.fighter.getDisplayName(), itextcomponent2, itemstack1.getTextComponent() });
					}
					else {
						itextcomponent = new TextComponentTranslation("death.fell.assist", new Object[]{ this.fighter.getDisplayName(), itextcomponent2 });
					}
				}
			}
			else {
				itextcomponent = new TextComponentTranslation("death.fell.accident." + this.getFallSuffix(combatentry), new Object[]{ this.fighter.getDisplayName() });
			}
		}
		else {
			itextcomponent = combatentry1.getDamageSrc().getDeathMessage(this.fighter);
		}

		return (ITextComponent)itextcomponent;
	}
}

EntityLivingBase* CombatTracker::getBestAttacker()
{
	EntityLivingBase* entitylivingbase;
	EntityPlayer* entityplayer;
	float f = 0.0F;
	float f1 = 0.0F;
	Iterator var5 = combatEntries.iterator();

	while (true) {
		CombatEntry combatentry;
		do {
			do {
				if (!var5.hasNext()) {
					if (entityplayer != nullptr && f1 >= f / 3.0F) {
						return entityplayer;
					}

					return entitylivingbase;
				}

				combatentry = (CombatEntry)var5.next();
				if (combatentry.getDamageSrc().getTrueSource() instanceof EntityPlayer && (entityplayer == null || combatentry.getDamage() > f1)) {
					f1 = combatentry.getDamage();
					entityplayer = (EntityPlayer)combatentry.getDamageSrc().getTrueSource();
				}
			} while (!(combatentry.getDamageSrc().getTrueSource() instanceof EntityLivingBase));
		} while (entitylivingbase != null && combatentry.getDamage() <= f);

		f = combatentry.getDamage();
		entitylivingbase = (EntityLivingBase)combatentry.getDamageSrc().getTrueSource();
	}
}

int32_t CombatTracker::getCombatDuration() const
{
	return inCombat ? fighter->ticksExisted - combatStartTime : combatEndTime - combatStartTime;
}

void CombatTracker::reset()
{
	int32_t i = inCombat ? 300 : 100;
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

std::string CombatTracker::getFallSuffix(CombatEntry entry)
{
	return entry.getFallSuffix().empty() ? "generic" : entry.getFallSuffix();
}

void CombatTracker::resetFallSuffix()
{
	fallSuffix.clear();
}

CombatEntry CombatTracker::getBestCombatEntry()
{
	CombatEntry combatentry;
	CombatEntry combatentry1;
	float f = 0.0F;
	float f1 = 0.0F;

	for (int i = 0; i < combatEntries.size(); ++i) {
		CombatEntry combatentry2 = (CombatEntry)combatEntries.get(i);
		CombatEntry combatentry3 = i > 0 ? (CombatEntry)combatEntries.get(i - 1) : null;
		if ((combatentry2.getDamageSrc() == DamageSource.FALL || combatentry2.getDamageSrc() == DamageSource.OUT_OF_WORLD) && combatentry2.getDamageAmount() > 0.0F && (combatentry == null || combatentry2.getDamageAmount() > f1)) {
			if (i > 0) {
				combatentry = combatentry3;
			}
			else {
				combatentry = combatentry2;
			}

			f1 = combatentry2.getDamageAmount();
		}

		if (combatentry2.getFallSuffix() != null && (combatentry1 == null || combatentry2.getDamage() > f)) {
			combatentry1 = combatentry2;
			f = combatentry2.getDamage();
		}
	}

	if (f1 > 5.0F && combatentry != null) {
		return combatentry;
	}
	else if (f > 5.0F && combatentry1 != null) {
		return combatentry1;
	}
	else {
		return null;
	}
}
