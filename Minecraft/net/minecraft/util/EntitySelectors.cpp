#include "EntitySelectors.h"


#include "../entity/Entity.h"
#include "../scoreboard/Team.h"
#include "../world/Explosion.h"
#include <spdlog/fmt/bundled/format.h>

bool EntitySelectors::IS_ALIVE(const Entity * lhs)
{
	return lhs->isEntityAlive();
}

bool EntitySelectors::IS_STANDALONE(const Entity * lhs)
{
	return lhs->isEntityAlive() && !lhs->isBeingRidden() && !lhs->isRiding();;
}

bool EntitySelectors::HAS_INVENTORY(const Entity * lhs)
{
	return Util::instanceof<IInventory>(lhs) && lhs->isEntityAlive();
}

bool EntitySelectors::CAN_AI_TARGET(const Entity * lhs)
{
	return !(Util::instanceof<EntityPlayer>(lhs)  ) || !((EntityPlayer*)lhs)->isSpectator() && !((EntityPlayer*)lhs)->isCreative();
}

bool EntitySelectors::NOT_SPECTATING(const Entity * lhs)
{
	return !(Util::instanceof<EntityPlayer>(lhs)) || !((EntityPlayer*)lhs)->isSpectator();
}

std::function<bool(const Entity *)> EntitySelectors::getTeamCollisionPredicate(const Entity * entityIn)
{
	const Team* team = entityIn->getTeam();
	const Team::CollisionRule team$collisionrule = team == nullptr ? Team::CollisionRule::ALWAYS : team->getCollisionRule();
	auto ret = team$collisionrule == Team::CollisionRule::NEVER ? [](const Entity * lhs){return false;} : [&](const Entity * lhs){
        if (!lhs->canBePushed()) {
		    return false;
	    }
	    else if (entityIn->world->isRemote && (!(Util::instanceof<EntityPlayer>(lhs)) || !((EntityPlayer*)lhs)->isUser())) {
		    return false;
	    }
	    else {
		    Team* team1 = lhs->getTeam();
		    Team::CollisionRule team$collisionrule1 = team1 == nullptr ? Team::CollisionRule::ALWAYS : team1->getCollisionRule();
		    if (team$collisionrule1 == Team::CollisionRule::NEVER) {
			    return false;
		    }
		    else {
			    bool flag = team != nullptr && team->isSameTeam(team1);
			    if ((team$collisionrule == Team::CollisionRule::HIDE_FOR_OWN_TEAM || team$collisionrule1 == Team::CollisionRule::HIDE_FOR_OWN_TEAM) && flag) {
				    return false;
			    }
			    else {
				    return team$collisionrule != Team::CollisionRule::HIDE_FOR_OTHER_TEAMS && team$collisionrule1 != Team::CollisionRule::HIDE_FOR_OTHER_TEAMS || flag;
			    }
		    }
	    }
    };
	return ret;
}

bool EntitySelectors::notRiding(const Entity * p_191324_0_, const Entity *p_apply_1_)
{
	while (true) {
		if (p_apply_1_->isRiding()) {
			p_apply_1_ = p_apply_1_->getRidingEntity();
			if (p_apply_1_ != p_191324_0_) {
				continue;
			}

			return false;
		}

		return true;
	}
}

bool EntitySelectors::withinRange(const Entity* lhs,const double x, const double y, const double z, double range)
{
	auto d0 = range * range;
	return lhs->getDistanceSq(x, y, z) <= d0;
}