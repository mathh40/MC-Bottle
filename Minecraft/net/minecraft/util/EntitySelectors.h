#pragma once
#include <functional>

class Entity;

namespace EntitySelectors
{
	bool IS_ALIVE(const Entity* lhs);
	bool IS_STANDALONE(const Entity* lhs);
	bool HAS_INVENTORY(const Entity* lhs);
	bool CAN_AI_TARGET(const Entity* lhs);
	bool NOT_SPECTATING(const Entity* lhs);
	bool withinRange(const Entity* lhs,const double x, const double y, const double z, double range);
	std::function<bool(const Entity *)> getTeamCollisionPredicate(const Entity* entityIn);
	bool notRiding(const Entity* p_191324_0_, const Entity *p_apply_1_);
};